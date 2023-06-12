/*
  ==============================================================================

    SongData.h
    Created: 16 May 2023 10:34:54am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{

class SongData;

struct SampleBeatsPair
{
    int samples;
    float beats;

private:
    friend SongData;

    inline void calculateSamples(const float samplesPerBeat)
    {
        samples = static_cast<int>(beats * samplesPerBeat);
    }

    inline void calculateBeats(const float beatsPerSample)
    {
        beats = static_cast<float>(samples) * beatsPerSample;
    }
};

class SongData
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setBufferSize(juce::AudioBuffer<float>& outputAudio);
    void incrementPlayHead();
    void updateBPM(const float newBPM);

    SampleBeatsPair getPlayHeadPosition() const { return playHeadPosition; }
    SampleBeatsPair getSongLength() const { return songLength; }
    SampleBeatsPair getBufferSize() const { return bufferSize; }

    // Converts sample offset in relation to current buffer to
    // play head position in beats.
    float sampleOffsetToBeats(const int sampleOffset = 0) const;
    int beatsToSampleOffset(const float beats) const;
    int beatsToSamples(const float beats) const;

    float getBPM() const { return beatsPerMinute; }
    float getBeatsPerSample() const { return beatsPerSample; }
    float getSamplesPerBeat() const { return samplesPerBeat; }
    float getSampleRate() const { return sampleRate; }

    bool isPlaying = false;
    bool isRecording = false;

protected:
    SampleBeatsPair playHeadPosition{ 0, 0.f };
    SampleBeatsPair songLength{ -1, 4.f * 4.f };
    SampleBeatsPair bufferSize{ -1 };

    float beatsPerMinute = 120.f;
    float beatsPerSample;
    float samplesPerBeat;

    int sampleRate = -1;

};

} // namespace xynth