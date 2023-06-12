/*
  ==============================================================================

    GuiData.cpp
    Created: 23 Sep 2022 11:14:25am
    Author:  thesp

  ==============================================================================
*/

#include "GuiData.h"
#include "../../PluginProcessor.h"

namespace xynth
{

GuiData::GuiData(MiniDAWAudioProcessor& p, LookAndFeelStruct& _lnfStruct, juce::ApplicationProperties& props)
    : audioProcessor(p), lnfStruct(_lnfStruct), properties(props), 
    audioEvents(p.audioEvents), userEvents(p.userEvents)
{}

void GuiData::updateLnf(int skin)
{
    //if (skin == first)
    //    defaultLnf.lnf = &defaultLnf.hellLook;
    //else
    //    jassertfalse; // wrong index (too many items?)

    lnfStruct.updateLnf();

    properties.getUserSettings()->setValue("Skin", skin);
    properties.saveIfNeeded();
}

} // namespace xynth