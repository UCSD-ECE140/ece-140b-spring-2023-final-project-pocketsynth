/*
  ==============================================================================

    ProjectsLayout.h
    Created: 6 Jun 2023 7:12:52pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Utils/GuiData.h"
#include "ProjectsHeader.h"
#include "ProjectsList.h"

namespace xynth
{

class ProjectsLayout  : public juce::Component
{
public:
    ProjectsLayout(GuiData&);
    ~ProjectsLayout() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void fetchProjects();

private:
    GuiData& guiData;

    ProjectsHeader projectsHeader;
    ProjectsList projectsList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectsLayout)
};

} // namespace xynth