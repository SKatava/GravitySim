#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

//Glad
#include <glad.h>

//GLFW include files(glfw native used for geting system info. from WIN32 API)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

//Headers to get WIN32 API
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")


//My include header files
#include "globals.h"
#include "shaderClass.h"
#include "Vector2f.h"
#include "Circle.h"
#include "ObjectManager.h"
#include "Background.h"
#include "GravityEngine.h"
#include "GUI.h"
#include "Arrow.h"



class Window{
    public:
    void Initialize();
    void Draw();
    void Clean();
    bool IsDarkModeEnabled();
    void ApplyDarkTitleBar(GLFWwindow* window, bool enableDark);
    void SetWindowIcon(GLFWwindow* window);

    private:
    GLFWwindow* window;
    Background bg;
};

#endif