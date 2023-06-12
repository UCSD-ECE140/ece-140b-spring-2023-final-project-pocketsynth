/*
  ==============================================================================

    SaveManager.cpp
    Created: 22 May 2023 2:56:17pm
    Author:  Mark

  ==============================================================================
*/

#include "SaveManager.h"

#include "../PluginProcessor.h"

namespace xynth
{
SaveManager::SaveManager(MiniDAWAudioProcessor& processor)
    : audioProcessor(processor)
{
    auto defaultDirectory = getDefaultDirectory();
    if (!defaultDirectory.exists())
    {
        const auto result = defaultDirectory.createDirectory();
        if (result.failed())
        {
            DBG("Could not create preset directory: " + result.getErrorMessage());
            jassertfalse;
        }
    }
}

std::unique_ptr<juce::XmlElement> SaveManager::saveStateInXml()
{
    std::lock_guard saveLock(saveMutex);

    auto saveState = std::make_unique<juce::XmlElement>("SaveState");

    // Tree state data
    auto state = audioProcessor.treeState.copyState();
    std::unique_ptr<juce::XmlElement> xmlTreeState(state.createXml());
    saveState->addChildElement(xmlTreeState.release());

    // Save other data here
    audioProcessor.arrangement.saveState(*saveState);

    return saveState;
}

void SaveManager::loadStateFromXml(const juce::XmlElement& xmlState)
{
    if (!xmlState.hasTagName("SaveState"))
        return;

    std::lock_guard saveLock(saveMutex);

    // Tree state data
    auto& treeState = audioProcessor.treeState;
    if (auto* xmlTreeState = xmlState.getChildByName(treeState.state.getType()))
        treeState.replaceState(juce::ValueTree::fromXml(*xmlTreeState));

    // Load other data here
    audioProcessor.arrangement.loadState(xmlState);
}

void SaveManager::saveToFile(const juce::File& saveFile)
{
    jassert(!saveFile.getFullPathName().isEmpty());
    DBG("File: '" + saveFile.getFullPathName() + "'");

    auto xmlState = saveStateInXml();

    if (!saveFile.hasFileExtension(getFileExtension()))
        jassertfalse; // TODO: Change the file extension to correct type

    if (!xmlState->writeTo(saveFile))
        jassertfalse;
}

void SaveManager::loadFromFile(const juce::File& saveFile)
{
    jassert(!saveFile.getFullPathName().isEmpty());
    DBG("File: '" + saveFile.getFullPathName() + "'");

    juce::XmlDocument xmlDocument(saveFile);
    auto xmlState = xmlDocument.getDocumentElement();
    loadStateFromXml(*xmlState);
}

} // namespace xynth