/*
  ==============================================================================

    MidiLane.h
    Created: 16 May 2023 9:56:04am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include "../Lane.h"
#include "MidiStore.h"
#include "../UserEventDispatcher.h"
#include "../Storable.h"
#include "../../../Common/RingBuffer.h"
#include "../../Effects/ReverbProcessor.h"

namespace xynth
{

class MidiLane : public Lane, public Storable
{
public:
    void setParameters(juce::AudioProcessorValueTreeState& treeState);

    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi,
        const SongData& songData) override;

    void saveState(juce::XmlElement& xmlElement) override;
    void loadState(const juce::XmlElement& xmlElement) override;

    void clearRecording();
    void setLaneIndex(const int newLaneIndex);

    RingBuffer ringBuffer;

protected:
    juce::String getName();

    SynthProcessor synthProcessor;

    juce::AudioBuffer<float> audioBuffer;
    juce::MidiBuffer midiSubBuffer;
    juce::dsp::Gain<float> gain;
    ReverbProcessor reverb;

    std::atomic<float>* volumeParameter;
    std::atomic<float>* soundSelectParameter;
    std::atomic<float>* pluckParameter;
    std::atomic<float>* releaseParameter;
    std::atomic<float>* reverbParameter;

    MidiStore midiStore;

    int laneIndex = -1;

};

} // namespace xynth
