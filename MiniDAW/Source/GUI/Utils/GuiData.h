/*
  ==============================================================================

    GuiData.h
    Created: 5 Sep 2022 3:56:01pm
    Author:  thesp

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../CustomLooks/CustomLook.h"
#include "../../Common/Events.h"
#include "../../PluginProcessor.h"

namespace xynth
{

struct LookAndFeelStruct
{
    LookAndFeelStruct() { /*juce::LookAndFeel::setDefaultLookAndFeel(lnf);*/ }
    ~LookAndFeelStruct() { juce::LookAndFeel::setDefaultLookAndFeel(nullptr); }

    void updateLnf() { juce::LookAndFeel::setDefaultLookAndFeel(lnf); }

    juce::CustomLook customLook;

    // Change to desired LookAndFeel
    juce::CustomLook* lnf = &customLook;
};

struct GuiData
{
    GuiData(MiniDAWAudioProcessor& p, LookAndFeelStruct& defLnf, juce::ApplicationProperties& props);

    // Look and feel
    void updateLnf(int skin);
    inline juce::CustomLook& getLnf() { return *lnfStruct.lnf; }

    // References
    MiniDAWAudioProcessor& audioProcessor;
    LookAndFeelStruct& lnfStruct;
    juce::ApplicationProperties& properties;

    // Event buffers
    AudioEventBuffer& audioEvents;
    UserEventBuffer&  userEvents;

    // Callbacks
    std::function<void()> showAbout;

    // Global tooltip functions
    std::function<void(juce::Component*)> showTooltip;
    std::function<void(const juce::String& newText)> updateTooltipText;
    std::function<void()> hideTooltip;

    // Projects menu
    std::function<void()> showProjectsMenu;
    std::function<void()> hideProjectsMenu;

private:
    enum skinEnum { first };

};
} //namespace xynth