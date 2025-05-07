#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "glad.h"
#include "VBO.h"

class VAO
{
    public:
    GLuint ID;
    VAO();
    void Bind();
    void LinkVBO(VBO &VBO, GLuint layout, GLint size, GLenum type, GLsizeiptr stride, void* offset);
    void Unbind();
    void Delete();
};

#endif
