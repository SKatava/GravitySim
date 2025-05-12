#ifndef ENTITIES_CLASS_H
#define ENTITIES_CLASS_H

#include "Entity.h"
#include <vector>
#include <array>
#include <cstdint>
#include <string>

class Entities{
    public:
        Entities();
        ~Entities();
        
        static std::vector<Entity> entities;
        static std::vector<std::array<int, 4>> groups;
        static int ObjectCount;
        static int ParticleCount;

        bool InitEntities();
        void UpdateEntities();
        
        static void CreateEntity(float mass, float radius, Vector2f pos, EntityType type, Vector3f color, std::string name, Vector2f velocity);
        static void CreateBachOfEntities(float mass, float radius, Vector2f pos, EntityType type, Vector3f color, std::string name, int num, Vector2f velocity);
        static void DeleteEntity(int index);
        static void DeleteBachOfEntities(int index);
        static std::vector<Vector2f> GenerateUniquePositions(int count, float minX, float maxX, float minY, float maxY);

    private:
};

#endif
