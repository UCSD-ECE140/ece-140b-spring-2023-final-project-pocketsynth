/*
  ==============================================================================

    MidiLaneTests.cpp
    Created: 23 May 2023 10:30:15am
    Author:  Mark

  ==============================================================================
*/

#ifdef UNIT_TESTS

#include "MidiStoreTests.h"

namespace xynth
{

MidiStoreTests::MidiStoreTests() : juce::UnitTest("MidiStoreTests")
{
}

void MidiStoreTests::runTest()
{
    beginTest("Part 1");

    prepare();
    expect(runTest1(), "Test 1");
    
}

void MidiStoreTests::prepare()
{
    using uint32 = juce::uint32;
    juce::dsp::ProcessSpec spec = { 44100.0, (uint32)256, (uint32)2 };

    audioBuffer.setSize(2, spec.maximumBlockSize);
    songData.prepare(spec);
    songData.isRecording = true;
    songData.setBufferSize(audioBuffer);
}

bool MidiStoreTests::runTest1()
{
    auto noteOn  = juce::MidiMessage::noteOn (1, 48, (juce::uint8)120);
    auto noteOff = juce::MidiMessage::noteOff(1, 48, (juce::uint8)120);
    for (int i = 0; i < 5; i++)
    {
        midiBuffer.addEvent(noteOn, 0);
        midiBuffer.addEvent(noteOff, 20);

        midiBuffer.addEvent(noteOn, audioBuffer.getNumSamples() - 20);
        midiBuffer.addEvent(noteOff, audioBuffer.getNumSamples() - 1);

        midiStore.addEvents(midiBuffer, songData);

        songData.incrementPlayHead();
    }

    juce::XmlElement xml("MidiStoreTest");
    midiStore.saveState(xml);
    DBG(xml.toString());

    return true;
}

} // namespace xynth

#endif