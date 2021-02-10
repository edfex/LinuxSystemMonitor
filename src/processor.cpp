#include "processor.h"
#include "linux_parser.h"

using std::stof;
using LinuxParser::CPUStates;

// Return the aggregate CPU utilization
float Processor::Utilization() {
    // Get current cpu values
    std::vector<std::string> cpu = LinuxParser::CpuUtilization();
    float curCpuUtil = stof(cpu[0]) + stof(cpu[1]) + stof(cpu[2]) + stof(cpu[3]) + stof(cpu[4]) + stof(cpu[5]) + stof(cpu[6]) + stof(cpu[7]);
    float curCpuIdle = stof(cpu[3]) + stof(cpu[4]);
    
    // Calculate differences and %
    float cpuUtil = (curCpuUtil - prevCpuUtil);
    float cpuIdle = (curCpuIdle - prevCpuIdle);
    float cpuPercent = (cpuUtil - cpuIdle) / cpuUtil;

    // Set new prev values
    prevCpuUtil = curCpuUtil;
    prevCpuIdle = curCpuIdle;

    // Return %
    return cpuPercent;
}