/*
  ==============================================================================

    UserEventDispatcher.h
    Created: 15 May 2023 5:07:18pm
    Author:  Mark

  ==============================================================================
*/

#pragma once
#include <vector>
#include <array>
#include <functional>
#include "../../Common/Events.h"

namespace xynth
{

class UserEventDispatcher
{
public:
    using Callback = std::function<void(const MessageType&)>;

    UserEventDispatcher(UserEventBuffer& newUserEventBuffer);

    void addCallback(UserEvent userEvent, const Callback callback);

    void processEvents();

private:
    std::vector<Callback>& getCallbacks(const UserEvent userEvent)
    {
        return callbackList[(size_t)userEvent];
    }

    UserEventBuffer& userEvents;

    std::array<std::vector<Callback>, (size_t)UserEvent::END> callbackList;

};

} // namespace xynth