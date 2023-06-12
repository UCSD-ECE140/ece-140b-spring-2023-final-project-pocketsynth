/*
  ==============================================================================

    SongData.cpp
    Created: 16 May 2023 10:34:54am
    Author:  Mark

  ==============================================================================
*/

#include "SongData.h"

namespace xynth
{

void SongData::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    updateBPM(beatsPerMinute);
}

void SongData::setBufferSize(juce::AudioBuffer<float>& outputAudio)
{
    bufferSize.samples = outputAudio.getNumSamples();
    bufferSize.calculateBeats(beatsPerSample);
}

void SongData::incrementPlayHead()
{
    jassert(bufferSize.samples != -1); // Call setBufferSize() first
    jassert(songLength.samples != -1); // Call prepare() first

    playHeadPosition.samples += bufferSize.samples;
    playHeadPosition.samples = playHeadPosition.samples % songLength.samples;
    playHeadPosition.calculateBeats(beatsPerSample);
}

void SongData::updateBPM(const float newBPM)
{
    jassert(sampleRate != -1); // Call prepare() first

    beatsPerMinute = newBPM;
    const float beatsPerSecond = beatsPerMinute / 60.f;
    beatsPerSample = beatsPerSecond / (float)sampleRate;
    samplesPerBeat = 1.f / beatsPerSample;

    songLength.calculateSamples(samplesPerBeat);
    playHeadPosition.calculateSamples(samplesPerBeat);
}

float SongData::sampleOffsetToBeats(const int sampleOffset) const
{
    const int positionInSamples = (playHeadPosition.samples + sampleOffset) % songLength.samples;
    return static_cast<float>(positionInSamples) * beatsPerSample;
}

int SongData::beatsToSampleOffset(const float beats) const
{
    const int samples = beatsToSamples(beats);
    if (samples >= playHeadPosition.samples)
        return samples - playHeadPosition.samples;

    // If the buffer wrapped around
    const int remainingSamples = songLength.samples - playHeadPosition.samples;
    return samples + remainingSamples;
}

int SongData::beatsToSamples(const float beats) const
{
    return (int)(beats * samplesPerBeat);
}

} // namespace xynth