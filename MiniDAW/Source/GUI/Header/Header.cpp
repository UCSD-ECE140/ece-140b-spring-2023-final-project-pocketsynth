/*
  ==============================================================================

    Header.cpp
    Created: 5 Jun 2023 3:23:59pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Header.h"

namespace xynth
{

Header::Header(GuiData& g) : guiData(g), tempoSlider(g), gainSlider(g)
{
    auto& treeState = g.audioProcessor.treeState;

    recordButton.setButtonText("Record");
    recordButton.setClickingTogglesState(true);
    recordButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::firebrick.darker());
    recordButton.onClick = [&]() {
        const bool state = recordButton.getToggleState();
        guiData.userEvents.pushBack(state ? xynth::UserEvent::startRecord : xynth::UserEvent::stopRecord);
    };
    addAndMakeVisible(recordButton);

    clearButton.setButtonText("Clear");
    clearButton.onClick = [&]() {
        guiData.userEvents.pushBack({ UserEvent::clearRecording, -1 });
    };
    addAndMakeVisible(clearButton);

    metronomeButton.setButtonText("Metronome");
    metronomeButton.setClickingTogglesState(true);
    metronomeButton.onClick = [&]() {
        const bool state = metronomeButton.getToggleState();
        guiData.userEvents.pushBack(state ? xynth::UserEvent::metronomeOn : xynth::UserEvent::metronomeOff);
    };
    addAndMakeVisible(metronomeButton);

    addAndMakeVisible(tempoSlider);

    gainSlider.assignParameter(GLOBAL_VOLUME_ID);
    gainSlider.setName("Gain");
    gainSlider.setPostfix("dB");
    addAndMakeVisible(gainSlider);

    addAndMakeVisible(projectsButton);
    projectsButton.setButtonText("Projects");
    projectsButton.onClick = [&]()
    {
        guiData.showProjectsMenu();
    };
}

Header::~Header()
{
}

void Header::paint (juce::Graphics& g)
{
    const auto backgroundColour = getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId);
    g.fillAll(backgroundColour);
}

void Header::resized()
{
    const int buttonWidth = getWidth() / 6;
    juce::Rectangle<int> bounds(0, 0, buttonWidth, getHeight());

    projectsButton.setBounds(bounds.reduced(10));
    bounds.translate(buttonWidth, 0);

    recordButton.setBounds(bounds.reduced(10));
    bounds.translate(buttonWidth, 0);
    clearButton.setBounds(bounds.reduced(10));
    bounds.translate(buttonWidth, 0);
    tempoSlider.setBounds(bounds.reduced(10));
    bounds.translate(buttonWidth, 0);
    metronomeButton.setBounds(bounds.reduced(10));
    bounds.translate(buttonWidth, 0);

    gainSlider.setBounds(bounds.withTrimmedTop(4));
}

} // namespace xynth