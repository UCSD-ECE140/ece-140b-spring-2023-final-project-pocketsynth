/*
  ==============================================================================

    SoundSelect.cpp
    Created: 6 Jun 2023 11:12:41am
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SoundSelect.h"

namespace xynth
{

SoundSelect::SoundSelect(GuiData& g, const int laneIndex) 
    : guiData(g)
{
    addItemList(wavetableTypes, 1);
    attachment = std::make_unique<Attachment>(g.audioProcessor.treeState, getLaneParameter(LANE_SOUND_SELECT_ID, laneIndex), *this);
}

SoundSelect::~SoundSelect()
{
}

} // namespace xynth
