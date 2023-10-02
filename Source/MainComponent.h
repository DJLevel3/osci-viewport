#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "parser/FileParser.h"
#include "parser/FrameProducer.h"
#include "components/VisualiserComponent.h"

class OscirenderAudioProcessorEditor;
class MainComponent : public juce::GroupComponent {
public:
	MainComponent(OscirenderAudioProcessor&, OscirenderAudioProcessorEditor&);
	~MainComponent() override;

	void resized() override;
	void paint(juce::Graphics& g) override;
private:
	OscirenderAudioProcessor& audioProcessor;
	OscirenderAudioProcessorEditor& pluginEditor;
	
	VisualiserComponent visualiser{ 2, audioProcessor };
	juce::TextButton openOscilloscope{"Open Oscilloscope"};

	juce::Label frequencyLabel;
	int callbackIndex = -1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};