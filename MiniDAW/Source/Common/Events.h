/*
  ==============================================================================

    Events.h
    Created: 15 May 2023 2:34:30pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <variant>
#include "EventBuffer.h"

namespace xynth
{

// These are all the events going from the audio thread to the GUI thread
enum class AudioEvent
{
    nullEvent = 0,

    END // Length of enum
};

// These are all the events going from the GUI thread to the audio thread
enum class UserEvent
{
    nullEvent = 0,
    playEvent,      // User hits 'play'
    stopEvent,      // User hits 'stop'
    startRecord,    // User starts recording
    stopRecord,     // User stops the recording
    clearRecording, // Clear the recording in the current lane
    metronomeOn,    // Turn on the metronome
    metronomeOff,   // Turn off the metronome
    selectTrack,    // User selects a certain track

    END // Length of enum
};

using MessageType = std::variant<bool, int, float>;

template<typename T>
struct Event
{
    Event() = default;
    Event(const T newType) : type(newType) {}
    Event(const T newType, const MessageType& newMessage) 
        : type(newType), message(newMessage) {}

    T type;
    MessageType message;
};

using AudioEventBuffer = EventBuffer<Event<AudioEvent>>;
using UserEventBuffer  = EventBuffer<Event<UserEvent>>;

} // namespace xynth