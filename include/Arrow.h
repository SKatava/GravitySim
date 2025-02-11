#ifndef ARROW_CLASS_H
#define ARROW_CLASS_H

#include <glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "VAO.h"
#include "VBO.h"
#include "Vector2f.h"
#include "Vector3f.h"

class Arrow{
    public:
        Arrow(Vector2f start, Vector2f end);
        void Update();
        void Draw();
        void Delete();
        void SetStart(Vector2f start);
        void SetEnd(Vector2f end);
        void SetColor(float r, float g, float b);
    private:
        Vector2f m_start;
        Vector2f m_end;
        Vector3f m_color;
        VAO m_VAO;
        VBO m_VBO;
};

#endif