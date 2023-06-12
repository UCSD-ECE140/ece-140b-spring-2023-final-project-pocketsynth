/*
  ==============================================================================

    RingBuffer.h
    Created: 5 Jun 2023 9:34:40pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{

// Thread safe, non-blocking ring buffer for audio, 
// useful for audio meters and more
class RingBuffer
{
public:
    struct StereoSample
    {
        float left, right;
    };

    void prepare(const juce::dsp::ProcessSpec& spec);

    // Call from audio thread to append new audio data
    void pushBack(const juce::AudioBuffer<float>& inputAudio);

    // Call from GUI thread to read new audio data
    StereoSample readPeaks();

private:
    std::array<std::vector<float>, 2> buffer;

    std::atomic<size_t> readIndex{ 0 }, writeIndex{ 0 };

};

} // namespace xynth