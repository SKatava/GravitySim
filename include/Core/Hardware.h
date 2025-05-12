#ifndef HARDWARE_NAMESPACE_H
#define HARDWARE_NAMESPACE_H

#include <string>
#include <deque>

namespace Hardware {
    extern float cpuHistory[10];
    extern std::string cpuModel;
    extern std::string cpuCores;
    extern std::string cpuThreads;
    extern std::string cpuFrequency;
    bool Init();
    void GetCPUUsage(); 
    std::string GetNumOfCPUCores();
    std::string GetCPUModel();
    std::string GetNmuOfCpuThreads();
    std::string GetCPUFrequency();
};

#endif
