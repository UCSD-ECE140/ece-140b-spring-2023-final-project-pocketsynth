/*
  ==============================================================================

    Arrangement.h
    Created: 15 May 2023 10:16:58am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SongData.h"
#include "MidiLane/MidiLane.h"
#include "Metronome/MetronomeLane.h"
#include "UserEventDispatcher.h"
#include "Storable.h"

namespace xynth
{

class Arrangement : public Storable
{
public:
    Arrangement(UserEventDispatcher& eventDispatcher, juce::AudioProcessorValueTreeState& treeState);

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi);

    void saveState(juce::XmlElement& xmlElement) override;
    void loadState(const juce::XmlElement& xmlElement) override;

    RingBuffer& getLaneRingBuffer(const int laneIndex);
    void clearRecording();

private:
    void processMidiLanes(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi);

    std::array<MidiLane, NUM_LANES> midiLaneList;
    MetronomeLane metronomeLane;
    SongData songData;

    // To send to non-focused lanes
    juce::MidiBuffer emptyMidiBuffer;

    std::atomic<float>* bpmParameter;
    std::atomic<float>* volumeParameter;
    juce::dsp::Gain<float> gain;

    int laneIndex = 0;

};

} // namespace xynth