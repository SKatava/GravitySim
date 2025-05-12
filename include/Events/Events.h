#ifndef EVENTS_CLASS_H
#define EVENTS_CLASS_H

#include <queue>
#include <functional>
#include <vector>
#include <unordered_map>

#include "Event.h"

class Events{
    public:
        using EventCallback = std::function<void(const Event&)>;
        static Events& GetInstance();
        void PushEvent(const Event& event);
        void RegisterListener(EventType type, EventCallback callback);
        void ProcessEvents();
    private:
        std::queue<Event> m_Events;
        std::unordered_map<EventType, std::vector<EventCallback>> m_listeners;

        Events() = default;
};

#endif
