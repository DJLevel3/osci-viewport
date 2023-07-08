#include "VisualiserComponent.h"

VisualiserComponent::VisualiserComponent(int numChannels, OscirenderAudioProcessor& p) : numChannels(numChannels), backgroundColour(juce::Colours::black), waveformColour(juce::Colour(0xff00ff00)), audioProcessor(p) {
    setOpaque(true);
}

VisualiserComponent::~VisualiserComponent() {}

void VisualiserComponent::setBuffer(std::vector<float>& newBuffer) {
    juce::SpinLock::ScopedLockType scope(lock);
    buffer.clear();
    for (int i = 0; i < newBuffer.size(); i += precision * numChannels) {
        buffer.push_back(newBuffer[i]);
        buffer.push_back(newBuffer[i + 1]);
    }
}

void VisualiserComponent::setColours(juce::Colour bk, juce::Colour fg) {
    backgroundColour = bk;
    waveformColour = fg;
}

void VisualiserComponent::paint(juce::Graphics& g) {
    g.fillAll(backgroundColour);

    auto r = getLocalBounds().toFloat();
    auto channelHeight = r.getHeight() / (float)numChannels;

    g.setColour(waveformColour);
    juce::SpinLock::ScopedLockType scope(lock);
    paintXY(g, r.removeFromRight(r.getHeight()));

    for (int i = 0; i < numChannels; ++i) {
        paintChannel(g, r.removeFromTop(channelHeight), i);
    }
}

void VisualiserComponent::paintChannel(juce::Graphics& g, juce::Rectangle<float> area, int channel) {
    juce::Path path;

    for (int i = 0; i < buffer.size(); i += numChannels) {
        auto sample = buffer[i + channel];

        if (i == 0) {
            path.startNewSubPath(0.0f, sample);
        } else {
            path.lineTo((float)i, sample);
        }
    }

    // apply affine transform to path to fit in area
    auto transform = juce::AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(), 0.0f, 1.0f, area.getX(), area.getBottom(), buffer.size(), -1.0f, area.getRight(), area.getY());
    path.applyTransform(transform);

    g.strokePath(path, juce::PathStrokeType(1.0f));
}

void VisualiserComponent::paintXY(juce::Graphics& g, juce::Rectangle<float> area) {
    juce::Path path;

    path.startNewSubPath(buffer[0], buffer[1]);

    for (int i = 2; i < buffer.size(); i += 2) {
        path.lineTo(buffer[i + 0], buffer[i + 1]);
    }

    // apply affine transform to path to fit in area
    auto transform = juce::AffineTransform::fromTargetPoints(-1.0f, -1.0f, area.getX(), area.getBottom(), 1.0f, 1.0f, area.getRight(), area.getY(), 1.0f, -1.0f, area.getRight(), area.getBottom());
    path.applyTransform(transform);

    g.strokePath(path, juce::PathStrokeType(1.0f));
}
