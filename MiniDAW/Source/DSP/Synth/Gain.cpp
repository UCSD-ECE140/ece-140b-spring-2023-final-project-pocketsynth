/*
  ==============================================================================

    Gain.cpp
    Created: 9 Oct 2022 4:15:16pm
    Author:  thesp

  ==============================================================================
*/

#include "Gain.h"

namespace xynth
{
void Gain::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    sampleRate = spec.sampleRate;
}

void Gain::reset() noexcept
{
    gain.reset(sampleRate, duration);
}

void Gain::updateDurationInSeconds(double _duration) noexcept
{
    if (_duration == duration) return;

    duration = _duration;
    const float current = gain.getCurrentValue();
    const float target  = gain.getTargetValue();

    gain.reset(sampleRate, duration);

    gain.setCurrentAndTargetValue(current);
    gain.setTargetValue(target);
}

void Gain::setGainLinear(float newGain) noexcept
{
    gain.setTargetValue(newGain);
}

void Gain::resetGainLinear(float newGain) noexcept
{
    gain.setCurrentAndTargetValue(newGain);
}

void Gain::process(const juce::dsp::ProcessContextReplacing<float>& context) noexcept
{
    jassert(sampleRate != 0.0);

    auto&  inBlock = context.getInputBlock();
    auto& outBlock = context.getOutputBlock();

    auto len = inBlock.getNumSamples();
    auto numChannels = inBlock.getNumChannels();

    // Ignore these warnings, MSVC hates alloca(), since alloca can't raise exceptions.
    // This is good for performance though.
    // In very rare cases (where the buffer size is huge), this could result in stack overflows
    // We need to allocate this array on the stack, or else the performance penalty of allocation will
    // outweigh the improvements from SIMD.
    auto* gains = static_cast<float*> (alloca(sizeof(float) * len));

    for (size_t i = 0; i < len; ++i)
        gains[i] = gain.getNextValue();

    for (size_t chan = 0; chan < numChannels; ++chan)
        juce::FloatVectorOperations::multiply(outBlock.getChannelPointer(chan),
            inBlock.getChannelPointer(chan),
            gains, static_cast<int> (len));
}
} // namespace xynth