#ifndef PROCESSOR_H
#define PROCESSOR_H

// Class for CPU information
class Processor {
 public:
  float Utilization();    // Calculates CPU utilization based on current and previous data

 private:
   float prevCpuUtil{0};  // Holds previous CPU usage time
   float prevCpuIdle{0};  // Holds previous CPU idle time
};

#endif