#include "ComputeSystem.h"
#include "Timer.h"

ComputeSystem::ComputeSystem(){}

ComputeSystem::~ComputeSystem(){
    m_compShader->Delete();
    m_ssbo->Delete();
    delete m_compShader; 
    delete m_ssbo;
}

bool ComputeSystem::InitComputeSystem(const char* computeFile){
    m_ssbo = new SSBO();
    m_compShader = new ComputeShader(computeFile);
    m_ssbo->BindBase(0);
    return true;
}



SSBO* ComputeSystem::GetSSBO(){
    return m_ssbo;
}

void ComputeSystem::WriteToSSBO(){
    std::vector<EntityData> buffer(Entities::entities.size());
    for(size_t i = 0; i < Entities::entities.size(); i++){
        buffer[i] = *Entities::entities[i].GetData();
    }
    m_ssbo->Bind();
    m_ssbo->BindData(buffer.data(), sizeof(EntityData) * buffer.size(), GL_DYNAMIC_DRAW);
    m_ssbo->Unbind();
}


void ComputeSystem::ReadFromSSBO(){
    m_ssbo->Bind();
    EntityData* data = (EntityData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, Entities::entities.size() * sizeof(EntityData), GL_MAP_READ_BIT);
    for (size_t i = 0; i < Entities::entities.size(); i++) {
        *Entities::entities[i].GetData() = data[i];
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); 
    m_ssbo->Unbind();
}


void ComputeSystem::Run(){
    m_ssbo->Bind();
    m_compShader->Activate();
   
    GLint dtLocation = glGetUniformLocation(m_compShader->ID, "deltaTime");
    glUniform1f(dtLocation, Timer::deltaTime);   
    GLuint numGroups = (Entities::entities.size() / 64) + 1;
    glDispatchCompute(numGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
   
    m_ssbo->Unbind();
}   
