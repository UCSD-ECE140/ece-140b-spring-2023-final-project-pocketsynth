/*
  ==============================================================================

    SynthProcessor.cpp
    Created: 15 Oct 2022 9:32:45am
    Author:  thesp

  ==============================================================================
*/

#include "SynthProcessor.h"

namespace xynth
{
void SynthProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    for (int i = 0; i < synthVoiceArray.size(); i++)
        synthVoiceArray[i].prepare(spec);
}

void SynthProcessor::process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& midiData)
{
    jassert(sampleRate != 0.0);

    juce::MidiBuffer::Iterator it(midiData);
    juce::MidiMessage currentMessage;
    int samplePos, prevSamplePos = 0;

    while (it.getNextEvent(currentMessage, samplePos))
    {
        const int samplesBetweenMessages = samplePos - prevSamplePos;
        renderSynthVoices(outputAudio, prevSamplePos, samplesBetweenMessages);

        prevSamplePos = samplePos;
        handleMidiEvent(currentMessage);
    }

    // Finishes rendering the block of audio
    const int samplesBetweenMessages = outputAudio.getNumSamples() - prevSamplePos;
    renderSynthVoices(outputAudio, prevSamplePos, samplesBetweenMessages);
}

void SynthProcessor::reset()
{
    for (auto& synthVoice : synthVoiceArray)
        synthVoice.stopNote(0.f, false);
}

void SynthProcessor::setWavetableSound(const int soundIndex)
{
    if (soundIndex == previousSoundIndex)
        return;

    for (auto& synthVoice : synthVoiceArray)
        synthVoice.setSound(soundIndex);

    previousSoundIndex = soundIndex;
}

void SynthProcessor::setPluckAmount(const float pluckAmount)
{
    for (auto& synthVoice : synthVoiceArray)
        synthVoice.setPluckAmount(pluckAmount);
}

void SynthProcessor::setReleaseAmount(float releaseAmount)
{
    juce::NormalisableRange<float> range(0.f, 1.f);
    range.setSkewForCentre(0.25f);

    releaseAmount = 0.002 + range.convertFrom0to1(releaseAmount / 100.f);
    for (auto& synthVoice : synthVoiceArray)
        synthVoice.setRelease(releaseAmount);
}

void SynthProcessor::renderSynthVoices(juce::AudioBuffer<float>& outputAudio, int prevSamplePos, int numSamples)
{
    for (int i = 0; i < synthVoiceArray.size(); i++)
        synthVoiceArray[i].renderNextBlock(outputAudio, prevSamplePos, numSamples);
}

void SynthProcessor::handleMidiEvent(const juce::MidiMessage& midiMessage)
{
    if (midiMessage.isNoteOn())
        noteOn(midiMessage);
    else if (midiMessage.isNoteOff())
        noteOff(midiMessage);

    //synthVoiceArrayDBG();
}

void SynthProcessor::noteOn(const juce::MidiMessage& midiMessage)
{
    const int midiNoteNumber = midiMessage.getNoteNumber();
    const float velocity = float(midiMessage.getVelocity()) / 127.f;
    const int pitchWheelPosition = 0;

    SynthVoice* synthVoice = nullptr;

    // Make sure note does not already exist
    const int existingNoteIdx = findExistingNoteIndex(midiNoteNumber);
    if (existingNoteIdx == -1)
    {
        // Make sure there is an empty space for the new note
        const int emptyNoteIdx = findEmptyNoteIndex();
        if (emptyNoteIdx != -1)
        {
            synthVoice = &synthVoiceArray[emptyNoteIdx];
            synthVoice->setQueuePos(newQueuePos);
            newQueuePos++;
        }
        // If there is no space...
        else
        {
            const int oldestIdx = findOldestNoteIndex();

            // Replace oldest note with new note
            synthVoice = &synthVoiceArray[oldestIdx];
            synthVoice->setQueuePos(newQueuePos);

            updateNoteQueue();
        }
    }
    else // If an existing note is activated again (somehow)
    {
        synthVoice = &synthVoiceArray[existingNoteIdx];

        updateNoteQueue();
    }

    jassert(synthVoice);
    synthVoice->startNote(midiNoteNumber, velocity, pitchWheelPosition);
}

void SynthProcessor::noteOff(const juce::MidiMessage& midiMessage)
{
    const float velocity = float(midiMessage.getVelocity()) / 127.f;
    const int existingNote = findExistingNoteIndex(midiMessage.getNoteNumber());
    if (existingNote != -1)
    {
        // Turn off note in array
        synthVoiceArray[existingNote].stopNote(velocity, true);
        updateNoteQueue();
    }
}

int SynthProcessor::findExistingNoteIndex(const int midiNoteNumber)
{
    for (int i = 0; i < synthVoiceArray.size(); ++i)
    {
        auto& voice = synthVoiceArray[i];
        if (voice.isCurrentlyPlaying())
            if (voice.getMidiNoteNumber() == midiNoteNumber)
                return i;
    }

    return -1;
}

int SynthProcessor::findEmptyNoteIndex()
{
    for (int i = 0; i < synthVoiceArray.size(); ++i)
        if (synthVoiceArray[i].isCurrentlyPlaying() == false)
            return i;

    return -1;
}

int SynthProcessor::findOldestNoteIndex(const int minQueuePos)
{
    int oldestIndex = -1;
    int smallestQueue = 99999;

    for (int i = 0; i < synthVoiceArray.size(); ++i)
    {
        auto& note = synthVoiceArray[i];
        if (note.isCurrentlyPlaying())
        {
            const int queuePos = note.getQueuePos();
            if (queuePos < smallestQueue && queuePos >= minQueuePos)
            {
                oldestIndex = i;
                smallestQueue = queuePos;
            }
        }
    }

    jassert(oldestIndex != -1);
    return oldestIndex;
}

void SynthProcessor::updateNoteQueue()
{
    // Sum up total number of notes
    int numNotes = 0;
    for (auto& note : synthVoiceArray)
         numNotes += (int)note.isCurrentlyPlaying();

    newQueuePos = numNotes;

    // Remove any 'queue gaps'
    for (int i = 0; i < numNotes; ++i)
    {
        // Find oldest note (note with lowest queuePos)
        int oldestIndex = findOldestNoteIndex(i);
        synthVoiceArray[oldestIndex].setQueuePos(i);
    }
}

void SynthProcessor::synthVoiceArrayDBG()
{
    juce::String out = "";

    for (auto& note : synthVoiceArray)
    {
        int midiNum = note.getMidiNoteNumber();
        juce::String noteName = midiNum >= 10 ? juce::String(midiNum) : juce::String(midiNum) + " ";
        out += juce::String(note.isCurrentlyPlaying() ? "On" : "  ") + "-" + noteName + ", ";
    }

    DBG(out);
}
} // namespace xynth
