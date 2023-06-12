/*
  ==============================================================================

    Header.h
    Created: 5 Jun 2023 3:23:59pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/GuiData.h"
#include "../Utils/TooltipSlider.h"
#include "TempoSlider.h"

namespace xynth
{

class Header  : public juce::Component
{
public:
    Header(GuiData&);
    ~Header() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GuiData& guiData;
    juce::TextButton recordButton, clearButton, metronomeButton, projectsButton;
    TempoSlider tempoSlider;
    TooltipSlider gainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};

} // namespace xynth