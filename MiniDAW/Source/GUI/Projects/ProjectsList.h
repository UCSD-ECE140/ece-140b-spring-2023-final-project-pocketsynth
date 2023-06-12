/*
  ==============================================================================

    ProjectsList.h
    Created: 6 Jun 2023 7:12:52pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Utils/GuiData.h"

namespace xynth
{

class ProjectsList : public juce::Component
{
public:
    ProjectsList(GuiData&);
    ~ProjectsList() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void fetchProjects();

private:
    struct Project
    {
        juce::String name, xml;
    };

    void updateProjectsList(juce::Array<juce::var>& projectsList);
    void setPageIndex(const size_t newPageIndex);

    GuiData& guiData;

    juce::TextButton prevButton, nextButton;

    std::vector<juce::TextButton> buttonsList;
    std::vector<Project> projectsList;
    size_t pageIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectsList)
};

} // namespace xynth
