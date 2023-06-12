/*
  ==============================================================================

    EventBuffer.h
    Created: 15 May 2023 2:31:57pm
    Author:  Mark

  ==============================================================================
*/

#pragma once

#include <vector>
#include <atomic>

namespace xynth
{

// Thread safe, non-blocking ring buffer of events
template<typename EventType, size_t length = 1000>
class EventBuffer
{
public:
    // Iterator class to allow range-based for loops
    class Iterator 
    {
    public:
        Iterator(EventBuffer* aParent, const size_t index)
            : parent(aParent), eventIndex(index) {}

        ~Iterator()
        {
            parent->readIndex.store(eventIndex, std::memory_order_release);
        }

        const EventType& operator*() const
        {
            return (*parent)[eventIndex];
        }

        const EventType* operator->() const
        { 
            return &(operator*()); 
        }

        Iterator& operator++() 
        {
            eventIndex = (eventIndex + 1) % length;
            return *this;
        }
        
        bool operator==(const Iterator& other) const 
        {
            return eventIndex == other.eventIndex && parent == other.parent;
        }

        bool operator!=(const Iterator& other) const 
        {
            return !(*this == other);
        }

        size_t getIndex() const { return eventIndex; }

    protected:
        EventBuffer* parent;
        size_t eventIndex;

    };

    EventBuffer()
    {
        eventList.resize(length);
    }

    // Add a new event (call from transmitter thread)
    void pushBack(const EventType& newEvent)
    {
        const size_t currentIndex = writeIndex.load(std::memory_order_acquire);
        eventList[currentIndex] = newEvent;
        writeIndex.store((currentIndex + 1) % length, std::memory_order_release);
    }

    // Iterator methods (call from receiver thread)
    Iterator begin() 
    {
        return Iterator(this, readIndex.load(std::memory_order_acquire));
    }

    Iterator end() 
    {
        return Iterator(this, writeIndex.load(std::memory_order_acquire));
    }

protected:
    // NOT thread-safe, use the Iterator via begin() & end() instead
    const EventType& operator[](const size_t index) const
    {
        return eventList[index];
    }

    std::vector<EventType> eventList;

    std::atomic<size_t> readIndex{ 0 }, writeIndex{ 0 };
    
};

} // namespace xynth