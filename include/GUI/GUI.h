#ifndef GUI_CLASS_H
#define GUI_CLASS_H

#include <GLFW/glfw3.h>

#include <imgui.h> 
#include <imgui_impl_glfw.h> 
#include <imgui_impl_opengl3.h>

#include "Entities.h"

class GUI {
    public:
        GUI();
        ~GUI();
        void InitGUI(GLFWwindow* window);
        void CreateContext();
        void Render();
        void TitleBar();
        void EntitiesList();
        void AddEntityBtn();
        void AddEntityForm();
        void DeleteEntityBtn();
        void SearchEntityBtn();
        void EntitySearch();
        void TimeInfo();
        void TimeStateButton();
        void ResetButton();
        void TimeModeButton();
        void AccelerationOptions();
        void UsageAnalytics();
        void CPUInfo();
        void PositionPlot();
        void ForcePlot();
        void TimePlot();
        void EntityAnalytics();
        void PositionPicker();
        void UpdateEntityAnalytics();
        void ResetPositionPlotData();
        void ResetForcePlotData();
        
        static const char* EntityTypes[];
    
    private:
        GLFWwindow* m_window;

        bool CALL_ADDENTITY_FORM;
        bool CALL_ENTITY_SEARCH;

        int m_NewPosX;
        int m_NewPosY;
        float m_NewMass;
        int m_NewSize;
        float m_InitVelocityX;
        float m_InitVelocityY;
        float m_NewColor[3];
        int m_NumToAdd;
        int m_EntityTypeSelected;
        char m_newName[32];
        int m_TableRowSelected;
        int m_EntitySelected;
        float m_PositionPlotDataX[128]; 
        float m_PositionPlotDataY[128]; 
        float m_ForcePlotData[128]; 
        float m_MaxForce;
        float m_NewDeltaTime;
};

#endif
