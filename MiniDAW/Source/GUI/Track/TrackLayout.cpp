/*
  ==============================================================================

    TrackArrangement.cpp
    Created: 5 Jun 2023 4:49:49pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackLayout.h"

namespace xynth
{

TrackLayout::TrackLayout(GuiData& g)
    : guiData(g), track1(g, 0), track2(g, 1)
{
    addAndMakeVisible(track1);
    addAndMakeVisible(track2);

    track1.updateSelectedTrack(0);
    track1.onTrackSelected = [&](const int trackIndex) {
        g.userEvents.pushBack({ UserEvent::selectTrack, trackIndex });
        track1.updateSelectedTrack(trackIndex);
        track2.updateSelectedTrack(trackIndex);
    };
    track2.onTrackSelected = track1.onTrackSelected;
}

TrackLayout::~TrackLayout()
{
}

void TrackLayout::paint(juce::Graphics& g)
{
}

void TrackLayout::resized()
{
    auto bounds = getLocalBounds();
    track1.setBounds(bounds.removeFromLeft(getWidth() / 2));
    track2.setBounds(bounds);
}

} // namespace xynth