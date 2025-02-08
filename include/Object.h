#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include <iostream>

#include "globals.h"
#include "Circle.h"
#include "Vector2f.h"
#include "Vector3f.h"


class Object{
    public:
    Object(float mass, Vector2f pos, Vector3f color);
    void SetInitialVelocity(Vector2f vel);
    void SetForce(Vector2f force);
    void AddForce(Vector2f force);
    void Update();
    void UpdateTrace();
    void UpdateForceHistory();
    void Draw();
    void Delete();

    float GetMass();
    Vector2f GetPos();
    Vector2f GetVelocity();
    Vector2f GetAcc();
    Vector2f GetForce();
    int GetID();
    float* GetTraceX();
    float* GetTraceY();
    float* GetForceHistory();
    float GetMaxForce();

    private:
    int m_ID;
    float m_mass;
    Vector2f m_pos;
    Vector2f m_force;
    Vector2f m_acc;
    Vector2f m_velocity;
    Circle m_circle;
    float m_maxForce;
    float m_traceX[64];
    float m_traceY[64];
    float m_forceHistory[16];
};

#endif