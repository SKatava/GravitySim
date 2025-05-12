#include "GravityEngine.h"
#include <math.h>
#include <chrono>
#include <thread>
#include "Entities.h"
#include "Hardware.h"


GravityEngine::GravityEngine() {}

GravityEngine::~GravityEngine() {
    delete m_ComputeGravity;
}

bool GravityEngine::InitCompShaders(){
    m_ComputeGravity = new ComputeSystem();
    m_ComputeGravity->InitComputeSystem("assets/shaders/gravityDirectMethod.comp");
    m_ComputeGravity->WriteToSSBO();
    
    return true;
}

void GravityEngine::UpdateComputeSystem(){
    m_ComputeGravity->WriteToSSBO();
}


void GravityEngine::CalculateGravityForceOnGPU(){
    m_ComputeGravity->Run();
    m_ComputeGravity->ReadFromSSBO();
}

void GravityEngine::CalculateGravityForceMultithread(){
    int numOfThreads = std::thread::hardware_concurrency();
    int size = Entities::entities.size();
    int groupSize = size / numOfThreads;

    std::vector<std::thread> threads;
    std::vector<Vector2f> forces(size, Vector2f(0, 0));

    for (int i = 0; i < numOfThreads; i++) {
        int start = i * groupSize;
        int count = (i == numOfThreads - 1) ? (size - start) : groupSize;

        threads.emplace_back(
            static_cast<void (GravityEngine::*)(int, int, std::vector<Vector2f>& forces)>(&GravityEngine::DirectImplementation),
            this, start, count, std::ref(forces)
        );
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < size; i++) {
        Entities::entities[i].AddForce(forces[i]);
    }
}

void GravityEngine::CalculateGravityForce(){
    DirectImplementation();
}

void GravityEngine::DirectImplementation(){
    size_t size = Entities::entities.size();
    for(size_t i = 0; i < size; i++){
        for(size_t j = i+1; j < size; j++){
            double distanceX = Entities::entities[j].GetPosition().x - Entities::entities[i].GetPosition().x;
            double distanceY = Entities::entities[j].GetPosition().y - Entities::entities[i].GetPosition().y;

            double distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

            if(distance == 0) continue;
            
            double force = G * Entities::entities[i].GetMass() * Entities::entities[j].GetMass() /(distance * distance + 0.1);
            double forceX = force * (distanceX/distance);
            double forceY = force * (distanceY/distance);

            Entities::entities[i].AddForce(Vector2f(forceX, forceY));
            Entities::entities[j].AddForce(Vector2f(-forceX, -forceY));
        }
    }
}

void GravityEngine::DirectImplementation(int start, int size, std::vector<Vector2f>& forces){
    for(size_t i = start; i < start+size; i++){
        for(size_t j = i+1; j < Entities::entities.size(); j++){
            double distanceX = Entities::entities[j].GetPosition().x - Entities::entities[i].GetPosition().x;
            double distanceY = Entities::entities[j].GetPosition().y - Entities::entities[i].GetPosition().y;

            double distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

            if(distance == 0) continue;
            
            double force = G * Entities::entities[i].GetMass() * Entities::entities[j].GetMass() /(distance * distance + 0.1);
            double forceX = force * (distanceX/distance);
            double forceY = force * (distanceY/distance);

            forces[i] += Vector2f(forceX, forceY);
            forces[j] += Vector2f(-forceX, -forceY);
        }
    }
}
