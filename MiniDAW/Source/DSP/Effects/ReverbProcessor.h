/*
  ==============================================================================

    ReverbProcessor.h
    Created: 7 Jun 2023 8:48:59pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{

class ReverbProcessor
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setAmount(const float reverbAmount);
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters parameters;

    juce::AudioBuffer<float> wetBuffer;

    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>,
        juce::dsp::StateVariableFilter::Parameters<float>> filter;

};

} // namespace xynth