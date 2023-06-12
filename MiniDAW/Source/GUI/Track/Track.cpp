/*
  ==============================================================================

    Track.cpp
    Created: 5 Jun 2023 3:56:34pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Track.h"

namespace xynth
{

Track::Track(GuiData& g, const int index) 
    : guiData(g), audioMeter(g.audioProcessor.getLaneRingBuffer(index)), trackIndex(index),
      soundSelect(g, index)
{
    addAndMakeVisible(audioMeter);
    addAndMakeVisible(gainSlider.slider);
    addAndMakeVisible(pluckSlider.slider);
    addAndMakeVisible(releaseSlider.slider);
    addAndMakeVisible(reverbSlider.slider);
    addAndMakeVisible(soundSelect);

    gainSlider   .init(g.audioProcessor.treeState, getLaneParameter(LANE_VOLUME_ID,  trackIndex), "Volume");
    pluckSlider  .init(g.audioProcessor.treeState, getLaneParameter(LANE_PLUCK_ID,   trackIndex), "Pluck");
    releaseSlider.init(g.audioProcessor.treeState, getLaneParameter(LANE_RELEASE_ID, trackIndex), "Release");
    reverbSlider .init(g.audioProcessor.treeState, getLaneParameter(LANE_REVERB_ID,  trackIndex), "Reverb");
}

Track::~Track()
{
}

void Track::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    const auto backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);

    g.setColour(backgroundColour.darker(isSelected ? 0.8f : 0.2f));
    const auto headerBounds = bounds.removeFromTop(60);
    g.fillRect(headerBounds);

    g.setColour(backgroundColour.darker(0.2f));
    g.drawVerticalLine(trackIndex == 0 ? getWidth() - 1 : 0, 0, getBottom());

    g.setColour(juce::Colours::white.withAlpha(isSelected ? 1.f : 0.6f));
    g.setFont(20.f);
    g.drawText("Track " + juce::String(trackIndex + 1), headerBounds.reduced(20, 0),
                juce::Justification::centredLeft, false);
}

void Track::resized()
{
    auto bounds = getLocalBounds();
    auto headerBounds = bounds.removeFromTop(60).withTrimmedLeft(150);
    audioMeter.setBounds(headerBounds.reduced(10));

    bounds.removeFromTop(32);
    const int buttonWidth = 100;
    auto topParameters = bounds.removeFromTop(100);
    auto bottomParameters = bounds.removeFromBottom(100);
    gainSlider .slider.setBounds(topParameters.removeFromLeft(buttonWidth).reduced(10, 0));
    pluckSlider.slider.setBounds(topParameters.removeFromLeft(buttonWidth).reduced(10, 0));

    soundSelect.setBounds(topParameters.reduced(10, 0).withHeight(40));

    releaseSlider.slider.setBounds(bottomParameters.removeFromLeft(buttonWidth).reduced(10, 0));
    reverbSlider .slider.setBounds(bottomParameters.removeFromLeft(buttonWidth).reduced(10, 0));
}

void Track::updateSelectedTrack(const int selectedTrackIndex)
{
    isSelected = trackIndex == selectedTrackIndex;
    repaint();
}

void Track::mouseDown(const juce::MouseEvent& mouseEvent)
{
    if (mouseEvent.y > 60)
        return;

    if (onTrackSelected)
        onTrackSelected(trackIndex);

    mouseDownTime = juce::Time::getCurrentTime();
}

void Track::mouseUp(const juce::MouseEvent& mouseEvent)
{
    if (mouseDownTime.toMilliseconds() == 0 || mouseEvent.y > 60)
        return;

    const auto timeDelta = juce::Time::getCurrentTime() - mouseDownTime;

    if (timeDelta.inMilliseconds() >= 1000)
    {
        DBG("Clearing lane " << trackIndex);
        guiData.userEvents.pushBack({ UserEvent::clearRecording, trackIndex });
    }

    mouseDownTime = juce::Time();
}

} // namespace xynth