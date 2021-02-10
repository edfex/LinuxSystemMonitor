#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read and return OS information
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    // Clean up each line
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      // Parse each line, looking for specific key/value
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return Kernel information
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    // Necessary data is on first line, no need for parsing
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> os >> kernel;
  }
  return kernel;
}

// Read and return list of PIDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  // Create pointer to a directory in filesystem
  DIR* directory = opendir(kProcDirectory.c_str());
  // Create pointer to file
  struct dirent* file;
  // Read each of the files in a directory
  while ((file = readdir(directory)) != nullptr) {
    // Checks to see that a file is a directory or not
    if (file->d_type == DT_DIR) {
      // PID files consist of all numbers, check to see if this is a PID file
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        // Get PID from filename and add to vector
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  // Close directory
  closedir(directory);
  return pids;
}

// Read and return Memory Utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  float memUtil = 0;
  float memTotal, memFree;
  float buffers;
  float cache, sreclaim, shmem;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    // Parse each line, looking for specific keys/values
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      value = 0;
      linestream >> key >> value;
      if (key == "MemTotal") memTotal = value;
      if (key == "MemFree") memFree = value;
      if (key == "Buffers") buffers = value;
      if (key == "Cached") cache = value;
      if (key == "Shmem") shmem = value;
      if (key == "SReclaimable") sreclaim = value;
    }
    // Total available memory = memTotal - memFree
    memTotal = memTotal - memFree;
    // Total used memory = totalAvail - (Buffers + (Cached + Sreclaimable -
    // Shmem)))
    memUtil = memTotal - (buffers + (cache + sreclaim - shmem));
    // Turn into percentage
    memUtil = memUtil / memTotal;

    return memUtil;
  }
  return memUtil;
}

// Read and return CPU UpTime
long LinuxParser::UpTime() {
  string line;
  double uptime;
  double idletime;
  long upTimeValue = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    // Data is on first line, no need to parse
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    upTimeValue = (long)uptime;
  }
  return upTimeValue;
}

// Read and return the process cpu utilization for a PID
float LinuxParser::CpuUtilization(int pid) {
  string line, txt;
  long utime, stime;
  float cpuutil{0};
  string pidS = to_string(pid);
  std::ifstream filestream(kProcDirectory + pidS + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // Get 14th and 15th values
      std::istringstream linestream(line);
      int i = 1;
      while (i != 14) {
        linestream >> txt;
        i++;
      }
      linestream >> utime >> stime;
      cpuutil = utime + stime;
      // Convert into seconds
      cpuutil = cpuutil / sysconf(_SC_CLK_TCK);
      return cpuutil;
    }
  }
  return cpuutil;
}

// Returns current CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string cpu;
  vector<string> value(8, "");
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> value[kUser_] >> value[kNice_] >> value[kSystem_] >>
        value[kIdle_] >> value[kIOwait_] >> value[kIRQ_] >> value[kSoftIRQ_] >>
        value[kSteal_];
  }
  return value;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line{}, command{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command;
  }
  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value;
  float mem{0};
  string pidS = to_string(pid);
  std::ifstream filestream(kProcDirectory + pidS + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "VmSize:") {
          linestream >> mem;
          // Convert from kB to mB
          mem /= 1000;
          value = to_string(mem);
          // Truncate extra decimal points
          value.erase(value.find('.') + 3);
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid, key, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid:") {
          linestream >> uid;
          return uid;
        }
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid, user, txt, key, line;
  string pidS = to_string(pid);
  bool flag{false};
  // Open file, get uid
  std::ifstream filestream(kProcDirectory + pidS + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid:") {
          linestream >> uid;
          flag = true;
          break;
        }
        break;
      }
      if (flag) break;
    }
  }
  // Open new file, use uid to get user string
  filestream.close();
  filestream.open(kPasswordPath);
  if (filestream.is_open()) {
    // Parse file
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      // Find the UID / user pair
      while (linestream >> user >> txt >> key) {
        if (key == uid) {
          return user;
        }
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, key, value;
  string pidS = to_string(pid);
  long uptime{0};
  std::ifstream filestream(kProcDirectory + pidS + kStatFilename);
  if (filestream.is_open()) {
    // Parse file for uptime, value 22 in line
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i = 1;
    while (i != 22) {
      linestream >> key;
      i++;
    }
    linestream >> value;
    uptime = stol(value);
    // Convert from clock ticks to seconds
    uptime = uptime / sysconf(_SC_CLK_TCK);
    // Subtract from system runtime to get process runtime
    long systime = UpTime();
    uptime = systime - uptime;
    return uptime;
  }
  return uptime;
}