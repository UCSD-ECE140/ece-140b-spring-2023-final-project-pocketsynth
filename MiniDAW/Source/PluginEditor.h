/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/Utils/GuiData.h"
#include "GUI/WindowLayout.h"

class MiniDAWAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MiniDAWAudioProcessorEditor (MiniDAWAudioProcessor&);
    ~MiniDAWAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::SharedResourcePointer<xynth::LookAndFeelStruct> lnfStruct;

private:
    MiniDAWAudioProcessor& audioProcessor;

    xynth::GuiData guiData;
    WindowLayout windowLayout;

    // Stores global plugin settings
    juce::ApplicationProperties properties;
    double scale{ 1.0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniDAWAudioProcessorEditor)
};
