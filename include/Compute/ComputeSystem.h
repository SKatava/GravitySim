#ifndef COMPUTESYSTEM_CLASS_H
#define COMPUTESYSTEM_CLASS_H

#include "SSBO.h"
#include "ComputeShader.h"
#include "Entities.h"
#include "BHTree.h"

class ComputeSystem{
    public:
        ComputeSystem();
        ~ComputeSystem();

        bool InitComputeSystem(const char* computeFile);
        void Run();
        void WriteToSSBO();
        void WriteToSSBOExtra(BHTree tree);
        void ReadFromSSBO();
        void ReadFromSSBOExtra();
        SSBO* GetSSBO();

    private:
        SSBO* m_ssbo;
        SSBO* m_ssboExtra;
        ComputeShader* m_compShader;
};

#endif
