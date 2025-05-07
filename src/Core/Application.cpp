#include "Application.h"
#include "Settings.h"
#include "Hardware.h"
#include <iostream>
#include <chrono>

Application::Application() : window(nullptr){
    entities = new Entities();
    renderer = new Renderer();
    physics = new PhysicsEngine();
    time = new Time();
    gui = new GUI(); 
}

Application::~Application(){
    if(window){
        glfwDestroyWindow(window);
    }
    glfwTerminate();

    delete entities;
    delete renderer;
    delete physics;
    delete time;
    delete gui;
}

bool Application::InitWindow(){
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(1720, 1000, "GravitySim", NULL, NULL);

	if (window == NULL){
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}
    glfwSwapInterval(0);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    Settings::mode = Mode::NORMAL;
    Settings::time_mode = TimeMode::REAL_TIME;
    Settings::state = State::STOPPED;
   
    Hardware::Init();
    renderer->InitRenderer();
    entities->InitEntities();
    time->InitTime();
    physics->InitPhysicsEngine();
    gui->InitGUI(window);
    return true;
}

void Application::Run(){
    if(!InitWindow()){
        return;
    }
    MainLoop();
}

void Application::MainLoop(){
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        auto start = std::chrono::high_resolution_clock::now();
        Events::GetInstance().ProcessEvents();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        
        Time::EventProcessingTime = duration.count();

        time->Update();

        Hardware::GetCPUUsage();

        start = std::chrono::high_resolution_clock::now();
        physics->Step();
        end = std::chrono::high_resolution_clock::now();
        duration = end-start;
        Time::PhysicsEngineTime = duration.count();

        start = std::chrono::high_resolution_clock::now();
        entities->UpdateEntities();
        end = std::chrono::high_resolution_clock::now();
        duration = end-start;
        Time::IntegratorTime = duration.count();
        
        start = std::chrono::high_resolution_clock::now();
        renderer->Render(gui, window);
        end = std::chrono::high_resolution_clock::now();
        duration = end-start;
        Time::RenderingTime = duration.count();

        glfwSwapBuffers(window);
    }
    return;
}
