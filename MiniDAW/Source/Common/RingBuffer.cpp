/*
  ==============================================================================

    RingBuffer.cpp
    Created: 5 Jun 2023 9:34:40pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RingBuffer.h"

namespace xynth
{

void RingBuffer::prepare(const juce::dsp::ProcessSpec& spec)
{
    for (auto& channel : buffer)
        channel.resize((size_t)spec.sampleRate * 2);
}

void RingBuffer::pushBack(const juce::AudioBuffer<float>& inputAudio)
{
    jassert(inputAudio.getNumChannels() == buffer.size());

    const size_t bufferSize = buffer[0].size();
    const size_t currentIndex = writeIndex.load(std::memory_order_acquire);
    const auto numSamples = inputAudio.getNumSamples();

    for (int ch = 0; ch < buffer.size(); ch++)
    {
        for (int i = 0; i < numSamples; i++)
        {
            size_t bufferIndex = (i + currentIndex) % bufferSize;
            buffer[ch][bufferIndex] = inputAudio.getReadPointer(ch)[i];
        }
    }

    writeIndex.store((currentIndex + numSamples) % bufferSize, std::memory_order_release);
}

RingBuffer::StereoSample RingBuffer::readPeaks()
{
    StereoSample output{ 0.f, 0.f };
    const auto start = readIndex .load(std::memory_order_acquire);
    const auto end   = writeIndex.load(std::memory_order_acquire);
    
    for (size_t i = start; i < end; i++)
    {
        output.left  = std::max(output.left,  std::abs(buffer[0][i]));
        output.right = std::max(output.right, std::abs(buffer[1][i]));
    }

    readIndex.store(end, std::memory_order_release);
    return output;
}

} // namespace xynth