/*
  ==============================================================================

    MidiLane.cpp
    Created: 16 May 2023 9:56:04am
    Author:  Mark

  ==============================================================================
*/

#include "MidiLane.h"

namespace xynth
{

void MidiLane::setParameters(juce::AudioProcessorValueTreeState& treeState)
{
    jassert(laneIndex != -1);

    volumeParameter      = treeState.getRawParameterValue(getLaneParameter(LANE_VOLUME_ID,       laneIndex));
    soundSelectParameter = treeState.getRawParameterValue(getLaneParameter(LANE_SOUND_SELECT_ID, laneIndex));
    pluckParameter       = treeState.getRawParameterValue(getLaneParameter(LANE_PLUCK_ID,        laneIndex));
    releaseParameter     = treeState.getRawParameterValue(getLaneParameter(LANE_RELEASE_ID,      laneIndex));
    reverbParameter      = treeState.getRawParameterValue(getLaneParameter(LANE_REVERB_ID,       laneIndex));
}

void MidiLane::prepare(const juce::dsp::ProcessSpec& spec)
{
    audioBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
    synthProcessor.prepare(spec);
    ringBuffer.prepare(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.f);
    gain.setRampDurationSeconds(0.002);

    reverb.prepare(spec);
}

void addAudioBuffers(juce::AudioBuffer<float>& destination, const juce::AudioBuffer<float>& source)
{
    for (int ch = 0; ch < destination.getNumChannels(); ch++)
        destination.addFrom(ch, 0, source.getReadPointer(ch), destination.getNumSamples());
}

void MidiLane::process(juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi, const SongData& songData)
{
    audioBuffer.setSize(outputAudio.getNumChannels(), outputAudio.getNumSamples(), false, false, true);
    audioBuffer.clear();

    midiSubBuffer.clear();
    midiSubBuffer.addEvents(inputMidi, 0, songData.getBufferSize().samples, 0);

    midiStore.readEvents(midiSubBuffer, songData);
    midiStore.addEvents(inputMidi, songData);
    
    synthProcessor.setWavetableSound((int)soundSelectParameter->load(std::memory_order_relaxed));
    synthProcessor.setPluckAmount(pluckParameter->load(std::memory_order_relaxed));
    synthProcessor.setReleaseAmount(releaseParameter->load(std::memory_order_relaxed));
    synthProcessor.process(audioBuffer, midiSubBuffer);

    juce::dsp::AudioBlock<float> audioBlock(audioBuffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    reverb.setAmount(reverbParameter->load(std::memory_order_relaxed));
    reverb.process(context);

    gain.setGainDecibels(volumeParameter->load(std::memory_order_relaxed));
    gain.process(context);

    ringBuffer.pushBack(audioBuffer);
    addAudioBuffers(outputAudio, audioBuffer);
}

void MidiLane::saveState(juce::XmlElement& xmlElement)
{
    using XmlElement = juce::XmlElement;
    XmlElement* laneElement = new XmlElement(getName());
    
    midiStore.saveState(*laneElement);

    xmlElement.addChildElement(laneElement);
}

void MidiLane::loadState(const juce::XmlElement& xmlElement)
{
    if (auto* laneElement = xmlElement.getChildByName(getName()))
        midiStore.loadState(*laneElement);
}

void MidiLane::clearRecording()
{
    midiStore.clear();
    synthProcessor.reset();
}

void MidiLane::setLaneIndex(const int newLaneIndex)
{
    laneIndex = newLaneIndex;
}

juce::String MidiLane::getName()
{
    return "MidiLane" + juce::String(laneIndex);
}

} // namespace xynth
