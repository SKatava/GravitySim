#ifndef DOT_CLASS_H
#define DOT_CLASS_H

#include "glad.h"
#include <GLFW/glfw3.h>
#include "Vector2f.h"
#include "VAO.h"
#include "VBO.h"
#include <math.h>
#include <vector>
#include <iostream>

class Dot{
    public:
    Dot(Vector2f pos);
    void Update();
    void SetPos(Vector2f newPos);
    void Draw();
    void Delete();


    private:
    Vector2f m_pos;
    VAO m_VAO;
    VBO m_VBO;
};

#endif
