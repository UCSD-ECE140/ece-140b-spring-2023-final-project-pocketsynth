/*
  ==============================================================================

    TempoSlider.h
    Created: 19 May 2023 3:52:29pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/GuiData.h"

namespace xynth
{

class TempoSlider : public juce::Slider
{
public:
    TempoSlider(xynth::GuiData&);
    ~TempoSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit (const juce::MouseEvent&) override;

    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;

private:
    xynth::GuiData& guiData;
    juce::AudioProcessorValueTreeState::SliderAttachment attach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TempoSlider)
};

} // namespace xynth