/*
  ==============================================================================

    SynthVoice.h
    Created: 15 Oct 2022 10:04:28am
    Author:  thesp

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Oscillators/WavetableOsc.h"
#include "Gain.h"

namespace xynth
{
class SynthVoice
{
public:
    SynthVoice() = default;
    ~SynthVoice() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);

    void setQueuePos(int newQueuePos);
    void startNote(int midiNoteNumber, float velocity, int currentPitchWheelPosition);
    void stopNote(float velocity, bool allowTailOff);

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples);

    // For SynthProcessor
    inline bool isCurrentlyPlaying() { return !gain.isSilent(); }
    inline int getQueuePos() const { return queuePos; }
    inline int getMidiNoteNumber() const { return midiNoteNumber; }

    void setSound(const int soundIndex);
    void setPluckAmount(float pluckAmount);
    
    void setRelease(const float release);

private:
    void processPluck(juce::dsp::ProcessContextReplacing<float>& context);

    double sampleRate = 0;
    WavetableOsc oscillator;
    bool noteOn = false;

    juce::AudioBuffer<float> voiceBuffer;

    // ADSR gain
    xynth::Gain gain;
    float release = 0.002;

    // For SynthProcessor
    int queuePos = 0, midiNoteNumber = 0;

    // Pluck
    std::array<juce::dsp::StateVariableFilter::Filter<float>, 2> pluckFilter;
    float pluckCoefficient = 1.f;
    float pluckCutoff = 20000.f;

};
} // namespace xynth