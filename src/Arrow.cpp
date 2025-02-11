#include "Arrow.h"

Arrow::Arrow(Vector2f start, Vector2f end) : m_start(start), m_end(end), m_color(Vector3f(1.f, 1.f, 1.f)), m_VBO(), m_VAO() {
    m_VAO.Bind(); 
    m_VBO.Bind();
    m_VAO.LinkVBO(m_VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    m_VAO.LinkVBO(m_VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
    m_VAO.Unbind();
    m_VBO.Unbind();   
}

void Arrow::Update(){
    
    m_VBO.Bind();
    float dx = m_start.x - m_end.x;
    float dy = m_start.y - m_end.y;
    
    float length = std::sqrt(dx * dx + dy*dy);
    
    float ux = dx/length;
    float uy = dy/length; 
    
    float px = -uy;
    float py = ux;

    GLfloat vertices[] = {
        
        m_start.x, m_start.y, 0.f, m_color.x, m_color.y, m_color.z,
        m_end.x, m_end.y, 0.f, m_color.x, m_color.y, m_color.z,

        m_end.x + 0.025f * ux - 0.025f * px, m_end.y + 0.025f * uy - 0.025f * py, 0.f, m_color.x, m_color.y, m_color.z,

        m_end.x + 0.025f * ux + 0.025f * px, m_end.y + 0.025f * uy + 0.025f * py, 0.f, m_color.x, m_color.y, m_color.z,
        m_end.x, m_end.y, 0, m_color.x, m_color.y, m_color.z,

    };
    m_VBO.BindData(vertices, sizeof(vertices));
    m_VBO.Unbind();
}

void Arrow::Draw(){
    m_VAO.Bind();
    glDrawArrays(GL_LINES, 0, 2);
    glDrawArrays(GL_TRIANGLES, 2, 3);
    m_VAO.Unbind();
}

void Arrow::Delete(){
    m_VAO.Delete();
    m_VBO.Delete();
}

void Arrow::SetStart(Vector2f start){
    Vector2f GLpos(0.f, 0.f);
    GLpos.x = (start.x / 500.f) - 1.f;
    GLpos.y = 1.f - (start.y / 500.f);
    m_start = GLpos;
}

void Arrow::SetEnd(Vector2f end){
    Vector2f GLpos(0.f, 0.f);
    GLpos.x = (end.x / 500.f) - 1.f;
    GLpos.y = 1.f - (end.y / 500.f);
    m_end = GLpos;
}

void Arrow::SetColor(float r, float g, float b){
    m_color = Vector3f(r, g, b);
    Update();
}


