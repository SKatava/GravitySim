#ifndef GUI_NAMESPACE_H
#define GUI_NAMESPACE_H

#include <imgui.h> 
#include <imgui_impl_glfw.h> 
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "ObjectManager.h"
#include "globals.h"

namespace GUI{
    extern int selected;

    extern int new_mass;
    extern int new_posX;
    extern int new_posY;
    extern float new_velX;
    extern float new_velY;
    extern float DT;

    void Initialize(GLFWwindow* window);
    void Draw();
    void InfoGraphics();
    void ObjectList();
    void TimeInfo();
    void StopButton();
    void ClearButton();
    void ObjectCreator();
    void PositionPlot();
    void ForcePlot();

    void Delete();
};

#endif