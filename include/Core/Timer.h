#ifndef TIMER_CLASS_H
#define TIMER_CLASS_H

#include <GLFW/glfw3.h>

#include <iostream>

class Timer{
    public:
        Timer();
        ~Timer();

        static float TotalRealTime;
        static float TotalSimTime;
        static float deltaTime;
        static float HardwareLastCheck;
        static float LastAnalyticUpdate;
        static float EventProcessingTime;
        static float RenderingTime;
        static float IntegratorTime;
        static float PhysicsEngineTime;
        static int FPS;
        
        bool InitTime();
        void Update();
        static bool ShouldHardwareCheck();
        static bool ShouldUpdateAnalytics();
        static std::string FormatTime(float seconds);

    private:
        double m_currentTime;
        double m_lastTime;
};

#endif
