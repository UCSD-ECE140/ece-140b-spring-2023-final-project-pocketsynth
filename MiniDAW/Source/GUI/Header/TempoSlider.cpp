/*
  ==============================================================================

    TempoSlider.cpp
    Created: 19 May 2023 3:52:29pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TempoSlider.h"

namespace xynth
{

TempoSlider::TempoSlider(xynth::GuiData& g) : guiData(g),
    attach(g.audioProcessor.treeState, BPM_PARAM_ID, *this)
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setRange(BPM_MIN, BPM_MAX, 1.0);
}

TempoSlider::~TempoSlider()
{}

void TempoSlider::paint (juce::Graphics& g)
{
    setMouseDragSensitivity(1200 * getTransform().getScaleFactor());
    auto& lnf = guiData.getLnf();

    g.setColour(lnf.findColour(juce::ResizableWindow::backgroundColourId));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.f);

    g.setColour(juce::Colours::grey);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 4.f, 1.f);

    g.setColour(juce::Colours::white);
    g.setFont(16.f);
    g.drawText(juce::String(getValue()) + " BPM", getLocalBounds(), juce::Justification::centred, false);
}

void TempoSlider::resized()
{}

void TempoSlider::mouseEnter(const juce::MouseEvent& mouseEvent)
{
    setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
    juce::Slider::mouseEnter(mouseEvent);
}

void TempoSlider::mouseExit(const juce::MouseEvent& mouseEvent)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    juce::Slider::mouseExit(mouseEvent);
}

void TempoSlider::mouseWheelMove(const juce::MouseEvent& mouseEvent, const juce::MouseWheelDetails& mouseWheel)
{
    const auto delta = std::round(mouseWheel.deltaY * 5.f);
    setValue(getValue() + delta, juce::sendNotification);
}

} // namespace xynth