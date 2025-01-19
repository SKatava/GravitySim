#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include <glad.h>
#include <GLFW/glfw3.h>

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

    private:
    GLFWwindow* window;
    Background bg;
};

#endif