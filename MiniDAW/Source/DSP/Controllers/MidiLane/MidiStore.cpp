/*
  ==============================================================================

    MidiStore.cpp
    Created: 16 May 2023 9:59:15am
    Author:  Mark

  ==============================================================================
*/

#include "MidiStore.h"

namespace xynth
{

// --- MidiEvent ---

void MidiEvent::saveState(juce::XmlElement& xmlElement, const int index) const
{
    using XmlElement = juce::XmlElement;
    XmlElement* eventElement = new XmlElement("i" + juce::String(index));

    eventElement->setAttribute("type", (int)type);
    eventElement->setAttribute("position", beatPosition);
    eventElement->setAttribute("noteNumber", noteNumber);
    eventElement->setAttribute("velocity", velocity);

    xmlElement.addChildElement(eventElement);
}

MidiEvent MidiEvent::fromXml(juce::XmlElement& xmlElement)
{
    MidiEvent midiEvent;

    midiEvent.type = (MidiEventType)xmlElement.getIntAttribute("type");
    midiEvent.beatPosition = (float)xmlElement.getDoubleAttribute("position");
    midiEvent.noteNumber = xmlElement.getIntAttribute("noteNumber");
    midiEvent.velocity   = xmlElement.getIntAttribute("velocity");

    return midiEvent;
}


// --- MidiStoreBuffer ---

void MidiStoreBuffer::addEvent(const MidiEvent& midiEvent)
{
    const auto insertIndex = findIndexOfPosition(midiEvent.beatPosition);
    noteBuffer.insert(insertIndex, midiEvent);
}

int MidiStoreBuffer::findIndexOfPosition(const float beatPosition)
{
    for (int i = 0; i < noteBuffer.size(); i++)
    {
        const auto& note = noteBuffer[i];
        if (note.beatPosition >= beatPosition)
            return i;
    }

    return noteBuffer.size();
}


// --- CurrentlyPlaying ---

CurrentlyPlaying::CurrentlyPlaying(MidiStoreBuffer& newBuffer)
    : midiStoreBuffer(newBuffer)
{
    clear();
}

void CurrentlyPlaying::noteOnEvent(const MidiEvent& midiEvent)
{
    const auto existingNoteIndex = findExistingNoteIndex(midiEvent.noteNumber);
    size_t noteIndex;
    if (existingNoteIndex == noExistingNote)
    {
        noteIndex = findEmptyNoteIndex();
        jassert(noteIndex != noExistingNote); // TODO: take care of this edge-case (no empty space for new note)
    }
    else
        noteIndex = existingNoteIndex;

    noteBuffer[noteIndex] = midiEvent;
}

void CurrentlyPlaying::noteOffEvent(const MidiEvent& midiEvent)
{
    const auto existingNoteIndex = findExistingNoteIndex(midiEvent.noteNumber);
    if (existingNoteIndex == noExistingNote)
        return;

    auto& existingNote = noteBuffer[existingNoteIndex];
    existingNote.noteNumber = emptyNoteNumber;
}

void CurrentlyPlaying::clear()
{
    for (auto& note : noteBuffer)
        note.noteNumber = emptyNoteNumber;
}

size_t CurrentlyPlaying::findExistingNoteIndex(const int noteNumber)
{
    for (size_t i = 0; i < noteBuffer.size(); i++)
        if (noteBuffer[i].noteNumber == noteNumber)
            return i;

    return noExistingNote;
}

size_t CurrentlyPlaying::findEmptyNoteIndex()
{
    return findExistingNoteIndex(emptyNoteNumber);
}


// --- MidiStore ---

void MidiStore::addEvents(const juce::MidiBuffer& inputBuffer, const SongData& songData)
{
    for (const auto metadata : inputBuffer)
        addEvent(metadata, songData);
}

void MidiStore::readEvents(juce::MidiBuffer& outputBuffer, const SongData& songData)
{
    const float startPosition = songData.getPlayHeadPosition().beats;
    const float endPosition   = songData.sampleOffsetToBeats(songData.getBufferSize().samples);
    const int startIndex = midiStoreBuffer.findIndexOfPosition(startPosition);
    const int endIndex   = midiStoreBuffer.findIndexOfPosition(endPosition);

    if (startIndex <= endIndex)
    {
        for (int i = startIndex; i < endIndex; i++)
            readEvent(outputBuffer, midiStoreBuffer[i], songData);
    }
    else
    {
        for (int i = startIndex; i < midiStoreBuffer.size(); i++)
            readEvent(outputBuffer, midiStoreBuffer[i], songData);

        for (int i = 0; i < endIndex; i++)
            readEvent(outputBuffer, midiStoreBuffer[i], songData);
    }
}

void MidiStore::startRecording()
{
    isRecording = true;
}

void MidiStore::stopRecording()
{
    isRecording = false;
    // TODO: Append noteOff events if necessary
}

void MidiStore::saveState(juce::XmlElement& xmlElement)
{
    using XmlElement = juce::XmlElement;
    XmlElement* storeElement = new XmlElement("MidiStore");

    for (int i = 0; i < midiStoreBuffer.size(); i++)
        midiStoreBuffer[i].saveState(*storeElement, i);

    xmlElement.addChildElement(storeElement);
}

void MidiStore::loadState(const juce::XmlElement& xmlElement)
{
    clear();

    auto* storeElement = xmlElement.getChildByName("MidiStore");
    if (storeElement == nullptr)
        return;

    for (auto* eventElement : storeElement->getChildIterator())
        midiStoreBuffer.addEvent(MidiEvent::fromXml(*eventElement));
}

void MidiStore::addEvent(const juce::MidiBufferIterator::reference midiEventData, const SongData& songData)
{
    const auto midiMessage = midiEventData.getMessage();
    const auto samplePosition = midiEventData.samplePosition;

    MidiEvent midiEvent;
    midiEvent.noteNumber = midiMessage.getNoteNumber();
    midiEvent.velocity = midiMessage.getVelocity();
    midiEvent.beatPosition = songData.sampleOffsetToBeats(samplePosition);
    jassert(0 <= midiEvent.noteNumber && midiEvent.noteNumber < 128);
    jassert(0 <= midiEvent.velocity && midiEvent.velocity < 128);
    jassert(0.f <= midiEvent.beatPosition && midiEvent.beatPosition <= songData.getSongLength().beats);

    if (midiMessage.isNoteOn())
    {
        midiEvent.type = MidiEvent::noteOn;
        noteOnEvent(midiEvent);
    }
    else if (midiMessage.isNoteOff())
    {
        midiEvent.type = MidiEvent::noteOff;
        noteOffEvent(midiEvent);
    }

    if (songData.isRecording == true)
        midiStoreBuffer.addEvent(midiEvent);
}

void MidiStore::noteOnEvent(const MidiEvent& midiEvent)
{
    currentlyPlaying.noteOnEvent(midiEvent);
}

void MidiStore::noteOffEvent(const MidiEvent& midiEvent)
{
    currentlyPlaying.noteOffEvent(midiEvent);
}

void MidiStore::readEvent(juce::MidiBuffer& outputBuffer, const MidiEvent& midiEvent, const SongData& songData)
{
    juce::MidiMessage midiMessage;
    if (midiEvent.type == MidiEvent::noteOn)
        midiMessage = juce::MidiMessage::noteOn (1, midiEvent.noteNumber, (juce::uint8)midiEvent.velocity);
    else
        midiMessage = juce::MidiMessage::noteOff(1, midiEvent.noteNumber, (juce::uint8)midiEvent.velocity);

    int sampleOffset = songData.beatsToSampleOffset(midiEvent.beatPosition);
    // TODO: THIS FOLLOWING LINE IS A CRAPPY TEMPORARY FIX TO A BIG ISSUE
    // Sometimes the beat position is above the max of 8?? 10 in this case
    sampleOffset = juce::jlimit(0, songData.getBufferSize().samples - 1, sampleOffset);

    jassert(0 <= midiMessage.getNoteNumber() && midiMessage.getNoteNumber() < 128);
    jassert(0 <= midiMessage.getVelocity() && midiMessage.getVelocity() < 128);
    jassert(0 <= sampleOffset && sampleOffset <= songData.getBufferSize().samples);

    outputBuffer.addEvent(midiMessage, sampleOffset);
}

} // namespace xynth
