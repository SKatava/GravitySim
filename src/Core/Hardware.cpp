#include "Hardware.h"
#include "Timer.h"
#include "Settings.h"


float Hardware::cpuHistory[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
std::string Hardware::cpuModel = "";
std::string Hardware::cpuCores = "";
std::string Hardware::cpuThreads = "";
std::string Hardware::cpuFrequency = "";


//--------------------LINUX------------------------------------------------------------------

#ifdef __linux__
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <sstream>

bool Hardware::Init(){
    cpuModel = GetCPUModel();
    cpuCores = GetNumOfCPUCores();
    cpuThreads = GetNmuOfCpuThreads();
    cpuFrequency = GetCPUFrequency();
    return true;
}

void Hardware::GetCPUUsage(){
    if (!Timer::ShouldHardwareCheck()) return;
    static auto lastTime = std::chrono::high_resolution_clock::now();
    static long lastProcTime = 0;
    static long lastTotalTime = 0;
    static bool initialized = false;

    std::ifstream statFile("/proc/self/stat");
    std::string token;
    long utime, stime;
    for (int i = 0; i < 13; ++i) statFile >> token;
    statFile >> utime >> stime;
    long procTime = utime + stime;

    std::ifstream cpuFile("/proc/stat");
    std::getline(cpuFile, token); 
    std::istringstream cpuLine(token);
    cpuLine >> token; 

    long user, nice, system, idle, iowait, irq, softirq, steal;
    cpuLine >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    auto now = std::chrono::high_resolution_clock::now();
    float elapsed = std::chrono::duration<float>(now - lastTime).count();

    if (!initialized) {
        lastTime = now;
        lastProcTime = procTime;
        lastTotalTime = totalTime;
        initialized = true;
        return;
    }

    long deltaProc = procTime - lastProcTime;
    long deltaTotal = totalTime - lastTotalTime;

    float cpuPercent = 0;
    if (deltaTotal > 0) {
        cpuPercent = (float)deltaProc / deltaTotal * 100.0f;
    }

    lastTime = now;
    lastProcTime = procTime;
    lastTotalTime = totalTime;

    for(int i = 0; i < 10; i++){
        cpuHistory[i-1] = cpuHistory[i];
    }
    cpuHistory[9] = cpuPercent;
    cpuFrequency = GetCPUFrequency();
}


std::string Hardware::GetNumOfCPUCores(){
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return "Failed to open /proc/cpuinfo";
    }

    std::string line;
    int numCores = 0;
    while (std::getline(cpuinfo, line)) {
        if (line.find("cpu cores") != std::string::npos) {
            numCores = std::stoi(line.substr(line.find(":") + 2));
            break;
        }
    }

    cpuinfo.close();
    return std::to_string(numCores);    
}
std::string Hardware::GetCPUModel(){
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return "Failed to open /proc/cpuinfo";
    }

    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            return line.substr(line.find(":") + 2);  
        }
    }

    cpuinfo.close();
    return "CPU model not found";
}
std::string Hardware::GetNmuOfCpuThreads(){
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return "Failed to open /proc/cpuinfo";
    }

    std::string line;
    int numThreads = 0;
    while (std::getline(cpuinfo, line)) {
        if (line.find("siblings") != std::string::npos) {
            numThreads = std::stoi(line.substr(line.find(":") + 2));
            break;
        }
    }

    cpuinfo.close();
    return std::to_string(numThreads);
}

std::string Hardware::GetCPUFrequency() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) return "Failed to open /proc/cpuinfo";

    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            return line.substr(line.find(":") + 2) + " MHz";
        }
    }

    return "CPU frequency not found";
}

#endif

//--------------------WINDOWS------------------------------------------------------------------

#ifdef _WIN32

#include <windows.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "Timer.h"

bool Hardware::Init(){
    cpuModel = GetCPUModel();
    cpuCores = GetNumOfCPUCores();
    cpuThreads = GetNmuOfCpuThreads();
    cpuFrequency = GetCPUFrequency();

    //Windows...
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    
    return true;
}


void Hardware::GetCPUUsage(){
    if(!Timer::ShouldHardwareCheck()){return;}
    static auto lastTime = std::chrono::high_resolution_clock::now();
    static ULONGLONG lastProcTime = 0;
    static ULONGLONG lastTotalTime = 0;
    static bool initialized = false;

    FILETIME ftCreate, ftExit, ftKernel, ftUser;
    GetProcessTimes(GetCurrentProcess(), &ftCreate, &ftExit, &ftKernel, &ftUser);

    ULARGE_INTEGER kernelTime, userTime;
    kernelTime.LowPart = ftKernel.dwLowDateTime;
    kernelTime.HighPart = ftKernel.dwHighDateTime;

    userTime.LowPart = ftUser.dwLowDateTime;
    userTime.HighPart = ftUser.dwHighDateTime;

    ULONGLONG procTime = kernelTime.QuadPart + userTime.QuadPart;

    FILETIME idleTime, kernelSysTime, userSysTime;
    GetSystemTimes(&idleTime, &kernelSysTime, &userSysTime);

    ULARGE_INTEGER kernelSys, userSys;
    kernelSys.LowPart = kernelSysTime.dwLowDateTime;
    kernelSys.HighPart = kernelSysTime.dwHighDateTime;

    userSys.LowPart = userSysTime.dwLowDateTime;
    userSys.HighPart = userSysTime.dwHighDateTime;

    ULONGLONG totalTime = kernelSys.QuadPart + userSys.QuadPart;

    auto now = std::chrono::high_resolution_clock::now();
    float elapsed = std::chrono::duration<float>(now - lastTime).count();

    if (!initialized) {
        lastTime = now;
        lastProcTime = procTime;
        lastTotalTime = totalTime;
        initialized = true;
        return;
    }

    ULONGLONG deltaProc = procTime - lastProcTime;
    ULONGLONG deltaTotal = totalTime - lastTotalTime;

    float cpuPercent = 0;
    if (deltaTotal > 0) {
        cpuPercent = (float)deltaProc / deltaTotal * 100.0f;
    }

    
    for(int i = 0; i < 10; i++){
        cpuHistory[i-1] = cpuHistory[i];
    }
    cpuHistory[9] = cpuPercent;

    lastTime = now;
    lastProcTime = procTime;
    lastTotalTime = totalTime;
}


std::string Hardware::GetNumOfCPUCores(){
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    DWORD bufferSize = 0;
    std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer;

    if (!GetLogicalProcessorInformation(nullptr, &bufferSize) && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));

        if (GetLogicalProcessorInformation(buffer.data(), &bufferSize)) {
            int coreCount = 0;
            for (const auto& processor : buffer) {
                if (processor.Relationship == RelationProcessorCore) {
                    coreCount++;
                }
            }
            std::stringstream ss;
            ss << coreCount;
            return ss.str();
        }
    }

    return "Unknown";
}

std::string Hardware::GetCPUModel(){
    HKEY hKey;
    const char* path = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char model[512];
        DWORD bufSize = sizeof(model);
        if (RegQueryValueExA(hKey, "ProcessorNameString", nullptr, nullptr, (LPBYTE)model, &bufSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::string(model);
        }
        RegCloseKey(hKey);
    }
    return "Unknown CPU Model";
}
std::string Hardware::GetNmuOfCpuThreads(){
    int threadCount = std::thread::hardware_concurrency();
    std::ostringstream oss;
    oss << threadCount;
    return oss.str();
}

std::string Hardware::GetCPUFrequency() {
    return "Unknown";
}
#endif
