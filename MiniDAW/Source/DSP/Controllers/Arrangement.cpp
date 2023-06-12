/*
  ==============================================================================

    Arrangement.cpp
    Created: 15 May 2023 10:16:58am
    Author:  Mark

  ==============================================================================
*/

#include "Arrangement.h"

namespace xynth
{
Arrangement::Arrangement(UserEventDispatcher& eventDispatcher, juce::AudioProcessorValueTreeState& treeState)
{
    eventDispatcher.addCallback(UserEvent::startRecord, 
        [&](const MessageType&) { songData.isRecording = true; });

    eventDispatcher.addCallback(UserEvent::stopRecord,
        [&](const MessageType&) { songData.isRecording = false; });

    eventDispatcher.addCallback(UserEvent::metronomeOn,
        [&](const MessageType&) { metronomeLane.turnOn(); });

    eventDispatcher.addCallback(UserEvent::metronomeOff,
        [&](const MessageType&) { metronomeLane.turnOff(); });

    eventDispatcher.addCallback(UserEvent::clearRecording, [&](const MessageType& message) 
        { 
            const int laneIndex = std::get<int>(message);
            if (laneIndex == -1)
                clearRecording();
            else
                midiLaneList[laneIndex].clearRecording();
        });

    eventDispatcher.addCallback(UserEvent::selectTrack, [&](const MessageType& message) 
        { 
            DBG("'selectTrack' index: " << std::get<int>(message));
            laneIndex = std::get<int>(message);
        });

    bpmParameter    = treeState.getRawParameterValue(BPM_PARAM_ID);
    volumeParameter = treeState.getRawParameterValue(GLOBAL_VOLUME_ID);

    for (size_t i = 0; i < midiLaneList.size(); i++)
    {
        midiLaneList[i].setLaneIndex(i);
        midiLaneList[i].setParameters(treeState);
    }
}

void Arrangement::prepare(const juce::dsp::ProcessSpec& spec)
{
    songData.prepare(spec);
    for (auto& midiLane : midiLaneList)
        midiLane.prepare(spec);
    metronomeLane.prepare(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.f);
    gain.setRampDurationSeconds(0.002);
}

void Arrangement::process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi)
{
    gain.setGainDecibels(volumeParameter->load(std::memory_order_relaxed));
    const float newBPM = bpmParameter->load(std::memory_order_relaxed);
    if (songData.getBPM() != newBPM)
        songData.updateBPM(newBPM);

    songData.setBufferSize(outputAudio);

    processMidiLanes(outputAudio, inputMidi);
    metronomeLane.process(outputAudio, inputMidi, songData);

    juce::dsp::AudioBlock<float> outputBlock(outputAudio);
    gain.process(juce::dsp::ProcessContextReplacing<float>(outputBlock));

    songData.incrementPlayHead();
}

void Arrangement::saveState(juce::XmlElement& xmlElement)
{
    for (auto& midiLane : midiLaneList)
        midiLane.saveState(xmlElement);
}
void Arrangement::loadState(const juce::XmlElement& xmlElement)
{
    for (auto& midiLane : midiLaneList)
        midiLane.loadState(xmlElement);
}

xynth::RingBuffer& Arrangement::getLaneRingBuffer(const int laneIndex)
{
    jassert(laneIndex >= 0);
    jassert(laneIndex < midiLaneList.size());
    return midiLaneList[laneIndex].ringBuffer;
}

void Arrangement::clearRecording()
{
    for (auto& midiLane : midiLaneList)
        midiLane.clearRecording();
}

void Arrangement::processMidiLanes(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi)
{
    for (size_t i = 0; i < midiLaneList.size(); i++) {
        const auto& midiToUse = laneIndex == i ? inputMidi : emptyMidiBuffer;
        midiLaneList[i].process(outputAudio, midiToUse, songData);
    }
}
} // namespace xynth