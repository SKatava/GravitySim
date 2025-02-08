#include "Line.h"

//Construct the line
Line::Line(Vector2f posA, Vector2f posB) : m_posA(posA), m_posB(posB), m_color(1.f, 1.f, 1.f) {
    m_VAO.Bind();
    m_VBO.Bind();
    m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
    m_VAO.Unbind();
    m_VBO.Unbind();   
}

void Line::SetPosA(Vector2f newPosA){m_posA = newPosA;}

void Line::SetPosB(Vector2f newPosB){m_posB = newPosB;}

void Line::SetColor(float r, float g, float b){
    m_color.x = r;
    m_color.y = g;
    m_color.z = b;
}

//Update the line VBO(positions)
void Line::Update(){
    m_VBO.Bind();
    GLfloat vertices[] = {
        m_posA.x, m_posA.y, 0.f, m_color.x, m_color.y, m_color.z,
        m_posB.x, m_posB.y, 0.f, m_color.x, m_color.y, m_color.z
    };
    m_VBO.BindData(vertices, sizeof(vertices));
    m_VBO.Unbind();
}

//Draw the line
void Line::Draw(){
    m_VAO.Bind();
    glDrawArrays(GL_LINES, 0, 2);
    m_VAO.Unbind();
}

//Delete the line's VBO and VAO
void Line::Delete(){
    m_VAO.Delete();
    m_VBO.Delete(); 
}
