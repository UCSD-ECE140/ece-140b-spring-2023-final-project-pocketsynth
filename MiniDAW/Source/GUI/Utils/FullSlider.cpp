/*
  ==============================================================================

    FullSlider.cpp
    Created: 31 Aug 2022 10:32:20am
    Author:  thesp

  ==============================================================================
*/

#include "FullSlider.h"

namespace xynth
{
FullSlider::FullSlider()
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    slider.setTextBoxIsEditable(true);
}

void FullSlider::init(juce::AudioProcessorValueTreeState& treeState, juce::String paramID, juce::String paramName)
{
    attach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, paramID, slider);

    if (paramName != "")
    {
        label.setText(paramName, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.attachToComponent(&slider, false);
    }
}
} // namespace xynth