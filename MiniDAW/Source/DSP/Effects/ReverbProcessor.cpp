/*
  ==============================================================================

    ReverbProcessor.cpp
    Created: 7 Jun 2023 8:48:59pm
    Author:  Mark

  ==============================================================================
*/

#include "ReverbProcessor.h"

namespace xynth
{

void ReverbProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    reverb.prepare(spec);
    filter.prepare(spec);

    parameters.dryLevel = 0.f;
    reverb.setParameters(parameters);

    wetBuffer.setSize(spec.numChannels, spec.maximumBlockSize);

    filter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
    filter.state->setCutOffFrequency(spec.sampleRate, 200.f, 0.5f);
}

void ReverbProcessor::setAmount(const float reverbAmount)
{
    juce::NormalisableRange<float> range(0.f, 1.f);
    range.setSkewForCentre(0.2f);

    parameters.wetLevel = range.convertFrom0to1(reverbAmount / 100.f);
    parameters.roomSize = 0.5f + reverbAmount / 200.f;
    reverb.setParameters(parameters);
}

void ReverbProcessor::process(juce::dsp::ProcessContextReplacing<float>& inputContext)
{
    const auto numSamples = inputContext.getInputBlock().getNumSamples();

    const auto& dryBlock = inputContext.getInputBlock();
    auto wetBlock = juce::dsp::AudioBlock<float>(wetBuffer).getSubBlock(0, numSamples);
    auto context = juce::dsp::ProcessContextNonReplacing<float>(dryBlock, wetBlock);

    reverb.process(context);

    juce::dsp::ProcessContextReplacing<float> wetContext(wetBlock);
    filter.process(wetContext);

    auto& outputBlock = inputContext.getOutputBlock();
    outputBlock.add(wetBlock);
}

} // namespace xynth