#ifndef LINE_CLASS_H
#define LINE_CLASS_H

#include "Vector2f.h"
#include "Vector3f.h"
#include "VAO.h"
#include "VBO.h"

class Line{
    public:
    Line(Vector2f posA, Vector2f posB);
    void SetPosA(Vector2f newPosA);
    void SetPosB(Vector2f newPosB);
    void Update();
    void Draw();
    void SetColor(float r, float g, float b);
    void Delete();

    private:
    Vector3f m_color;
    Vector2f m_posA;
    Vector2f m_posB;
    VAO m_VAO;
    VBO m_VBO;
};

#endif
