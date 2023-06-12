/*
  ==============================================================================

    WindowLayout.h
    Created: 5 Sep 2022 3:26:44pm
    Author:  thesp

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Common/Constants.h"
#include "Utils/GuiData.h"
#include "Header/Header.h"
#include "Track/TrackLayout.h"
#include "Utils/TooltipLabel.h"
#include "Projects/ProjectsLayout.h"

class WindowLayout : public juce::Component
{
public:
    WindowLayout(xynth::GuiData&);
    ~WindowLayout() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    xynth::GuiData& guiData;
    xynth::TooltipLabel tooltip;

    xynth::Header header;
    xynth::TrackLayout trackLayout;

    xynth::ProjectsLayout projectsLayout;

    // File saving
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowLayout)
};
