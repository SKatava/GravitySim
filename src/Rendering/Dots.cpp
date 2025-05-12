#include "Dots.h"
#include <iostream>


Dots::Dots() : m_VAO(), m_VBO(){
    m_VAO.Bind();
    m_VBO.Bind();
    m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
    m_VAO.Unbind();
    m_VBO.Unbind();
}

void Dots::SetColor(Vector3f color, int index){
    size_t startIndex = (index)*6;
    m_data[startIndex+3] = color.x;
    m_data[startIndex+4] = color.y;
    m_data[startIndex+5] = color.z;
}


void Dots::UpdatePos(Vector2f pos, int index){
    size_t startIndex = (index)*6;
    m_data[startIndex] = pos.x;
    m_data[startIndex+1] = pos.y;
    m_data[startIndex+2] = 0.f;
}

void Dots::AddDot(Vector2f pos, Vector3f color){
    m_data.push_back(pos.x);
    m_data.push_back(pos.y);
    m_data.push_back(0.f);
    m_data.push_back(color.x);
    m_data.push_back(color.y);
    m_data.push_back(color.z);
}

void Dots::DeleteDot(int index){
    size_t startIndex = index*6;
    m_data.erase(m_data.begin() + startIndex, m_data.begin() + startIndex + 6);
}

void Dots::Update(){
    m_VBO.Bind();
    m_VBO.BindData(m_data.data(), sizeof(GLfloat) * m_data.size());
    m_VBO.Unbind();
}

void Dots::Draw()
{
    m_VAO.Bind();
    glPointSize(1);
    glDrawArrays(GL_POINTS, 0, m_data.size()/6);
    m_VAO.Unbind();
}

void Dots::Delete()
{
    m_VAO.Delete();
    m_VBO.Delete();
}

int Dots::GetSize(){
    return m_data.size()/6;
}
