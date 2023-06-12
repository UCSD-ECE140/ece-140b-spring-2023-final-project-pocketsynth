/*
  ==============================================================================

    TooltipLabel.cpp
    Created: 25 Dec 2022 2:03:01pm
    Author:  thesp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TooltipLabel.h"

namespace xynth
{
TooltipLabel::TooltipLabel(xynth::GuiData& g) : guiData(g)
{
    setAlwaysOnTop(true);
}

void TooltipLabel::paint(juce::Graphics& g)
{
    auto& lnf = guiData.getLnf();

    auto bgRect = getLocalBounds().toFloat();
    const auto backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    g.setColour(backgroundColour.darker());
    g.fillRoundedRectangle(bgRect, 4.f);

    g.setColour(juce::Colours::white);
    g.setFont(18.f);
    g.drawText(tooltipText, getLocalBounds(), juce::Justification::centred, false);

    g.setColour(juce::Colours::grey);
    g.drawRoundedRectangle(bgRect.reduced(0.5f), 4.f, 1.f);
}

void TooltipLabel::showTooltip(juce::Point<int> screenPosition)
{
    setVisible(true);

    juce::Rectangle<int> rect(screenPosition, screenPosition);
    rect.expand(58, 0);
    rect.setHeight(32);
    setBounds(rect);
    repaint();
}

void TooltipLabel::updateTooltipText(const juce::String& newText)
{
    tooltipText = newText;
    repaint();
}

void TooltipLabel::hideTooltip()
{
    setVisible(false);
    repaint();
}
} // namespace xynth