/*
  ==============================================================================

    WindowLayout.cpp
    Created: 5 Sep 2022 3:26:44pm
    Author:  thesp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WindowLayout.h"

WindowLayout::WindowLayout(xynth::GuiData& g) 
    : guiData(g), tooltip(g), header(g), trackLayout(g), projectsLayout(g)
{
    addAndMakeVisible(tooltip);
    addAndMakeVisible(header);
    addAndMakeVisible(trackLayout);
    addChildComponent(projectsLayout);

    // --Tooltip--
    guiData.showTooltip = [&](juce::Component* component)
    {
        auto componentPosition = component->getScreenPosition();
        auto relativePosition = componentPosition - getScreenPosition();
        const auto scale = getTransform().getScaleFactor();
        relativePosition /= scale;
        relativePosition.addXY(component->getWidth() / 2, component->getHeight() + 2);
        tooltip.showTooltip(relativePosition);
    };

    guiData.updateTooltipText = [&](const juce::String& newText)
    {
        tooltip.updateTooltipText(newText);
    };

    guiData.hideTooltip = [&]()
    {
        tooltip.hideTooltip();
    };

    // --Projects Menu--
    guiData.showProjectsMenu = [&]()
    {
        header.setVisible(false);
        trackLayout.setVisible(false);
        projectsLayout.setVisible(true);
        projectsLayout.fetchProjects();
    };

    guiData.hideProjectsMenu = [&]()
    {
        header.setVisible(true);
        trackLayout.setVisible(true);
        projectsLayout.setVisible(false);
    };
}

WindowLayout::~WindowLayout()
{}

void WindowLayout::paint (juce::Graphics& g)
{
}

void WindowLayout::resized()
{
    auto bounds = getLocalBounds();
    header.setBounds(bounds.removeFromTop(80));
    trackLayout.setBounds(bounds);

    projectsLayout.setBounds(getLocalBounds());
}
