#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include <string>

#include "Vector3f.h"
#include "Vector2f.h"

struct EntityData{
    float posX; // 4 bytes
    float posY; // 4 bytes
    float velX; // 4 bytes
    float velY; // 4 bytes
    float forceX; // 4 bytes
    float forceY; // 4 bytes
    float mass; // 4 bytes
    float padding; // 4 bytes - to make it multiple of 16(great for GPU)
    //--TOTAL:32 bytes
};

enum class EntityType{
    PARTICLE,
    OBJECT
};

class Entity{
    public:
        Entity(float mass, float radius, Vector2f pos, EntityType type);

        bool Update();
        void AddForce(Vector2f force);
        void SetColor(Vector3f color);
        void SetName(std::string name);
        void SetInitVelocity(float vel_x, float vel_y);

        Vector2f GetPosition() const;
        float GetRadius();
        float GetMass() const;
        EntityData* GetData();
        EntityType GetType() const;
        Vector3f GetColor() const;
        std::string GetName() const;
        float GetForce() const;
        Vector2f GetStoredForce();

    private:
        EntityData m_data;
        float m_radius;
        Vector3f m_color;
        EntityType m_type;
        std::string m_name; 
        float m_force;
        Vector2f m_storedForce;
};

#endif
