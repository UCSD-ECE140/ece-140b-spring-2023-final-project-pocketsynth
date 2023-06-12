/*
  ==============================================================================

    MetronomeLane.cpp
    Created: 19 May 2023 4:40:00pm
    Author:  Mark

  ==============================================================================
*/

#include "MetronomeLane.h"


namespace xynth
{

void MetronomeLane::prepare(const juce::dsp::ProcessSpec& spec)
{
    gain.reset(spec.sampleRate, 0.1);
    gain.setCurrentAndTargetValue(0.f);
}

void MetronomeLane::process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer&, const SongData& songData)
{
    if (enabled == false)
        return;

    auto left  = outputAudio.getWritePointer(0);
    auto right = outputAudio.getWritePointer(1);

    const auto position = songData.getPlayHeadPosition();

    // Calculate the first sample of the next beat
    const int nextBeatPosition = (int)(std::ceil(position.beats) * songData.getSamplesPerBeat());

    const int numSamples = songData.getBufferSize().samples;

    for (int i = 0; i < numSamples; i++)
    {
        const int sampleIndex = position.samples + i;
        if (nextBeatPosition == sampleIndex)
            startNote(songData);

        const float gainValue = gain.getNextValue();
        if (gainValue == 0.f)
            continue;

        const float sample = gainValue * random.nextFloat() * 0.24f;
        left [i] += sample;
        right[i] += sample;
    }
}

void MetronomeLane::startNote(const SongData& songData)
{
    const int beatPosition = (int)std::round(songData.getPlayHeadPosition().beats);
    if (beatPosition % 4 == 0)
        gain.reset((double)songData.getSampleRate(), 0.08);
    else
        gain.reset((double)songData.getSampleRate(), 0.02);

    gain.setCurrentAndTargetValue(1.f);
    gain.setTargetValue(0.f);
}

} // namespace xynth