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
        
        static void CreateEntity(double mass, float radius, Vector2d pos, EntityType type, Vector3f color, std::string name);
        static void CreateBachOfEntities(double mass, float radius, Vector2d pos, EntityType type, Vector3f color, std::string name, int num);
        static void DeleteEntity(int index);
        static void DeleteBachOfEntities(int index);
        static uint64_t ZOrder(double x, double y);
        static void SortEntities();
        static std::vector<Vector2d> GenerateUniquePositions(int count, double minX, double maxX, double minY, double maxY);

    private:
};

#endif
