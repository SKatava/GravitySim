#include "Circle.h"

//Construct the circle
Circle::Circle() : m_VAO(), m_VBO(), m_pos(0.f, 0.f), m_color(1.f, 1.f, 1.f){
    m_VAO.Bind();
    m_VBO.Bind();
    m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
    m_VAO.Unbind();
    m_VBO.Unbind();
}

void Circle::SetColor(Vector3f color){
    m_color = color;
}


void Circle::UpdatePos(Vector2f pos){
    m_pos = pos;
    Update();
}

//Update the circle's VBO(positions)
void Circle::Update(){
    m_VBO.Bind();
    GLfloat vertices[216]; 
    for (int i = 0; i < 36; ++i) {
        float angle = 2.0f * M_PI * i / 36;
        vertices[i * 6] = m_pos.x + 0.05 * std::cos(angle);
        vertices[i * 6 + 1] = m_pos.y + 0.05 * std::sin(angle);
        vertices[i * 6 + 2] = 0.f;
        vertices[i * 6 + 3] = m_color.x/(i+2);
        vertices[i * 6 + 4] = m_color.y/(i+2);
        vertices[i * 6 + 5] = m_color.z/(i+2);
    }
    m_VBO.BindData(vertices, sizeof(vertices));
    m_VBO.Unbind();
}

//Draw the circle
void Circle::Draw()
{
    m_VAO.Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 36);
    m_VAO.Unbind();
}

//Delete the circle's VBO and VAO
void Circle::Delete()
{
    m_VAO.Delete();
    m_VBO.Delete();
}

Vector3f Circle::GetColor(){
    return m_color;
}