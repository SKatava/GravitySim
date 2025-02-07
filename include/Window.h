#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <glad.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")



#include "globals.h"
#include "shaderClass.h"
#include "Vector2f.h"
#include "Circle.h"
#include "ObjectManager.h"
#include "Background.h"
#include "GravityEngine.h"
#include "GUI.h"



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