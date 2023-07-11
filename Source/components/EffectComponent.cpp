#include "EffectComponent.h"

EffectComponent::EffectComponent(double min, double max, double step, double value, juce::String name, juce::String id) : name(name), id(id) {
    componentSetup();
	slider.setRange(min, max, step);
    slider.setValue(value, juce::dontSendNotification);
}

EffectComponent::EffectComponent(double min, double max, double step, EffectDetails details) : name(details.name), id(details.id) {
    componentSetup();
    slider.setRange(min, max, step);
    slider.setValue(details.value, juce::dontSendNotification);
}

EffectComponent::EffectComponent(double min, double max, double step, EffectDetails details, bool checkboxVisible) : EffectComponent(min, max, step, details) {
	setCheckboxVisible(checkboxVisible);
}

EffectComponent::EffectComponent(double min, double max, double step, Effect& effect) : EffectComponent(min, max, step, effect.getDetails()[0]) {}

EffectComponent::EffectComponent(double min, double max, double step, Effect& effect, bool checkboxVisible) : EffectComponent(min, max, step, effect) {
    setCheckboxVisible(checkboxVisible);
}

void EffectComponent::componentSetup() {
    addAndMakeVisible(slider);
    addAndMakeVisible(selected);

    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, slider.getTextBoxHeight());

    selected.setToggleState(false, juce::dontSendNotification);
}


EffectComponent::~EffectComponent() {}

void EffectComponent::resized() {
    auto sliderRight = getWidth() - 160;
    auto bounds = getLocalBounds();
    auto componentBounds = bounds.removeFromRight(25);
    if (component != nullptr) {
		component->setBounds(componentBounds);
	}

    slider.setBounds(bounds.removeFromRight(sliderRight));
    if (checkboxVisible) {
        bounds.removeFromLeft(2);
        selected.setBounds(bounds.removeFromLeft(25));
    } else {
        bounds.removeFromLeft(5);
    }
    textBounds = bounds;
}

void EffectComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText(name, textBounds, juce::Justification::left);
}

void EffectComponent::setComponent(std::shared_ptr<juce::Component> component) {
	this->component = component;
    addAndMakeVisible(component.get());
}

void EffectComponent::setCheckboxVisible(bool visible) {
    checkboxVisible = visible;
}
