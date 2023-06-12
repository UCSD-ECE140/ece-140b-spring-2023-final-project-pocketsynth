/*
  ==============================================================================

    SynthProcessor.h
    Created: 15 Oct 2022 9:32:45am
    Author:  thesp

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthVoice.h"
#include "../../Common/Constants.h"

namespace xynth
{
class SynthProcessor
{
public:
    SynthProcessor() = default;
    ~SynthProcessor() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi);
    void reset();
    
    void setWavetableSound(const int soundIndex);
    void setPluckAmount(const float pluckAmount);
    void setReleaseAmount(float releaseAmount);

private:
    void renderSynthVoices(juce::AudioBuffer<float>& outputAudio, int prevSamplePos, int numSamples);

    void handleMidiEvent(const juce::MidiMessage& midiMessage);
    void noteOn (const juce::MidiMessage& midiMessage);
    void noteOff(const juce::MidiMessage& midiMessage);

    int findExistingNoteIndex(const int midiNoteNumber);
    int findEmptyNoteIndex();
    int findOldestNoteIndex(const int minQueuePos = 0);
    void updateNoteQueue();

    void synthVoiceArrayDBG();

    double sampleRate = 0.0;
    std::array<SynthVoice, MAX_VOICES> synthVoiceArray;

    int newQueuePos = 0;
    int previousSoundIndex = -1;

};
} // namespace xynth
