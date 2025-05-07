#include "GravityEngine.h"
#include <math.h>
#include <chrono>
#include <thread>
#include "Entities.h"
#include "BHTree.h"
#include "Hardware.h"

GravityEngine::GravityEngine() : m_algorithm(GravityAlgorithm::DIRECT_IMPLEMENTATION) {}

GravityEngine::~GravityEngine() {
    delete m_ComputeGravity;
    delete m_ComputeBarnesHut;
}

bool GravityEngine::InitCompShaders(){
    m_ComputeGravity = new ComputeSystem();
    m_ComputeGravity->InitComputeSystem("shaders/gravityDirectMethod.comp");
    m_ComputeGravity->WriteToSSBO();
    m_ComputeBarnesHut = new ComputeSystem();
    m_ComputeBarnesHut->InitComputeSystem("shaders/barnesHutMethod.comp");
    m_ComputeBarnesHut->WriteToSSBO();
    return true;
}

void GravityEngine::UpdateComputeSystem(){
    m_ComputeGravity->WriteToSSBO();
    m_ComputeBarnesHut->WriteToSSBO();
}

void GravityEngine::CalculateGravityBarnesHut(){
    m_ComputeBarnesHut->WriteToSSBOExtra(BarnesHutAlgorithm());
    m_ComputeBarnesHut->Run();
    m_ComputeBarnesHut->ReadFromSSBO();
}

void GravityEngine::CalculateGravityForceOnGPU(){
    if(m_algorithm == GravityAlgorithm::DIRECT_IMPLEMENTATION) {
        m_ComputeGravity->Run();
        m_ComputeGravity->ReadFromSSBO();
    }
    else if(m_algorithm == GravityAlgorithm::BARNES_HUT_ALGORITHM) {}
}

void GravityEngine::CalculateGravityForceMultithread(){
    int numOfThreads = std::stoi(Hardware::GetNmuOfCpuThreads());
    int size = Entities::entities.size();
    int groupSize = size / numOfThreads;

    std::vector<std::thread> threads;

    for (int i = 0; i < numOfThreads; i++) {
        int start = i * groupSize;
        int count = (i == numOfThreads - 1) ? (size - start) : groupSize;

        threads.emplace_back(
            static_cast<void (GravityEngine::*)(int, int)>(&GravityEngine::DirectImplementation),
            this, start, count
        );
    }

    for (auto& t : threads) {
        t.join();
    }
}

void GravityEngine::CalculateGravityForce(){
    if(m_algorithm == GravityAlgorithm::DIRECT_IMPLEMENTATION) {DirectImplementation();}
    else if(m_algorithm == GravityAlgorithm::BARNES_HUT_ALGORITHM) {BarnesHutAlgorithm();}
}

void GravityEngine::SetGravityAlgorithm(GravityAlgorithm algorithm){
    m_algorithm = algorithm;
}

BHTree GravityEngine::BarnesHutAlgorithm(){
    BHTree tree;
    for(int i = 0; i < Entities::entities.size(); i++){
        tree.InsertEntity(0, i);
    }
    return tree;
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

            Entities::entities[i].AddForce(Vector2d(forceX, forceY));
            Entities::entities[j].AddForce(Vector2d(-forceX, -forceY));
        }
    }
}

void GravityEngine::DirectImplementation(int start, int size){
    for(size_t i = start; i < start+size; i++){
        for(size_t j = i+1; j < start+size; j++){
            double distanceX = Entities::entities[j].GetPosition().x - Entities::entities[i].GetPosition().x;
            double distanceY = Entities::entities[j].GetPosition().y - Entities::entities[i].GetPosition().y;

            double distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

            if(distance == 0) continue;
            
            double force = G * Entities::entities[i].GetMass() * Entities::entities[j].GetMass() /(distance * distance + 0.1);
            double forceX = force * (distanceX/distance);
            double forceY = force * (distanceY/distance);

            Entities::entities[i].AddForce(Vector2d(forceX, forceY));
            Entities::entities[j].AddForce(Vector2d(-forceX, -forceY));
        }
    }
}
