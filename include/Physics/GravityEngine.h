#ifndef GRAVITY_CLASS_H
#define GRAVITY_CLASS_H

#include "ComputeSystem.h"

#define G 6.6743e-11

enum class GravityAlgorithm{
    DIRECT_IMPLEMENTATION,
    BARNES_HUT_ALGORITHM
};


class GravityEngine{
    public:
        GravityEngine();
        ~GravityEngine();
    
        bool InitCompShaders();
        void CalculateGravityBarnesHut();
        void CalculateGravityForceOnGPU();
        void CalculateGravityForceMultithread();
        void CalculateGravityForce();
        void SetGravityAlgorithm(GravityAlgorithm algorithm);
        void DirectImplementation();
        void DirectImplementation(int start, int size);
        BHTree BarnesHutAlgorithm();
        void UpdateComputeSystem();

    private:
        GravityAlgorithm m_algorithm;
        ComputeSystem* m_ComputeGravity;
        ComputeSystem* m_ComputeBarnesHut;
};

#endif
