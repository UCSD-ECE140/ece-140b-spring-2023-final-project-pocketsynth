/*
  ==============================================================================

    SynthVoice.cpp
    Created: 15 Oct 2022 10:04:28am
    Author:  thesp

  ==============================================================================
*/

#include "SynthVoice.h"

namespace xynth
{
void SynthVoice::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    oscillator.prepare(spec);
    gain.prepare(spec);
    gain.updateDurationInSeconds(0.002);
    gain.resetGainLinear(0.f);

    voiceBuffer.setSize(spec.numChannels, spec.maximumBlockSize);

    for (auto& filter : pluckFilter)
        filter.prepare(spec);
}

void SynthVoice::setQueuePos(int newQueuePos)
{
    queuePos = newQueuePos;
}

void SynthVoice::startNote(int _midiNoteNumber, float velocity, int currentPitchWheelPosition)
{
    gain.resetGainLinear(1.f);
    gain.updateDurationInSeconds(0.5);
    gain.setGainLinear(0.1f);

    pluckCutoff = 21000.f;
    for (auto& filter : pluckFilter)
        filter.reset();

    midiNoteNumber = _midiNoteNumber;
    oscillator.resetAngle();
    oscillator.setFrequency((float)juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    noteOn = true;
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    gain.updateDurationInSeconds(release);
    gain.setGainLinear(0.f);
    noteOn = false;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (gain.isSilent() || numSamples == 0) return;

    auto audioBlock = juce::dsp::AudioBlock<float>(voiceBuffer).getSubBlock(startSample, numSamples);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    oscillator.process(context);
    gain.process(context);
    processPluck(context);

    for (int ch = 0; ch < outputBuffer.getNumChannels(); ch++)
        outputBuffer.addFrom(ch, startSample, voiceBuffer, ch, startSample, numSamples);
}

void SynthVoice::setSound(const int soundIndex)
{
    oscillator.setSound(soundIndex);
}

void SynthVoice::setPluckAmount(float pluckAmount)
{
    pluckAmount /= 2.f;
    pluckCoefficient = 1.f - (pluckAmount / (float)sampleRate);
}

void SynthVoice::setRelease(const float newRelease)
{
    release = newRelease;
}

void SynthVoice::processPluck(juce::dsp::ProcessContextReplacing<float>& context)
{
    const auto numChannels = context.getInputBlock().getNumChannels();
    const auto numSamples = context.getInputBlock().getNumSamples();

    const auto& inBlock = context.getInputBlock();
    auto& outBlock = context.getOutputBlock();

    const float startPluckCutoff = pluckCutoff;

    for (size_t ch = 0; ch < numChannels; ch++)
    {
        pluckCutoff = startPluckCutoff;

        auto in = inBlock.getChannelPointer(ch);
        auto out = outBlock.getChannelPointer(ch);

        for (int i = 0; i < numSamples; i++)
        {
            if (i % 20 == 0)
                pluckFilter[ch].parameters->setCutOffFrequency(sampleRate, pluckCutoff);

            pluckCutoff *= pluckCoefficient;
            pluckCutoff = std::max(pluckCutoff, 20.f);
            out[i] = pluckFilter[ch].processSample(in[i]);
        }
    }
}

} // namespace xynth