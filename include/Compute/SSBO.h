#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <glad.h>

class SSBO{
    public:
        GLuint ID;
        SSBO();
        void Bind();
        void BindData(void* data, GLsizeiptr size, GLenum usage);
        void BindBase(GLuint bindingPoint);
        void Unbind();
        void Delete();
};

#endif
