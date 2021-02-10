#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;


// Class for Processes
class Process {
 public:
  Process(int pid, string user, string command, string uid);
  int Pid() const;               // Returns PID
  std::string User() const;      // Returns User
  std::string Command() const;   // Returns Command
  float CpuUtilization() const;  // Returns CPUUtilization
  std::string Ram() const;       // Returns Ram
  long UpTime() const;           // Returns UpTime

 private:
  int pid{0};
  std::string user{};
  std::string command{};
  float cpuutil{0};
  float prevcpuutil{0};
  long prevUptime{0};
  string uid{};
  string ram{};
  long uptime{0};
};

namespace ProcessSort {
bool ComparePID(const Process a, const Process b);  // Used for sorting
bool ComparexPID(const Process a,
                 const Process b);  // Used for sorting
bool CompareUSER(const Process a,
                 const Process b);  // Used for sorting
bool ComparexUSER(const Process a,
                  const Process b);                  // Used for sorting
bool CompareCPU(const Process a, const Process b);  // Used for sorting
bool ComparexCPU(const Process a,
                 const Process b);  // Used for sorting
bool CompareTIME(const Process a,
                 const Process b);  // Used for sorting
bool ComparexTIME(const Process a,
                  const Process b);                  // Used for sorting
bool CompareRAM(const Process a, const Process b);  // Used for sorting
bool ComparexRAM(const Process a,
                 const Process b);                   // Used for sorting
bool CompareCOM(const Process a, const Process b);  // Used for sorting
bool ComparexCOM(const Process a,
                 const Process b);  // Used for sorting
}

#endif