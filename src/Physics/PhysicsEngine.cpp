#include "PhysicsEngine.h"
#include "Entities.h"
#include "Settings.h"

bool PhysicsEngine::UpdateSSBO = false;

PhysicsEngine::PhysicsEngine(){
    m_gravityEngine = new GravityEngine();
}

PhysicsEngine::~PhysicsEngine(){
    delete m_gravityEngine;
}

bool PhysicsEngine::InitPhysicsEngine(){
    m_gravityEngine->InitCompShaders();
    return true;
}

void PhysicsEngine::Step(){
    if(UpdateSSBO){
        m_gravityEngine->UpdateComputeSystem();
        UpdateSSBO = false;
    }
    if(Settings::state == State::STOPPED){
        return;
    }
    if(Settings::mode == Mode::NORMAL){
        m_gravityEngine->CalculateGravityForce();
    }
    else if(Settings::mode == Mode::CPU_MULTITHREAD){
        m_gravityEngine->CalculateGravityForceMultithread(); 
    }
    else if(Settings::mode == Mode::GPU_ACCELERATION){
        m_gravityEngine->CalculateGravityForceOnGPU();
    }
    else if(Settings::mode == Mode::ULTRA){
        m_gravityEngine->CalculateGravityBarnesHut();
    }
}
