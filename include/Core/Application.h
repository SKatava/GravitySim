#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include <glad.h>
#include <GLFW/glfw3.h>

#include "Events.h"
#include "Entities.h"
#include "Renderer.h"
#include "PhysicsEngine.h"
#include "Timer.h"
#include "GUI.h"

class Application{
    public:
        Application();
        ~Application();

        void Run();
    private:
        bool InitWindow();
        void InitIcon();
        void MainLoop();
        GLFWwindow* window;
        Renderer* renderer;
        Entities* entities;
        PhysicsEngine* physics;
        Timer* time;
        GUI* gui;
};

#endif
