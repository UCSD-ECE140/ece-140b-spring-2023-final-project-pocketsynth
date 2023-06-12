/*
  ==============================================================================

    TrackAudioMeter.cpp
    Created: 5 Jun 2023 11:40:11pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackAudioMeter.h"

namespace xynth
{

TrackAudioMeter::TrackAudioMeter(RingBuffer& rb) : ringBuffer(rb)
{
    setInterceptsMouseClicks(false, false);
    startTimerHz(60);
}

TrackAudioMeter::~TrackAudioMeter()
{
}

void TrackAudioMeter::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    g.fillRect(0, 0,               (int)(peakLeft  * 80.f), getHeight() / 2);
    g.fillRect(0, getHeight() / 2, (int)(peakRight * 80.f), getHeight() / 2);
}

void TrackAudioMeter::resized()
{
}

void TrackAudioMeter::timerCallback()
{
    peakLeft  *= 0.95f;
    peakRight *= 0.95f;
    const auto newPeaks = ringBuffer.readPeaks();
    peakLeft  = std::max(peakLeft,  newPeaks.left);
    peakRight = std::max(peakRight, newPeaks.right);
    repaint();
}

} // namespace xynth