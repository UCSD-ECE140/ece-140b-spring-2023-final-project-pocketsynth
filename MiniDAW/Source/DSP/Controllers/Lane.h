/*
  ==============================================================================

    Lane.h
    Created: 11 May 2023 10:38:32am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Synth/SynthProcessor.h"
#include "SongData.h"

namespace xynth
{

class Lane
{
public:
    virtual ~Lane() {}

    virtual void prepare(const juce::dsp::ProcessSpec& spec) = 0;
    virtual void process(juce::AudioBuffer<float>&, const juce::MidiBuffer&, const SongData&) = 0;
};

} // namespace xynth