#include "Dot.h"

Dot::Dot(Vector2f pos) : m_pos(pos){
    m_VAO.Bind();
    m_VBO.Bind();
    m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
    m_VAO.Unbind();
    m_VBO.Unbind();
}

void Dot::SetPos(Vector2f newPos){m_pos = newPos;}

void Dot::Update(){
    m_VBO.Bind();
    GLfloat vertices[] = {
        m_pos.x, m_pos.y, 0.f,
        1.f, 1.f, 1.f
    };
    m_VBO.BindData(vertices, sizeof(vertices));
    m_VBO.Unbind();
}

void Dot::Draw(){
    m_VAO.Bind();
    glDrawArrays(GL_POINTS, 0, 1);
    m_VAO.Unbind();
}

void Dot::Delete(){
    m_VAO.Delete();
    m_VBO.Delete();
}
