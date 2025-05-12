#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H


#include <vector>

#include "Circle.h"
#include "Shader.h"
#include "Background.h"
#include "Events.h"
#include "Dots.h"
#include "SSBO.h"
#include "GUI.h"

class Renderer{
    public:
        Renderer();
        ~Renderer();

        bool InitRenderer();
        void Render(GUI* gui, GLFWwindow* window);
        void CreateObject(int index);
        void UpdateObject(int index);
        void DeleteObject(int index);
        void CreateParticle(int index);
        void UpdateParticle(int index);
        void DeleteParticle(int index);
        void UpdateAllParticle();
        void UpdateAll();
        void OnEntityCreated(const Event& event);
        void OnEntityUpdated(const Event& event);
        void OnEntityDeleted(const Event& event);
        void OnParticleCreated(const Event& event);
        void OnParticleUpdated(const Event& event);
        void OnParticleAllUpdated(const Event& event);
        void OnAllUpdated(const Event& event);
        void OnParticleDeleted(const Event& event);

    private:
        Background m_background;
        std::vector<Circle> m_objects;
        Dots *particles;
        Shader *m_defaultShader;
        Shader *m_particleShader;
        Shader *m_SSBOparticleShader;
};

#endif
