/*
  ==============================================================================

    MyOsc.cpp
    Created: 14 Jul 2022 5:55:10pm
    Author:  Speechrezz

  ==============================================================================
*/

#include "WavetableOsc.h"

namespace xynth
{

void WavetableOsc::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = (float)spec.sampleRate;
    wavetable.prepare(spec);
}

void WavetableOsc::setFrequency(float frequency)
{
    updateAngleDelta(frequency);
    wavetable.setFrequency(frequency);
}

void WavetableOsc::process(juce::dsp::ProcessContextReplacing<float> context)
{
    auto left  = context.getOutputBlock().getChannelPointer(0);
    auto right = context.getOutputBlock().getChannelPointer(1);
    const auto numSamples = context.getOutputBlock().getNumSamples();

    for (int i = 0; i < numSamples; ++i)
    {
        const float currentSample = wavetable[currentAngle];
        currentAngle += angleDelta;
        currentAngle = fmod(currentAngle, float(FFT_SIZE));
        left [i] = currentSample;
        right[i] = currentSample;
    }
}

void WavetableOsc::setSound(const int soundIndex)
{
    wavetable.setSound(soundIndex);
}

void WavetableOsc::updateAngleDelta(const float frequency)
{
    float cyclesPerSample = float(frequency) / sampleRate;
    angleDelta = cyclesPerSample * float(FFT_SIZE);
}

} // namespace xynth