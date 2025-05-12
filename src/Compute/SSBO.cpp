#include "SSBO.h"

SSBO::SSBO(){
    glGenBuffers(1, &ID);
}

void SSBO::Bind(){
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

void SSBO::BindData(void* data, GLsizeiptr size, GLenum usage){
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
}

void SSBO::BindBase(GLuint bindingPoint)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, ID);
}

void SSBO::Unbind(){
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBO::Delete(){
    glDeleteBuffers(1, &ID);
}

