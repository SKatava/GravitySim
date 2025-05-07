#ifndef COMPUTESHADER_CLASS_H
#define COMPUTESHADER_CLASS_H

#include <glad.h>
#include <iostream>

std::string get_contents(const char *filename);

class ComputeShader{
    public:
        GLuint ID;
        ComputeShader(const char* computeFile);
        void Activate();
        void Delete();
    private:
        void compileErrors(unsigned int shader, const char* type);
};
#endif
