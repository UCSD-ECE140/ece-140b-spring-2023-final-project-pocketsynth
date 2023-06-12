/*
  ==============================================================================

    UserEventDispatcher.cpp
    Created: 15 May 2023 5:07:18pm
    Author:  Mark

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UserEventDispatcher.h"

namespace xynth
{

UserEventDispatcher::UserEventDispatcher(UserEventBuffer& newUserEventBuffer)
    : userEvents(newUserEventBuffer)
{}

void UserEventDispatcher::addCallback(UserEvent userEvent, const Callback callback)
{
    jassert(userEvent != UserEvent::END);

    getCallbacks(userEvent).emplace_back(callback);
}

void UserEventDispatcher::processEvents()
{
    for (const auto& userEvent : userEvents)
    {
        jassert(userEvent.type != UserEvent::END);

        DBG("[USER EVENT] " << (int)userEvent.type);
        for (auto& callback : getCallbacks(userEvent.type))
            callback(userEvent.message);
    }
}

} // namespace xynth