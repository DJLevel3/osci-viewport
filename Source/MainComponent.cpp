#include "MainComponent.h"
#include "PluginEditor.h"

MainComponent::MainComponent(OscirenderAudioProcessor& p, OscirenderAudioProcessorEditor& editor) : audioProcessor(p), pluginEditor(editor) {
	addAndMakeVisible(visualiser);
}

MainComponent::~MainComponent() {
}

void MainComponent::resized() {
	auto bounds = getLocalBounds().reduced(5);

	auto minDim = juce::jmin(bounds.getWidth(), bounds.getHeight());
	visualiser.setBounds(bounds.withSizeKeepingCentre(minDim, minDim));
}

void MainComponent::paint(juce::Graphics& g) {
	juce::GroupComponent::paint(g);

	// add drop shadow to the visualiser
	auto dc = juce::DropShadow(juce::Colours::black, 30, juce::Point<int>(0, 0));
	dc.drawForRectangle(g, visualiser.getBounds());
}
