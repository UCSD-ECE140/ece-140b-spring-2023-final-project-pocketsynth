/*
  ==============================================================================

    ProjectsHeader.cpp
    Created: 6 Jun 2023 7:12:52pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProjectsHeader.h"

#include "../Utils/Config.h"

namespace xynth
{

ProjectsHeader::ProjectsHeader(GuiData& g) : guiData(g)
{
    addAndMakeVisible(backButton);
    backButton.setButtonText("<- Back");
    backButton.onClick = [&]()
    {
        guiData.hideProjectsMenu();
    };

    saveButton.setButtonText("Save Locally");
    saveButton.onClick = [&]() {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Please enter the name of the preset to save",
            xynth::SaveManager::getDefaultDirectory(),
            "*." + xynth::SaveManager::getFileExtension()
        );
        fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
            {
                const auto resultFile = chooser.getResult();
                if (!resultFile.getFullPathName().isEmpty())
                    guiData.audioProcessor.saveManager.saveToFile(resultFile);
            });
    };
    addAndMakeVisible(saveButton);

    loadButton.setButtonText("Load Locally");
    loadButton.onClick = [&]() {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Please select the project you want to load",
            xynth::SaveManager::getDefaultDirectory(),
            "*." + xynth::SaveManager::getFileExtension()
        );

        auto fileChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        fileChooser->launchAsync(fileChooserFlags, [&](const juce::FileChooser& chooser)
            {
                const auto resultFile = chooser.getResult();
                if (!resultFile.getFullPathName().isEmpty())
                {
                    guiData.audioProcessor.getArrangement().clearRecording();
                    guiData.audioProcessor.saveManager.loadFromFile(resultFile);
                }
            });
    };
    addAndMakeVisible(loadButton);
    
    addAndMakeVisible(uploadButton);
    uploadButton.setButtonText("Upload");
    uploadButton.onClick = [&]()
    {
        uploadProject();
    };
}

ProjectsHeader::~ProjectsHeader()
{
}

void ProjectsHeader::paint (juce::Graphics& g)
{
    const auto backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    g.fillAll(backgroundColour);
}

void ProjectsHeader::resized()
{
    const int buttonWidth = getWidth() / 6;
    juce::Rectangle<int> bounds(0, 0, buttonWidth, getHeight());

    backButton.setBounds(bounds.reduced(10));

    bounds.setX(getWidth() - buttonWidth);
    loadButton.setBounds(bounds.reduced(10));
    bounds.translate(-buttonWidth, 0);
    saveButton.setBounds(bounds.reduced(10));
    bounds.translate(-buttonWidth, 0);
    uploadButton.setBounds(bounds.reduced(10));
}

void ProjectsHeader::uploadProject()
{
    DBG("[UPLOAD] Uploading...");
    juce::StringPairArray responseHeaders;
    int statusCode = 0;

    auto url = juce::URL(Config::getURL());
    
    juce::StringPairArray headers;
    headers.set("Content-Type", "application/json");
    
    const auto currentTime = juce::Time::getCurrentTime();
    DBG(currentTime.toString(true, true));

    juce::String jsonString ="{\"name\": \"" + currentTime.toString(true, true) + "\", \"xml\": \"";
    const auto xmlData = guiData.audioProcessor.saveManager.saveStateInXml()->toString();
    jsonString += juce::JSON::escapeString(xmlData);
    jsonString += "\"}";
    
    if (auto stream = url.withPOSTData(jsonString)
        .createInputStream(juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inAddress)
            .withExtraHeaders("Content-Type: application/json; charset=utf-8")
            .withConnectionTimeoutMs(2000)
            .withResponseHeaders(&responseHeaders)
            .withStatusCode(&statusCode)))
    {
        if (statusCode == 201)
        {
            DBG("[FETCH] Successful connection, status code = " + juce::String(statusCode));

            DBG(stream->readEntireStreamAsString());
            onUpload();
        }
    }
}

} // namespace xynth
