/*
  ==============================================================================

    MidiStore.h
    Created: 16 May 2023 9:59:15am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../SongData.h"
#include "../Storable.h"

namespace xynth
{

struct MidiEvent
{
    enum MidiEventType : char
    {
        noteOff, noteOn
    };

    void saveState(juce::XmlElement& xmlElement, const int index) const;
    static MidiEvent fromXml(juce::XmlElement& xmlElement);

    MidiEventType type;
    float beatPosition;
    int noteNumber, velocity;
};

// Long-term storage for recorded notes
class MidiStoreBuffer
{
public:
    void addEvent(const MidiEvent& midiEvent);
    void clear() { noteBuffer.clearQuick(); }

    int size() { return noteBuffer.size(); }
    const MidiEvent& operator[](const int index) const { return noteBuffer.getReference(index); }

    int findIndexOfPosition(const float beatPosition);

private:
    juce::Array<MidiEvent> noteBuffer;

};

// Temporarily store currently playing notes
class CurrentlyPlaying
{
public:
    CurrentlyPlaying(MidiStoreBuffer& newBuffer);

    void noteOnEvent (const MidiEvent& midiEvent);
    void noteOffEvent(const MidiEvent& midiEvent);

    void clear();

private:
    size_t findExistingNoteIndex(const int noteNumber);
    size_t findEmptyNoteIndex();

    MidiStoreBuffer& midiStoreBuffer;

    std::array<MidiEvent, 20> noteBuffer;
    static constexpr size_t emptyNoteNumber = -1;
    static constexpr size_t noExistingNote = -1;

};

#ifdef UNIT_TESTS
class MidiLaneTests;
#endif

// Storage and processing of Midi events
class MidiStore : public Storable
{
public:
    MidiStore() : currentlyPlaying(midiStoreBuffer) {}

    void addEvents(const juce::MidiBuffer& inputBuffer, const SongData& songData);
    void readEvents(juce::MidiBuffer& outputBuffer, const SongData& songData);

    void clear() { midiStoreBuffer.clear(); currentlyPlaying.clear(); }

    void startRecording();
    void stopRecording();

    void saveState(juce::XmlElement& xmlElement) override;
    void loadState(const juce::XmlElement& xmlElement) override;

protected:
    void addEvent(const juce::MidiBufferIterator::reference midiEventData, const SongData& songData);
    void noteOnEvent (const MidiEvent& midiEvent);
    void noteOffEvent(const MidiEvent& midiEvent);

    void readEvent(juce::MidiBuffer& outputBuffer, const MidiEvent& midiEvent, const SongData& songData);

    bool isRecording = false;

    // Long-term storage of midi notes
    MidiStoreBuffer midiStoreBuffer;

    // Temporarily store midi notes as they are coming in
    CurrentlyPlaying currentlyPlaying;

#ifdef UNIT_TESTS
    friend MidiLaneTests;
#endif

};

} // namespace xynth
