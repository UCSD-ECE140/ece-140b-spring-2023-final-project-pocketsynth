/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/Controllers/Arrangement.h"
#include "DSP/Controllers/UserEventDispatcher.h"
#include "Common/Events.h"
#include "Common/SaveManager.h"
#include "DSP/Testing/MidiStoreTests.h"

class MiniDAWAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    MiniDAWAudioProcessor();
    ~MiniDAWAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    xynth::Arrangement& getArrangement() { return arrangement; }
    xynth::RingBuffer& getLaneRingBuffer(const int laneIndex);

    juce::AudioProcessorValueTreeState treeState;

    xynth::AudioEventBuffer audioEvents; // Outgoing
    xynth::UserEventBuffer  userEvents;  // Incoming

    friend xynth::SaveManager;
    xynth::SaveManager saveManager;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    xynth::UserEventDispatcher eventDispatcher;
    xynth::Arrangement arrangement;

    juce::UnitTestRunner testRunner;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniDAWAudioProcessor)
};
