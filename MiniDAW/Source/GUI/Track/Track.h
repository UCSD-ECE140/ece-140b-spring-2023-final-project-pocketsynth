/*
  ==============================================================================

    Track.h
    Created: 5 Jun 2023 3:56:34pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/GuiData.h"
#include "../Utils/TooltipSlider.h"
#include "../Utils/FullSlider.h"
#include "TrackAudioMeter.h"
#include "SoundSelect.h"

namespace xynth
{

class Track : public juce::Component
{
public:
    Track(GuiData&, const int newTrackIndex);
    ~Track() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getTrackIndex() const { return trackIndex; }
    void updateSelectedTrack(const int selectedTrackIndex);

    void mouseDown(const juce::MouseEvent& mouseEvent) override;
    void mouseUp(const juce::MouseEvent& mouseEvent) override;

    std::function<void(const int)> onTrackSelected;

private:
    GuiData& guiData;
    TrackAudioMeter audioMeter;
    FullSlider gainSlider, pluckSlider, releaseSlider, reverbSlider;
    SoundSelect soundSelect;

    int trackIndex = -1;
    bool isSelected = false;

    juce::Time mouseDownTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Track)
};

} // namespace xynth