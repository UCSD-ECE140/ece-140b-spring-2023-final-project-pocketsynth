/*
  ==============================================================================

    Storable.h
    Created: 19 May 2023 7:09:40pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace xynth
{

class Storable
{
public:
    virtual void saveState(juce::XmlElement& xmlElement) = 0;
    virtual void loadState(const juce::XmlElement& xmlElement) = 0;

};

} // namespace xynth