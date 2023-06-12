/*
  ==============================================================================

    SoundSelect.h
    Created: 6 Jun 2023 11:12:41am
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Utils/GuiData.h"

namespace xynth
{

class SoundSelect : public juce::ComboBox
{
public:
    SoundSelect(GuiData&, const int laneIndex);
    ~SoundSelect() override;


private:
    GuiData& guiData;

    using Attachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    std::unique_ptr<Attachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundSelect)
};

} // namespace xynth
