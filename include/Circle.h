#ifndef CIRCLE_CLASS_H
#define CIRCLE_CLASS_H

#define _USE_MATH_DEFINES

#include <glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "Vector2f.h"
#include "Vector3f.h"
#include "VAO.h"
#include "VBO.h"

class Circle{
    public:
    Circle();
    void SetColor(Vector3f color);
    void UpdatePos(Vector2f pos);
    void Update();
    void Draw();
    void Delete();

    private:
    Vector2f m_pos;
    VAO m_VAO;
    VBO m_VBO;
    Vector3f m_color;
};

#endif