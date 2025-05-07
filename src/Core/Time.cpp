#include "Time.h"
#include "Settings.h"
#include <iostream>


double Time::TotalRealTime = 0;
double Time::TotalSimTime = 0;
double Time::deltaTime = 0;
double Time::HardwareLastCheck = 0;
double Time::LastAnalyticUpdate = 0;
double Time::EventProcessingTime = 0;
double Time::RenderingTime = 0;
double Time::IntegratorTime = 0;
double Time::PhysicsEngineTime = 0;
int Time::FPS = 0;

Time::Time() : m_currentTime(0), m_lastTime(0){}

Time::~Time(){}

bool Time::InitTime(){
    m_lastTime = glfwGetTime();
    return true;
} 

void Time::Update(){
    m_currentTime = glfwGetTime();
    if(Settings::time_mode == TimeMode::REAL_TIME){
        deltaTime = m_currentTime - m_lastTime;
    }
    if(Settings::state == State::RUNNING){
        TotalRealTime += m_currentTime - m_lastTime;
        TotalSimTime += deltaTime;
        FPS = 1/(m_currentTime - m_lastTime);
    }
    m_lastTime = m_currentTime;
}

bool Time::ShouldHardwareCheck(){
    double delta = glfwGetTime() - HardwareLastCheck;
    if(delta < 1){return false;}
    else{
        HardwareLastCheck = glfwGetTime();
        return true;
    }
}

bool Time::ShouldUpdateAnalytics(){
    double delta = glfwGetTime() - LastAnalyticUpdate;
    if(delta < 0.25){return false;}
    else{
        LastAnalyticUpdate = glfwGetTime();
        return true;
    }
}

std::string Time::FormatTime(double seconds){
    int totalSeconds = static_cast<int>(seconds);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int secs = totalSeconds % 60;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, secs);
    return std::string(buffer);
}
