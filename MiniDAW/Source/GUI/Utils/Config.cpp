/*
  ==============================================================================

    Config.cpp
    Created: 7 Jun 2023 9:51:12pm
    Author:  Mark

  ==============================================================================
*/

#include "Config.h"

namespace xynth
{

juce::String Config::getURL()
{
    return "http://192.168.1.2:31337/projects";
}

} // namespace xynth