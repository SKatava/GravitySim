#include "Events.h"

Events& Events::GetInstance(){
    static Events instance;
    return instance;
}

void Events::PushEvent(const Event& event){
    m_Events.push(event);
}

void Events::RegisterListener(EventType type, EventCallback callback){
    m_listeners[type].push_back(callback);
}

void Events::ProcessEvents(){
    while(!m_Events.empty()){
        Event event = m_Events.front();
        m_Events.pop();

        if(m_listeners.find(event.type) != m_listeners.end()){
            for(auto& callback : m_listeners[event.type]){
                callback(event);
            }
        }
    }
}
