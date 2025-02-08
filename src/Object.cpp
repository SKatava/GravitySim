#include "Object.h"

//Construct the object
Object::Object(float mass, Vector2f pos, Vector3f color) : 
    m_mass(mass), m_pos(pos), m_acc(Vector2f(0.f, 0.f)), m_velocity(Vector2f(0.f, 0.f)), m_force(Vector2f(0.f, 0.f)), m_circle() {
    Vector2f GLpos(0.f, 0.f);
    GLpos.x = (m_pos.x / 500.f) - 1.f;
    GLpos.y = 1.f - (m_pos.y / 500.f);
    m_circle.SetColor(color);
    m_circle.UpdatePos(GLpos);
    m_ID = ID++;
    for(int i = 0; i < 64; i++){
        m_traceX[i] = -10;
        m_traceY[i] = -10;
    }
    for(int i = 0; i < 16; i++){
        m_forceHistory[i] = 0;
    }
}

//Set initial velocity of the object
void Object::SetInitialVelocity(Vector2f vel){
    m_velocity = vel;
}

//Change the force of the object
void Object::SetForce(Vector2f force){
    m_force = force;
}

//Add force to object
void Object::AddForce(Vector2f force){
    m_force.x += force.x;
    m_force.y += force.y;
}

//Update the object based on current values and time
void Object::Update(){
    m_acc.x = m_force.x / m_mass;
    m_acc.y = m_force.y / m_mass;

    m_velocity.x += (m_acc.x * deltaTime);
    m_velocity.y += (m_acc.y * deltaTime);

    m_pos.x += m_velocity.x * deltaTime;
    m_pos.y += m_velocity.y * deltaTime;

    UpdateForceHistory();
    
    m_force.x = 0.f;
    m_force.y = 0.f;
    
    UpdateTrace();
}

//Update trace
void Object::UpdateTrace(){
    if(FRAME_COUNTER % 500 != 0){
        return;
    }
    FRAME_COUNTER = 0;
    for(int i = 1; i < 64; ++i){
        m_traceX[i - 1] = m_traceX[i]; 
        m_traceY[i - 1] = m_traceY[i];
    }
    m_traceX[63] = m_pos.x;
    m_traceY[63] = m_pos.y;
}

//Add force to the history
void Object::UpdateForceHistory(){
    if(FRAME_COUNTER % 500 != 0){
        return;
    }
    FRAME_COUNTER = 0;
    for(int i = 1; i < 16; ++i){
        m_forceHistory[i - 1] = m_forceHistory[i]; 
    }
    m_forceHistory[15] = std::sqrt(m_force.x * m_force.x + m_force.y*m_force.y);
}

//Draw the object
void Object::Draw(){
    Vector2f GLpos(0.f, 0.f);
    GLpos.x = (m_pos.x / 500.f) - 1.f;
    GLpos.y = 1.f - (m_pos.y / 500.f);
    m_circle.UpdatePos(GLpos);
    m_circle.Draw();
} 

//Delete the object
void Object::Delete(){
    m_circle.Delete();
}

float Object::GetMass(){
    return m_mass;
}

Vector2f Object::GetPos(){
    return m_pos;
}

Vector2f Object::GetVelocity(){
    return m_velocity;
}

Vector2f Object::GetAcc(){
    return m_acc;
}

Vector2f Object::GetForce(){
    return m_force;
}

int Object::GetID(){
    return m_ID;
}

float* Object::GetTraceX(){
    return m_traceX;
}
float* Object::GetTraceY(){
    return m_traceY;
}

float* Object::GetForceHistory(){
    return m_forceHistory;
}
