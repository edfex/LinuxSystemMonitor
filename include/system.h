#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();  // Creates and returns list of Processes
  float MemoryUtilization();          // Returns system memory utilization %
  long UpTime();                      // Returns system uptime in seconds
  int TotalProcesses();               // Returns total number of processes
  int RunningProcesses();         // Returns total number of running processes
  std::string Kernel();           // Returns kernel name
  std::string OperatingSystem();  // Returns OS name

 private:
  Processor cpu_ = {};                   // Initialize default CPU obect
  std::vector<Process> processes_ = {};  // Initialize empty processes vector

  struct OSValues {  // Struct for Processor strings and flags
    std::string osName = "";
    bool osNameFlag = false;
    std::string kernelName = "";
    bool osKernelFlag = false;
  };
  OSValues osvalues;  // Create struct of processor strings/flags
};

#endif