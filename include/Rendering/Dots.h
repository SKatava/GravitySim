#ifndef DOTS_CLASS_H
#define DOTS_CLASS_H

#include <glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "VAO.h"
#include "VBO.h"
#include "Vector2f.h"
#include "Vector3f.h"

class Dots{
    public:
        Dots();
        void SetColor(Vector3f color, int index);
        void UpdatePos(Vector2f pos, int index);
        void AddDot(Vector2f pos, Vector3f color);
        void DeleteDot(int index);
        void Update();
        void Draw();
        void Delete();
        int GetSize();
    private:
        std::vector<GLfloat> m_data;
        VAO m_VAO;
        VBO m_VBO;

};

#endif
