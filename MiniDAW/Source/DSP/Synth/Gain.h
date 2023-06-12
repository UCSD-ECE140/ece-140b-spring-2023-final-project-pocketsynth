/*
  ==============================================================================

    Gain.h
    Created: 9 Oct 2022 4:15:16pm
    Author:  thesp

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace xynth
{
class Gain
{
public:
    Gain() = default;
    ~Gain() = default;

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    void reset() noexcept;

    void updateDurationInSeconds(double duration) noexcept;
    void setGainLinear(float gain) noexcept;
    void resetGainLinear(float gain) noexcept;

    void process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept;

    inline bool isSilent() noexcept { return !isSmoothing() && gain.getCurrentValue() == 0.f; }
    inline bool isSmoothing() noexcept { return gain.isSmoothing(); }

private:
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> gain;
    double sampleRate = 0.0, duration = 1.0;

};
}