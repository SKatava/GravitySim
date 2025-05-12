#include "Timer.h"
#include "Settings.h"
#include <iostream>


float Timer::TotalRealTime = 0;
float Timer::TotalSimTime = 0;
float Timer::deltaTime = 0;
float Timer::HardwareLastCheck = 0;
float Timer::LastAnalyticUpdate = 0;
float Timer::EventProcessingTime = 0;
float Timer::RenderingTime = 0;
float Timer::IntegratorTime = 0;
float Timer::PhysicsEngineTime = 0;
int Timer::FPS = 0;

Timer::Timer() : m_currentTime(0), m_lastTime(0){}

Timer::~Timer(){}

bool Timer::InitTime(){
    m_lastTime = glfwGetTime();
    return true;
} 

void Timer::Update(){
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

bool Timer::ShouldHardwareCheck(){
    float delta = glfwGetTime() - HardwareLastCheck;
    if(delta < 1){return false;}
    else{
        HardwareLastCheck = glfwGetTime();
        return true;
    }
}

bool Timer::ShouldUpdateAnalytics(){
    float delta = glfwGetTime() - LastAnalyticUpdate;
    if(delta < 0.25){return false;}
    else{
        LastAnalyticUpdate = glfwGetTime();
        return true;
    }
}

std::string Timer::FormatTime(float seconds){
    int totalSeconds = static_cast<int>(seconds);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int secs = totalSeconds % 60;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, secs);
    return std::string(buffer);
}
