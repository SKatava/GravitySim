#ifndef COMPUTESYSTEM_CLASS_H
#define COMPUTESYSTEM_CLASS_H

#include "SSBO.h"
#include "ComputeShader.h"
#include "Entities.h"

class ComputeSystem{
    public:
        ComputeSystem();
        ~ComputeSystem();

        bool InitComputeSystem(const char* computeFile);
        void Run();
        void WriteToSSBO();
        void ReadFromSSBO();
        SSBO* GetSSBO();

    private:
        int nodeCount;
        SSBO* m_ssbo;
        ComputeShader* m_compShader;
};

#endif
