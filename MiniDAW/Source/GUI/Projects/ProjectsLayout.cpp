/*
  ==============================================================================

    ProjectsLayout.cpp
    Created: 6 Jun 2023 7:12:52pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProjectsLayout.h"

namespace xynth
{

ProjectsLayout::ProjectsLayout(GuiData& g) : guiData(g), projectsHeader(g), projectsList(g)
{
    addAndMakeVisible(projectsHeader);
    addAndMakeVisible(projectsList);
    
    projectsHeader.onUpload = [&]()
    {
        projectsList.fetchProjects();
    };
}

ProjectsLayout::~ProjectsLayout()
{
}

void ProjectsLayout::paint (juce::Graphics& g)
{
}

void ProjectsLayout::resized()
{
    auto bounds = getLocalBounds();
    projectsHeader.setBounds(bounds.removeFromTop(80));
    projectsList.setBounds(bounds);
}

void ProjectsLayout::fetchProjects()
{
    projectsList.fetchProjects();
}

} // namespace xynth
