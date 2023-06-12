/*
  ==============================================================================

    TrackAudioMeter.h
    Created: 5 Jun 2023 11:40:11pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Common/RingBuffer.h"

namespace xynth
{

class TrackAudioMeter : public juce::Component, public juce::Timer
{
public:
    TrackAudioMeter(RingBuffer&);
    ~TrackAudioMeter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback();

private:
    RingBuffer& ringBuffer;
    float peakLeft = 0.f, peakRight = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackAudioMeter)
};

} // namespace xynth