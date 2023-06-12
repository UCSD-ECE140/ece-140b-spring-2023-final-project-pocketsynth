/*
  ==============================================================================

    MetronomeLane.h
    Created: 19 May 2023 4:40:00pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include "../Lane.h"
#include "../../Synth/Gain.h"

namespace xynth
{

class MetronomeLane : public Lane
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void process(juce::AudioBuffer<float>&, const juce::MidiBuffer&, const SongData&) override;

    void turnOn()  { enabled = true; }
    void turnOff() { enabled = false; }

private:
    void startNote(const SongData&);

    bool enabled = false;

    //Gain gain;
    juce::SmoothedValue<float> gain;
    juce::Random random;


};

} // namespace xynth