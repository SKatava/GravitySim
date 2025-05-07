#ifndef EVENT_CLASS_H
#define EVENT_CLASS_H

enum class EventType {
    ENTITY_CREATED,
    ENTITY_DELETED,
    ENTITY_UPDATED,
    ENTITY_ALL_UPDATED,
    PARTICLE_CREATED,
    PARTICLE_UPDATED,
    PARTICLE_DELETED,
    PARTICLE_ALL_UPDATED,
    ALL_UPDATED
};

class Event{
    public:
        EventType type;
        int ID;
};

#endif
