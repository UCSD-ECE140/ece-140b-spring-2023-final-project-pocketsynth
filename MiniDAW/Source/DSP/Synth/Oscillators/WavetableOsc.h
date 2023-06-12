/*
  ==============================================================================

    MyOsc.h
    Created: 14 Jul 2022 5:55:10pm
    Author:  Speechrezz

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Wavetable.h"

namespace xynth
{

class WavetableOsc
{
public:
    WavetableOsc() = default;
    ~WavetableOsc() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void setFrequency(const float frequency);
    void resetAngle() { currentAngle = 0.f; }

    void process(juce::dsp::ProcessContextReplacing<float> context);

    void setSound(const int soundIndex);

private:
    void updateAngleDelta(const float frequency);
    float sampleRate = 0.f, currentAngle = 0.f, angleDelta = 0.f;

    Wavetable wavetable;
};

}