#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Create Process object, populate fields with data
Process::Process(int pid, string user, string command, string uid)
    : pid(pid), user(user), command(command), uid(uid) {
  // Calculate Process Cpu utilization by using old and new values
  float oldUtil = prevcpuutil;
  float oldUptime = prevUptime;
  float newUtil = LinuxParser::CpuUtilization(Pid());
  float newUptime = LinuxParser::UpTime();
  // util = (newutil - oldutil) / (newuptime - olduptime)
  cpuutil = ((newUtil - oldUtil) / (newUptime - oldUptime));
  // Set old values to new values
  prevcpuutil = newUtil;
  prevUptime = newUptime;

  // Calculate Memory Utilization
  ram = LinuxParser::Ram(pid);
  if (ram == ""){
    ram = "0";
  }

  // Calculate Uptime
  uptime = LinuxParser::UpTime(pid);
}

// Return this process's ID
int Process::Pid() const { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuutil; }

// Return the command that generated this process
string Process::Command() const { return command; }

// Return this process's memory utilization
string Process::Ram() const { return ram; }

// Return the user (name) that generated this process
string Process::User() const { return user; }

// Return the age of this process (in seconds)
long Process::UpTime() const { return uptime; }

// Compare methods for sorting to various values
namespace ProcessSort {

bool ComparePID(const Process a, const Process b) {
  auto value1 = a.Pid();
  auto value2 = b.Pid();
  return value1 > value2;
}

bool ComparexPID(const Process a, const Process b) {
  auto value1 = a.Pid();
  auto value2 = b.Pid();
  return value2 > value1;
}

bool CompareUSER(const Process a, const Process b) {
  auto value1 = a.User();
  auto value2 = b.User();
  return value1 > value2;
}

bool ComparexUSER(const Process a, const Process b) {
  auto value1 = a.User();
  auto value2 = b.User();
  return value2 > value1;
}

bool CompareCPU(const Process a, const Process b) {
  auto value1 = a.CpuUtilization();
  auto value2 = b.CpuUtilization();
  return value1 > value2;
}

bool ComparexCPU(const Process a, const Process b) {
  auto value1 = a.CpuUtilization();
  auto value2 = b.CpuUtilization();
  return value2 > value1;
}

bool CompareTIME(const Process a, const Process b) {
  auto value1 = a.UpTime();
  auto value2 = b.UpTime();
  return value1 > value2;
}

bool ComparexTIME(const Process a, const Process b) {
  auto value1 = a.UpTime();
  auto value2 = b.UpTime();
  return value2 > value1;
}

bool CompareRAM(const Process a, const Process b) {
  auto value1 = stof(a.Ram());
  auto value2 = stof(b.Ram());
  return value1 > value2;
}

bool ComparexRAM(const Process a, const Process b) {
  auto value1 = stof(a.Ram());
  auto value2 = stof(b.Ram());
  return value2 > value1;
}

bool CompareCOM(const Process a, const Process b) {
  string value1 = a.Command();
  string value2 = b.Command();
  if (value1.compare(value2) >= 0) {
    return true;
  }
  return false;
}

bool ComparexCOM(const Process a, const Process b) {
  string value1 = a.Command();
  string value2 = b.Command();
  if (value1.compare(value2) < 0) {
    return true;
  }
  return false;
}
}  // namespace ProcessSort