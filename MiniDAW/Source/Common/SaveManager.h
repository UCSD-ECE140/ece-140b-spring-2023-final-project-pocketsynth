/*
  ==============================================================================

    SaveManager.h
    Created: 22 May 2023 2:56:17pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MiniDAWAudioProcessor;

namespace xynth
{

class SaveManager
{
public:
    SaveManager(MiniDAWAudioProcessor& processor);

    // Creates XML save data
    std::unique_ptr<juce::XmlElement> saveStateInXml();

    // Loads from XML save data
    void loadStateFromXml(const juce::XmlElement& xmlState);

    void saveToFile  (const juce::File& saveFile);
    void loadFromFile(const juce::File& saveFile);

    // Get the default directory (Documents/Xynth Audio/MiniDAW)
    static juce::File getDefaultDirectory() 
    {
        using File = juce::File;
        static File defaultDirectory(
            File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
            .getChildFile(ProjectInfo::companyName)
            .getChildFile(ProjectInfo::projectName));
        return defaultDirectory;
    }

    static std::string getFileExtension()
    {
        return "mdaw";
    }

private:
    MiniDAWAudioProcessor& audioProcessor;

    std::mutex saveMutex;

};

} // namespace xynth