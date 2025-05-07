#ifndef TIME_CLASS_H
#define TIME_CLASS_H

#include <GLFW/glfw3.h>

#include <iostream>

class Time{
    public:
        Time();
        ~Time();

        static double TotalRealTime;
        static double TotalSimTime;
        static double deltaTime;
        static double HardwareLastCheck;
        static double LastAnalyticUpdate;
        static double EventProcessingTime;
        static double RenderingTime;
        static double IntegratorTime;
        static double PhysicsEngineTime;
        static int FPS;
        
        bool InitTime();
        void Update();
        static bool ShouldHardwareCheck();
        static bool ShouldUpdateAnalytics();
        static std::string FormatTime(double seconds);

    private:
        double m_currentTime;
        double m_lastTime;
};

#endif
