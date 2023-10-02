/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscirenderAudioProcessor::OscirenderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    {
}

OscirenderAudioProcessor::~OscirenderAudioProcessor() {
}

const juce::String OscirenderAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool OscirenderAudioProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OscirenderAudioProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OscirenderAudioProcessor::isMidiEffect() const {
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OscirenderAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int OscirenderAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OscirenderAudioProcessor::getCurrentProgram() {
    return 0;
}

void OscirenderAudioProcessor::setCurrentProgram(int index) {
}

const juce::String OscirenderAudioProcessor::getProgramName(int index) {
    return {};
}

void OscirenderAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

void OscirenderAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	currentSampleRate = sampleRate;
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void OscirenderAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OscirenderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OscirenderAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto* channelData = buffer.getArrayOfWritePointers();
    
	for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {

        double x = buffer.getSample(0, sample);
        double y = buffer.getSample(1, sample);

        
        // clip
        x = juce::jmax(-threshold, juce::jmin(threshold.load(), x));
        y = juce::jmax(-threshold, juce::jmin(threshold.load(), y));
        

        if (totalNumOutputChannels >= 2) {
			channelData[0][sample] = x;
			channelData[1][sample] = y;
		} else if (totalNumOutputChannels == 1) {
            channelData[0][sample] = x;
        }

        {
            juce::SpinLock::ScopedLockType scope(consumerLock);
            for (auto consumer : consumers) {
                consumer->write(x);
                consumer->write(y);
                consumer->notifyIfFull();
            }
        }
	}
}

//==============================================================================
bool OscirenderAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OscirenderAudioProcessor::createEditor() {
    auto editor = new OscirenderAudioProcessorEditor(*this);
    return editor;
}

//==============================================================================
void OscirenderAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {

    std::unique_ptr<juce::XmlElement> xml = std::make_unique<juce::XmlElement>("project");

    copyXmlToBinary(*xml, destData);
}

void OscirenderAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml;

    const uint32_t magicXmlNumber = 0x21324356;
    if (sizeInBytes > 8 && juce::ByteOrder::littleEndianInt(data) == magicXmlNumber) {
        // this is a binary xml format
        xml = getXmlFromBinary(data, sizeInBytes);
    } else {
        // this is a text xml format
        xml = juce::XmlDocument::parse(juce::String((const char*)data, sizeInBytes));
    }

    if (xml.get() != nullptr && xml->hasTagName("project")) {
    }
}

std::shared_ptr<BufferConsumer> OscirenderAudioProcessor::consumerRegister(std::vector<float>& buffer) {
    std::shared_ptr<BufferConsumer> consumer = std::make_shared<BufferConsumer>(buffer);
    juce::SpinLock::ScopedLockType scope(consumerLock);
    consumers.push_back(consumer);
    
    return consumer;
}

void OscirenderAudioProcessor::consumerRead(std::shared_ptr<BufferConsumer> consumer) {
    consumer->waitUntilFull();
    juce::SpinLock::ScopedLockType scope(consumerLock);
    consumers.erase(std::remove(consumers.begin(), consumers.end(), consumer), consumers.end());
}

void OscirenderAudioProcessor::consumerStop(std::shared_ptr<BufferConsumer> consumer) {
    if (consumer != nullptr) {
        juce::SpinLock::ScopedLockType scope(consumerLock);
        consumer->forceNotify();
    }
}

void OscirenderAudioProcessor::parameterValueChanged(int parameterIndex, float newValue) {
    // call apply on lua effects
}

void OscirenderAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting) {}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscirenderAudioProcessor();
}
