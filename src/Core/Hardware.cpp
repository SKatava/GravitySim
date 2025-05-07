#include "Hardware.h"
#include "Time.h"
#include "Settings.h"


std::deque<float> Hardware::cpuHistory;
std::string Hardware::cpuModel = "";
std::string Hardware::cpuCores = "";
std::string Hardware::cpuThreads = "";
std::string Hardware::cpuFrequency = "";

bool Hardware::Init(){
    cpuModel = GetCPUModel();
    cpuCores = GetNumOfCPUCores();
    cpuThreads = GetNmuOfCpuThreads();
    cpuFrequency = GetCPUFrequency();
    return true;
}

#ifdef __linux__
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <sstream>

void Hardware::GetCPUUsage(){
    if (!Time::ShouldHardwareCheck()) return;
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

    if (cpuHistory.size() >= 10) cpuHistory.pop_front();
    cpuHistory.push_back(cpuPercent <= 100 ? cpuPercent : 100);
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


