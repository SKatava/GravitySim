#ifndef GRAVITY_CLASS_H
#define GRAVITY_CLASS_H

#include "ComputeSystem.h"

#define G 6.6743e-11



class GravityEngine{
    public:
        GravityEngine();
        ~GravityEngine();
    

        bool InitCompShaders();
        void CalculateGravityForceOnGPU();
        void CalculateGravityForceMultithread();
        void CalculateGravityForce();
        void DirectImplementation();
        void DirectImplementation(int start, int size, std::vector<Vector2f>& forces);
        void UpdateComputeSystem();


    private:
        ComputeSystem* m_ComputeGravity;
};

#endif
