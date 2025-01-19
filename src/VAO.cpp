#include "VAO.h"

// generates VAO
VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}
 
// selects VAO(binds)
void VAO::Bind()
{
    glBindVertexArray(ID);
}


// links VAO with defined VBO
void VAO::LinkVBO(VBO &VBO, GLuint layout, GLint size, GLenum type, GLsizeiptr stride, void* offset)
{
    VBO.Bind();
    glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}


// unbinds VAO
void VAO::Unbind()
{
    glBindVertexArray(0);
}

// deletes VAO
void VAO::Delete()
{
    glDeleteVertexArrays(1, &ID);
}
