#include "Application.h"
#include "Settings.h"
#include "Hardware.h"
#include <iostream>
#include <chrono>

//STB header is defined here because it needs to be defined only once
#ifndef STB_IMAGE_IMPLEMENTATION  
#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"
#endif

Application::Application() : window(nullptr){
    entities = new Entities();
    renderer = new Renderer();
    physics = new PhysicsEngine();
    time = new Timer();
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	window = glfwCreateWindow(1720, 1040, "GravitySim", NULL, NULL);

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

void Application::InitIcon(){
    GLFWimage images[1];  

    int width, height, channels;
    unsigned char* pixels = stbi_load("assets/icons/icon.png", &width, &height, &channels, 4);
    
    if (pixels) {
        images[0].width = width;
        images[0].height = height;
        images[0].pixels = pixels;

        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(pixels);
    } else {
        std::cerr << "Failed to load icon!" << std::endl;
    }
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
        std::chrono::duration<float> duration = end - start;
        
        Timer::EventProcessingTime = duration.count();

        time->Update();

        Hardware::GetCPUUsage();

        start = std::chrono::high_resolution_clock::now();
        physics->Step();
        end = std::chrono::high_resolution_clock::now();
        duration = end-start;
        Timer::PhysicsEngineTime = duration.count();

        start = std::chrono::high_resolution_clock::now();
        entities->UpdateEntities();
        end = std::chrono::high_resolution_clock::now();
        duration = end-start;
        Timer::IntegratorTime = duration.count();
        
        start = std::chrono::high_resolution_clock::now();
        renderer->Render(gui, window);
        end = std::chrono::high_resolution_clock::now();
        duration = end-start;
        Timer::RenderingTime = duration.count();
        glfwSwapBuffers(window);
    }
    return;
}
