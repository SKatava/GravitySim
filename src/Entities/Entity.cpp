#include <math.h>
#include "Entity.h"
#include "Time.h"

Entity::Entity(double mass, float radius, Vector2d pos, EntityType type) : m_color(Vector3f(1.f, 1.f, 1.f)), m_radius(radius), m_type(type){
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
    m_data.velX += (accX * Time::deltaTime);
    m_data.velY += (accY * Time::deltaTime);


    m_data.posX += m_data.velX * Time::deltaTime;
    m_data.posY += m_data.velY * Time::deltaTime;

    m_force = sqrt(m_data.forceX * m_data.forceX + m_data.forceY * m_data.forceY);    

    m_data.forceX = 0;
    m_data.forceY = 0;

    return true;
}



Vector2d Entity::GetPosition() const{
    return Vector2d(m_data.posX, m_data.posY);
}
float Entity::GetRadius(){
    return m_radius;
}

double Entity::GetMass() const{
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

void Entity::AddForce(Vector2d force){
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
