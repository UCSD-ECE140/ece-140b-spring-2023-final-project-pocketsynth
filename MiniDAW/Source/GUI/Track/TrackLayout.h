/*
  ==============================================================================

    TrackArrangement.h
    Created: 5 Jun 2023 4:49:49pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/GuiData.h"
#include "Track.h"

namespace xynth
{

class TrackLayout : public juce::Component
{
public:
    TrackLayout(GuiData&);
    ~TrackLayout() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    GuiData& guiData;
    Track track1, track2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackLayout)
};

} // namespace xynth
