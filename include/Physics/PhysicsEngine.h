#ifndef PHYSICSENGINE_CLASS_H
#define PHYSICSENGINE_CLASS_H

#include "GravityEngine.h"


class PhysicsEngine{
    public:
        PhysicsEngine();
        ~PhysicsEngine();

        bool InitPhysicsEngine();
        void Step();

        static bool UpdateSSBO;
    private:
        GravityEngine* m_gravityEngine;
};

#endif

