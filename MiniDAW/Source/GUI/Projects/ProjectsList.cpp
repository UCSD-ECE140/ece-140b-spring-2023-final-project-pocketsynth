/*
  ==============================================================================

    ProjectsList.cpp
    Created: 6 Jun 2023 7:12:52pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProjectsList.h"

#include "../Utils/Config.h"

namespace xynth
{

ProjectsList::ProjectsList(GuiData& g) : guiData(g), buttonsList(10)
{
    for (auto& button : buttonsList)
        addChildComponent(button);

    addAndMakeVisible(prevButton);
    addAndMakeVisible(nextButton);
    prevButton.setButtonText("<- Newer");
    nextButton.setButtonText("Older ->");

    prevButton.onClick = [&]()
    {
        if (pageIndex > 0)
            setPageIndex(pageIndex - 1);
    };

    nextButton.onClick = [&]()
    {
        setPageIndex(pageIndex + 1);
    };
}

ProjectsList::~ProjectsList()
{
}

void ProjectsList::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    const auto backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);

    g.setColour(backgroundColour.darker(0.4f));
    const auto headerBounds = bounds.removeFromTop(60);
    const auto bottomBounds = bounds.removeFromBottom(60);
    g.fillRect(headerBounds);

    g.setColour(juce::Colours::white);
    g.setFont(20.f);
    g.drawText("Projects on the Cloud", headerBounds, juce::Justification::centred, false);
    g.drawText("Page " + juce::String(pageIndex + 1), bottomBounds, juce::Justification::centred, false);
}

void ProjectsList::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(60);

    auto leftColumn = bounds.removeFromLeft(getWidth() / 2);
    auto rightColumn = bounds;

    for (int i = 0; i < 5; i++)
        buttonsList[i].setBounds(leftColumn.removeFromTop(40));

    for (int i = 5; i < 10; i++)
        buttonsList[i].setBounds(rightColumn.removeFromTop(40));

    auto bottomBounds = getLocalBounds().removeFromBottom(60);
    prevButton.setBounds(bottomBounds.removeFromLeft (100));
    nextButton.setBounds(bottomBounds.removeFromRight(100));
}

void ProjectsList::fetchProjects()
{
    DBG("[FETCH] Fetching projects...");
    juce::StringPairArray responseHeaders;
    int statusCode = 0;

    auto url = juce::URL(Config::getURL());

    if (auto stream = url
        .createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)
            .withConnectionTimeoutMs(2000)
            .withResponseHeaders(&responseHeaders)
            .withStatusCode(&statusCode)))
    {
        if (statusCode == 200)
        {
            DBG("[FETCH] Successful connection, status code = " + juce::String(statusCode));

            auto resultJSON = juce::JSON::fromString(stream->readEntireStreamAsString());
            DBG(resultJSON.toString());

            if (resultJSON.isArray())
                updateProjectsList(*resultJSON.getArray());
            else
                DBG("[FETCH] Error reading JSON");
        }
    }
}

void ProjectsList::updateProjectsList(juce::Array<juce::var>& incomingProjectsList)
{    
    DBG("[FETCH] Projects count: " << incomingProjectsList.size());
    projectsList.resize(incomingProjectsList.size());
    
    size_t i = incomingProjectsList.size() - 1;
    for (auto& incomingProject : incomingProjectsList)
    {
        auto& project = projectsList[i--];

        project.name = incomingProject["name"].toString();
        project.xml  = incomingProject["xml"] .toString();
    }

    setPageIndex(0);
}

void ProjectsList::setPageIndex(const size_t newPageIndex)
{
    pageIndex = newPageIndex;

    for (auto& button : buttonsList)
        button.setVisible(false);

    if (projectsList.empty())
        return;

    const size_t startIndex = pageIndex * 10;
    const size_t endIndex   = std::min((pageIndex + 1) * 10, projectsList.size());

    for (size_t i = startIndex; i < endIndex; i++)
    {
        auto& project = projectsList[i];
        auto& button = buttonsList[i - startIndex];
        button.setVisible(true);
        button.setButtonText(project.name);
        button.setHelpText(project.xml);

        button.onClick = [&]()
        {
            const auto xmlElement = juce::parseXML(button.getHelpText());
            guiData.audioProcessor.saveManager.loadStateFromXml(*xmlElement);
        };
    }

    repaint();
}

} // namespace xynth
