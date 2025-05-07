#include "Entities.h"
#include "Events.h"
#include "Settings.h"
#include "BHTree.h"
#include <random>
#include <iostream>
#include <set>
#include <algorithm>

#include "Settings.h"
#include "PhysicsEngine.h"

std::vector<Entity> Entities::entities;
std::vector<std::array<int, 4>> Entities::groups;
int Entities::ObjectCount = 0;
int Entities::ParticleCount = 0;

Entities::Entities(){}

Entities::~Entities(){}

bool Entities::InitEntities(){
    entities.push_back(Entity(1.0e18, 0.05, Vector2d(400, 400), EntityType::OBJECT));
    entities.back().SetColor(Vector3f(0, 1.f, 0));
    entities.back().SetName("Sun");
    Events::GetInstance().PushEvent( {EventType::ENTITY_CREATED, 0} );
    groups.push_back({0, 1, ObjectCount++, 1});
    
    entities.push_back(Entity(1.0e4, 0.05, Vector2d(200, 400), EntityType::OBJECT));
    entities.back().SetColor(Vector3f(0, 0, 1.f));
    entities.back().SetName("Earth");
    Events::GetInstance().PushEvent( {EventType::ENTITY_CREATED, 1} );
    groups.push_back({1, 1, ObjectCount++, 1});
    entities[1].SetInitVelocity(0, 577.35f);

     entities.push_back(Entity(1.0e4, 0.05, Vector2d(600, 600), EntityType::OBJECT));
    entities.back().SetColor(Vector3f(1.f, 0, 0));
    entities.back().SetName("TEST");
    Events::GetInstance().PushEvent( {EventType::ENTITY_CREATED, 2} );
    groups.push_back({2, 1, ObjectCount++, 1});
    return true;
}

void Entities::UpdateEntities(){
    if(Settings::state == State::STOPPED){
        return;
    }
    else if(Settings::mode == Mode::GPU_ACCELERATION || Settings::mode == Mode::ULTRA){
        return; 
    }
    else if(Settings::mode == Mode::NORMAL || Settings::mode == Mode::CPU_MULTITHREAD){
        for(int i = 0; i < entities.size(); i++){entities[i].Update();}
        Events::GetInstance().PushEvent({EventType::ALL_UPDATED, -1}); 
    }
}

void Entities::CreateEntity(double mass, float radius, Vector2d pos, EntityType type, Vector3f color, std::string name){
    float x = (pos.x / 400) - 1.f; 
    float y = 1.f - (pos.y / 400);
    float r = radius/400;
    entities.push_back(Entity(mass, r, pos, type));
    entities.back().SetColor(color);
    entities.back().SetName(name);
    if(type == EntityType::OBJECT) {
        Events::GetInstance().PushEvent( {EventType::ENTITY_CREATED, (int)entities.size() -1} );
        groups.push_back({(int)entities.size()-1, 1, ObjectCount++, 1});
    }
    else if(type == EntityType::PARTICLE){
        Events::GetInstance().PushEvent( {EventType::PARTICLE_CREATED, (int)entities.size() -1});
        groups.push_back({(int)entities.size()-1, 1, ParticleCount++, 1});
    }
    PhysicsEngine::UpdateSSBO = true;
}

void Entities::CreateBachOfEntities(double mass, float radius, Vector2d pos, EntityType type, Vector3f color, std::string name, int num){
    float x = (pos.x / 400) - 1.f; 
    float y = 1.f - (pos.y / 400);
    float r = radius/400;
    if(type == EntityType::OBJECT){
        entities.push_back(Entity(mass, r, pos , type));
        entities.back().SetColor(color);
        entities.back().SetName(name);
        groups.push_back({(int)entities.size()-1, num, ObjectCount, num});
        ObjectCount += num;
        Events::GetInstance().PushEvent( {EventType::ENTITY_CREATED, (int)entities.size() -1} );
        std::vector<Vector2d> positions = GenerateUniquePositions(num-1, 0, 800, 0, 800);
        for(int i = 0; i < num-1; i++){
            entities.push_back(Entity(mass, r, positions[i] , type));
            entities.back().SetColor(color);
            entities.back().SetName(name);
            Events::GetInstance().PushEvent( {EventType::ENTITY_CREATED, (int)entities.size() -1} );
        }
    }
    else if(type == EntityType::PARTICLE){
        std::vector<Vector2d> positions = GenerateUniquePositions(num, pos.x-radius, pos.x+radius, pos.y-radius, pos.y+radius);
        groups.push_back({(int)entities.size(), num, ParticleCount, num});
        ParticleCount += num;
        for(int i = 0; i < num; i++){
            entities.push_back(Entity(mass, r, positions[i] , type));
            entities.back().SetColor(color);
            entities.back().SetName(name);
            Events::GetInstance().PushEvent( {EventType::PARTICLE_CREATED, (int)entities.size() -1} );
        }
    }
    PhysicsEngine::UpdateSSBO = true;
}

void Entities::DeleteEntity(int index){
    if(entities[groups[index][0]].GetType() == EntityType::PARTICLE){
        Events::GetInstance().PushEvent( {EventType::PARTICLE_DELETED, groups[index][2]} );
    }
    else if(entities[groups[index][0]].GetType() == EntityType::OBJECT){ 
        Events::GetInstance().PushEvent( {EventType::ENTITY_DELETED, groups[index][2]} );
    }
    entities.erase(entities.begin() + groups[index][0]);
    groups.erase(groups.begin() + index);
    for(int i = index; i < groups.size(); i++){
        groups[i][0]--; 
        if(entities[groups[index][0]].GetType() == entities[groups[i][0]].GetType()){
            groups[i][2]--;
        }
    }
}

void Entities::DeleteBachOfEntities(int index){
    EntityType type = entities[groups[index][0]].GetType();
    EventType evType;
    int size = groups[index][1];
    if(type == EntityType::PARTICLE){ 
        evType = EventType::PARTICLE_DELETED;
        ParticleCount -= size; 
    }
    else if(type == EntityType::OBJECT){
        evType = EventType::ENTITY_DELETED;
        ObjectCount -= size;
    }

    for(int i = 0; i < groups[index][3]; i++){   
        Events::GetInstance().PushEvent( {evType, groups[index][2]} );
    }
    entities.erase(entities.begin() + groups[index][0], entities.begin() + groups[index][0] + groups[index][1]);
    groups.erase(groups.begin() + index);
    for(int i = index; i < groups.size(); i++){
       groups[i][0] -= size; 
       if(entities[groups[i][0]].GetType() == type){
           groups[i][2] -= size;
       }
    }

}

std::vector<Vector2d> Entities::GenerateUniquePositions(int count, double minX, double maxX, double minY, double maxY) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> distX(minX, maxX);
    std::uniform_real_distribution<double> distY(minY, maxY);

    std::set<Vector2d> uniquePositions;
    while (uniquePositions.size() < count) {
        uniquePositions.insert({ distX(rng), distY(rng) });
    }

    return std::vector<Vector2d>(uniquePositions.begin(), uniquePositions.end());
}


uint64_t Entities::ZOrder(double x, double y){
    uint64_t xi = *reinterpret_cast<uint64_t*>(&x); 
    uint64_t yi = *reinterpret_cast<uint64_t*>(&y); 

    uint64_t zOrder = 0;
    for (int i = 0; i < 64; i++) {
        zOrder |= (xi & (1ULL << i)) << i; 
        zOrder |= (yi & (1ULL << i)) << (i + 1); 
    }
    return zOrder;
}

void Entities::SortEntities(){
   std::sort(Entities::entities.begin(), Entities::entities.end(),
    [](const Entity& a, const Entity& b) {
        uint64_t zOrderA = ZOrder(a.GetPosition().x, a.GetPosition().y);
        uint64_t zOrderB = ZOrder(b.GetPosition().x, b.GetPosition().y);
        return zOrderA < zOrderB;
    });
 
}

