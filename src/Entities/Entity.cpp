#include <math.h>
#include "Entity.h"
#include "Timer.h"

Entity::Entity(float mass, float radius, Vector2f pos, EntityType type) : m_color(Vector3f(1.f, 1.f, 1.f)), m_radius(radius), m_type(type), m_storedForce(Vector2f(0, 0)){
    m_data.posX = pos.x;
    m_data.posY = pos.y;
    m_data.mass = mass;
    m_data.velX = 0;
    m_data.velY = 0;
    m_data.forceX = 0;
    m_data.forceY = 0;
    m_name = "";
}

bool Entity::Update(){
    if(m_data.forceX == 0 && m_data.forceY == 0){
        return false;
    }

    float accX = m_data.forceX / m_data.mass;
    float accY = m_data.forceY / m_data.mass;
    m_data.velX += (accX * Timer::deltaTime);
    m_data.velY += (accY * Timer::deltaTime);


    m_data.posX += m_data.velX * Timer::deltaTime;
    m_data.posY += m_data.velY * Timer::deltaTime;

    m_force = sqrt(m_data.forceX * m_data.forceX + m_data.forceY * m_data.forceY);    
    m_storedForce = Vector2f(m_data.forceX, m_data.forceY);

    m_data.forceX = 0;
    m_data.forceY = 0;

    return true;
}



Vector2f Entity::GetPosition() const{
    return Vector2f(m_data.posX, m_data.posY);
}
float Entity::GetRadius(){
    return m_radius;
}

float Entity::GetMass() const{
    return m_data.mass;
}

EntityData* Entity::GetData(){
    return &m_data;
}

EntityType Entity::GetType() const{
    return m_type;
}

float Entity::GetForce() const{
    return m_force;
}

void Entity::AddForce(Vector2f force){
    m_data.forceX += force.x;
    m_data.forceY += force.y;
}

void Entity::SetColor(Vector3f color){
    m_color = color;
}

void Entity::SetName(std::string name){
    m_name = name;
}

void Entity::SetInitVelocity(float vel_x, float vel_y){
    m_data.velX = vel_x;
    m_data.velY = vel_y;
}

Vector3f Entity::GetColor() const{
    return m_color;
}

std::string Entity::GetName() const{
    return m_name;
}

Vector2f Entity::GetStoredForce(){
    return m_storedForce;
}
