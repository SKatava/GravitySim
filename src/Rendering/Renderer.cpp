#include "Renderer.h"
#include "Entities.h"
#include "Settings.h"


Renderer::Renderer(){
}

Renderer::~Renderer(){
    m_defaultShader->Delete();
    m_particleShader->Delete();
    m_SSBOparticleShader->Delete();
    delete m_defaultShader;
    delete m_particleShader;
    delete m_SSBOparticleShader;
    m_background.Delete();
    for(auto& object : m_objects){
        object.Delete();
    }

}

bool Renderer::InitRenderer(){
    glViewport(460, 205, 800, 800);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_MULTISAMPLE);


    m_defaultShader = new Shader("assets/shaders/default.vert", "assets/shaders/default.frag");
    m_particleShader = new Shader("assets/shaders/particle.vert", "assets/shaders/particle.frag");
    m_SSBOparticleShader = new Shader("assets/shaders/particleSSBO.vert", "assets/shaders/particleSSBO.frag");


    particles = new Dots();
    m_background.SetUp(800, 800);
    Events::GetInstance().RegisterListener(EventType::ENTITY_CREATED,[this](const Event& event) {this->OnEntityCreated(event);} );
    Events::GetInstance().RegisterListener(EventType::ENTITY_UPDATED,[this](const Event& event) {this->OnEntityUpdated(event);} );
    Events::GetInstance().RegisterListener(EventType::ENTITY_DELETED,[this](const Event& event) {this->OnEntityDeleted(event);} );
    Events::GetInstance().RegisterListener(EventType::PARTICLE_CREATED,[this](const Event& event) {this->OnParticleCreated(event);} );
    Events::GetInstance().RegisterListener(EventType::PARTICLE_UPDATED,[this](const Event& event) {this->OnParticleUpdated(event);} );
    Events::GetInstance().RegisterListener(EventType::PARTICLE_ALL_UPDATED,[this](const Event& event) {this->OnParticleAllUpdated(event);} );
    Events::GetInstance().RegisterListener(EventType::PARTICLE_DELETED,[this](const Event& event) {this->OnParticleDeleted(event);} );
    Events::GetInstance().RegisterListener(EventType::ALL_UPDATED,[this](const Event& event) {this->OnAllUpdated(event);} );
    return true;  
}

void Renderer::CreateObject(int index){
    float x = (Entities::entities[index].GetPosition().x / 400) - 1.f; 
    float y = 1.f - (Entities::entities[index].GetPosition().y / 400); 
    m_objects.push_back(Circle());
    m_objects.back().UpdatePos(Vector2f(x, y));
    m_objects.back().UpdateRadius(Entities::entities[index].GetRadius());
    m_objects.back().SetColor(Entities::entities[index].GetColor());
    m_objects.back().Update();
}

void Renderer::UpdateObject(int index){
    float x = (Entities::entities[index].GetPosition().x / 400) - 1.f; 
    float y = 1.f - (Entities::entities[index].GetPosition().y / 400); 

    m_objects[index].UpdatePos(Vector2f(x, y));
    m_objects[index].UpdateRadius(Entities::entities[index].GetRadius());
    m_objects[index].Update(); 
}

void Renderer::DeleteObject(int index){
    m_objects.erase(m_objects.begin() + index); 
}

void Renderer::CreateParticle(int index){
    float x = (Entities::entities[index].GetPosition().x / 400) - 1.f; 
    float y = 1.f - (Entities::entities[index].GetPosition().y / 400); 
    particles->AddDot(Vector2f(x, y), Entities::entities[index].GetColor());
    particles->Update();
}

void Renderer::UpdateParticle(int index){
    float x = (Entities::entities[index].GetPosition().x / 400) - 1.f; 
    float y = 1.f - (Entities::entities[index].GetPosition().y / 400); 
    particles->UpdatePos(Vector2f(x, y), index);
    particles->Update();
}

void Renderer::UpdateAllParticle(){
    float x, y;
    for(int i = 0; i < particles->GetSize(); i++){
        if(Entities::entities[i].GetType() != EntityType::PARTICLE) continue;
        x = (Entities::entities[i].GetPosition().x / 400) - 1.f; 
        y = 1.f - (Entities::entities[i].GetPosition().y / 400); 
        particles->UpdatePos(Vector2f(x, y), i);
        particles->Update(); 
    }
}

void Renderer::DeleteParticle(int index){
    particles->DeleteDot(index);
}

void Renderer::UpdateAll(){
    float x, y;
    int ParticleCount = 0;
    int ObjectCount = 0;
    for(int i = 0; i < Entities::entities.size(); i++){
        if(Entities::entities[i].GetType() == EntityType::PARTICLE && ParticleCount < particles->GetSize()){
            x = (Entities::entities[i].GetPosition().x / 400) - 1.f; 
            y = 1.f - (Entities::entities[i].GetPosition().y / 400); 
            particles->UpdatePos(Vector2f(x, y), ParticleCount);
            particles->Update();
            ParticleCount++;
        }
        else if(Entities::entities[i].GetType() == EntityType::OBJECT && ObjectCount < m_objects.size()){
            x = (Entities::entities[i].GetPosition().x / 400) - 1.f; 
            y = 1.f - (Entities::entities[i].GetPosition().y / 400); 
            m_objects[ObjectCount].UpdatePos(Vector2f(x, y));
            m_objects[ObjectCount].Update(); 
            ObjectCount++;
        }
    }
}

void Renderer::OnEntityCreated(const Event& event){
    CreateObject(event.ID);
}

void Renderer::OnEntityUpdated(const Event& event){
    UpdateObject(event.ID);
}

void Renderer::OnEntityDeleted(const Event& event){
    DeleteObject(event.ID);
}

void Renderer::OnParticleCreated(const Event& event){
    CreateParticle(event.ID);
}

void Renderer::OnParticleUpdated(const Event& event){
    UpdateParticle(event.ID);
}

void Renderer::OnParticleAllUpdated(const Event& event){
    UpdateAllParticle();
}

void Renderer::OnParticleDeleted(const Event& event){
    DeleteParticle(event.ID);
}

void Renderer::OnAllUpdated(const Event& event){
    UpdateAll();
}

void Renderer::Render(GUI* gui, GLFWwindow* window){
    glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
    m_defaultShader->Activate();
    m_background.Draw();

    if(Settings::mode == Mode::GPU_ACCELERATION || Settings::mode == Mode::ULTRA){
        m_SSBOparticleShader->Activate();
        GLuint dummyVAO;
        glGenVertexArrays(1, &dummyVAO);
        glBindVertexArray(dummyVAO);
        glPointSize(1);
        glDrawArrays(GL_POINTS, 0, Entities::entities.size());
    }
    else if(Settings::mode == Mode::NORMAL || Settings::mode == Mode::CPU_MULTITHREAD){
        for(auto& object : m_objects){
            object.Draw();
        }
        m_particleShader->Activate();
        particles->Draw();
    }
    gui->Render();
}
