#ifndef OBJECTMANAGER_NAMESPACE_H
#define OBJECTMANAGER_NAMESPACE_H

#include <vector>

#include "Object.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "CollisionSolution.h"

namespace OBJMG{
    extern std::vector<Object> objects;

    void AddObject(float mass, Vector2f pos, Vector3f color);
    void DrawObjects();
    void DeleteObjects();
    void Delete(int index);
    void UpdateObjects();
}


#endif