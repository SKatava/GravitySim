#ifndef APPLICATION_CLASS_H
#define APPLICATION_CLASS_H

#include <glad.h>
#include <GLFW/glfw3.h>

#include "Events.h"
#include "Entities.h"
#include "Renderer.h"
#include "PhysicsEngine.h"
#include "Time.h"
#include "GUI.h"

class Application{
    public:
        Application();
        ~Application();

        void Run();
        GLFWwindow* GetWindow();
    private:
        bool InitWindow();
        void MainLoop();
        GLFWwindow* window;
        Renderer* renderer;
        Entities* entities;
        PhysicsEngine* physics;
        Time* time;
        GUI* gui;
};

#endif
