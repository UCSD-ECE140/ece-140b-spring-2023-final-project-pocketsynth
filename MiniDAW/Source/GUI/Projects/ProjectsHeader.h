/*
  ==============================================================================

    ProjectsHeader.h
    Created: 6 Jun 2023 7:12:52pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Utils/GuiData.h"

namespace xynth
{

class ProjectsHeader : public juce::Component
{
public:
    ProjectsHeader(GuiData&);
    ~ProjectsHeader() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    std::function<void()> onUpload;

private:
    void uploadProject();
    
    GuiData& guiData;

    juce::TextButton backButton, saveButton, loadButton, uploadButton;
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectsHeader)
};

} // namespace xynth
