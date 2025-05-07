#ifndef PARTICLE_CLASS_H
#define PARTICLE_CLASS_H

#include <vector>

#include "Entity.h"


class ParticleGroup{
    public:
        ParticleGroup(int size, float particleRadius, double mass, double groupRadius);
        ~ParticleGroup();
        
        void Update();
        
        
    private:
        std::vector<Entity> Particles;
};

#endif
