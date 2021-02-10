#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
// Sorting is done external to this method
// Build a vector of processes read in using LinuxParser methods
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  // Create process objects by reading LinuxParser data
  // Push objects onto vector
  for (unsigned int i = 0; i < pids.size(); i++) {
    int pid = pids[i];
    string user = LinuxParser::User(pids[i]);
    string command = LinuxParser::Command(pids[i]);
    if (command == ""){
      command = " ";
    }
    string uid = LinuxParser::Uid(pids[i]);
    Process process = Process(pid, user, command, uid);
    processes_.push_back(process);
  }

  return processes_;
}

// Return the system's kernel identifier
std::string System::Kernel() {
  if (osvalues.osKernelFlag == true) {
    return osvalues.kernelName;
  } else {
    osvalues.kernelName = LinuxParser::Kernel();
    osvalues.osKernelFlag = true;
    return osvalues.kernelName;
  }
}

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() {
  if (osvalues.osNameFlag == true) {
    return osvalues.osName;
  } else {
    osvalues.osName = LinuxParser::OperatingSystem();
    osvalues.osNameFlag = true;
    return osvalues.osName;
  }
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }