#ifndef ENTITY_CLASS_H
#define ENTITY_CLASS_H

#include <string>

#include "Vector3f.h"
#include "Vector2d.h"

struct EntityData{
    double posX; //8
    double posY; //8
    double velX; //8
    double velY; //8
    double forceX; //8
    double forceY; //8
    double mass; //8
    // Total: 56
    double padding; //8
    // Total: 64 (need to be multiple of 16 for GPU optimization)
};

enum class EntityType{
    PARTICLE,
    OBJECT
};

class Entity{
    public:
        Entity(double mass, float radius, Vector2d pos, EntityType type);

        bool Update();
        void AddForce(Vector2d force);
        void SetColor(Vector3f color);
        void SetName(std::string name);
        void SetInitVelocity(float vel_x, float vel_y);

        Vector2d GetPosition() const;
        float GetRadius();
        double GetMass() const;
        EntityData* GetData();
        EntityType GetType() const;
        Vector3f GetColor() const;
        std::string GetName() const;
        float GetForce() const;

    private:
        EntityData m_data;
        float m_radius;
        Vector3f m_color;
        EntityType m_type;
        std::string m_name; 
        float m_force;
};

#endif
