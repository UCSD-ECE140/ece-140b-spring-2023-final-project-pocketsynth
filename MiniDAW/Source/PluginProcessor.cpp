/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MiniDAWAudioProcessor::MiniDAWAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETER", { createParameterLayout() }),
    eventDispatcher(userEvents), arrangement(eventDispatcher, treeState),
    saveManager(*this)
#endif
{
    testRunner.runAllTests();
}

MiniDAWAudioProcessor::~MiniDAWAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MiniDAWAudioProcessor::createParameterLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;

    // Global parameters
    layout.add(std::make_unique<AudioParameterFloat>(BPM_PARAM_ID, BPM_PARAM_NAME, BPM_MIN, BPM_MAX, 120.f));
    layout.add(std::make_unique<AudioParameterFloat>(GLOBAL_VOLUME_ID, GLOBAL_VOLUME_NAME, GLOBAL_VOLUME_MIN, GLOBAL_VOLUME_MAX, 0.f));

    // Lane parameters
    for (int i = 0; i < NUM_LANES; i++)
    {
        layout.add(std::make_unique<AudioParameterFloat>(getLaneParameter(LANE_VOLUME_ID, i), getLaneParameter(LANE_VOLUME_NAME, i), 
            GLOBAL_VOLUME_MIN, GLOBAL_VOLUME_MAX, 0.f));

        layout.add(std::make_unique<AudioParameterChoice>(getLaneParameter(LANE_SOUND_SELECT_ID, i), getLaneParameter(LANE_SOUND_SELECT_NAME, i),
            xynth::wavetableTypes, 0));

        layout.add(std::make_unique<AudioParameterFloat>(getLaneParameter(LANE_PLUCK_ID, i), getLaneParameter(LANE_PLUCK_NAME, i),
            0.f, 100.f, 0.f));

        layout.add(std::make_unique<AudioParameterFloat>(getLaneParameter(LANE_RELEASE_ID, i), getLaneParameter(LANE_RELEASE_NAME, i),
            0.f, 100.f, 0.f));

        layout.add(std::make_unique<AudioParameterFloat>(getLaneParameter(LANE_REVERB_ID, i), getLaneParameter(LANE_REVERB_NAME, i),
            0.f, 100.f, 0.f));
    }

    return layout;
}

//==============================================================================
const juce::String MiniDAWAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MiniDAWAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MiniDAWAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MiniDAWAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MiniDAWAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MiniDAWAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MiniDAWAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MiniDAWAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MiniDAWAudioProcessor::getProgramName (int index)
{
    return {};
}

void MiniDAWAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MiniDAWAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    using uint32 = juce::uint32;

    const auto channels = juce::jmax(getTotalNumInputChannels(), getTotalNumOutputChannels());
    juce::dsp::ProcessSpec spec = { sampleRate, (uint32)samplesPerBlock, (uint32)channels };

    arrangement.prepare(spec);
}

void MiniDAWAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MiniDAWAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MiniDAWAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Setup boilerplate
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // End of boilerplate

    eventDispatcher.processEvents();

    arrangement.process(buffer, midiMessages);
}

//==============================================================================
bool MiniDAWAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MiniDAWAudioProcessor::createEditor()
{
    return new MiniDAWAudioProcessorEditor (*this);
}

//==============================================================================
// Saving
void MiniDAWAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto saveState = saveManager.saveStateInXml();
    copyXmlToBinary(*saveState, destData);
}

// Loading
void MiniDAWAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() == nullptr)
        return;

    saveManager.loadStateFromXml(*xmlState);
}

xynth::RingBuffer& MiniDAWAudioProcessor::getLaneRingBuffer(const int laneIndex)
{
    return arrangement.getLaneRingBuffer(laneIndex);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MiniDAWAudioProcessor();
}
