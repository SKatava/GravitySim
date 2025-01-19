#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "glad.h"

class VBO
{
    public:
    GLuint ID;
    VBO();
    void Bind();
    void BindData(GLfloat* vertices, GLsizeiptr size);
    void Unbind();
    void Delete();
};

#endif