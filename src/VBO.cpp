#include "VBO.h"

// generates VBO
VBO::VBO()
{
    glGenBuffers(1, &ID);
}

// binds VBO
void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// binds data to VBO
void VBO::BindData(GLfloat* vertices, GLsizeiptr size)
{
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}


//unbinds VBO
void VBO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// deletes VBO
void VBO::Delete()
{
    glDeleteBuffers(1, &ID);
}


