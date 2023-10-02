/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>
#include "concurrency/BufferConsumer.h"
#include <numbers>
#include "audio/AudioWebSocketServer.h"

//==============================================================================
/**
*/
class OscirenderAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorParameter::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    OscirenderAudioProcessor();
    ~OscirenderAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    std::shared_ptr<BufferConsumer> consumerRegister(std::vector<float>& buffer);
    void consumerStop(std::shared_ptr<BufferConsumer> consumer);
    void consumerRead(std::shared_ptr<BufferConsumer> consumer);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
    
    int VERSION_HINT = 1;

    std::atomic<double> currentSampleRate = 0.0;

    juce::ChangeBroadcaster broadcaster;
    std::atomic<bool> objectServerRendering = false;
    juce::ChangeBroadcaster fileChangeBroadcaster;

private:
    juce::SpinLock consumerLock;
    std::vector<std::shared_ptr<BufferConsumer>> consumers;
public:

    // shouldn't be accessed by audio thread, but needs to persist when GUI is closed
    // so should only be accessed by message thread
    juce::String currentProjectFile;

    juce::SpinLock fontLock;
    juce::Font font = juce::Font(juce::Font::getDefaultSansSerifFontName(), 1.0f, juce::Font::plain);

private:
    std::atomic<double> volume = 1.0;
    std::atomic<double> threshold = 1.0;

    juce::Synthesiser synth;

    AudioWebSocketServer softwareOscilloscopeServer{*this};

    const double MIN_LENGTH_INCREMENT = 0.000001;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscirenderAudioProcessor)
};
