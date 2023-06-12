/*
  ==============================================================================

    MidiLaneTests.h
    Created: 23 May 2023 10:30:15am
    Author:  Mark

  ==============================================================================
*/
#ifdef UNIT_TESTS

#pragma once

#include <JuceHeader.h>
#include "../Controllers/MidiLane/MidiLane.h"

namespace xynth
{

class MidiStoreTests : public juce::UnitTest
{
public:
    MidiStoreTests();

    void runTest() override;

private:
    void prepare();

    bool runTest1();

    MidiStore midiStore;
    SongData songData;
    juce::AudioBuffer<float> audioBuffer;
    juce::MidiBuffer midiBuffer;

};

} // namespace xynth

static xynth::MidiStoreTests MidiStoreTests;

#endif