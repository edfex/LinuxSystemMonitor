#include "ncurses_display.h"

#include <curses.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "format.h"
#include "system.h"

using std::string;
using std::to_string;
using namespace ProcessSort;

// 50 bars uniformly displayed from 0 - 100 %, 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  // Write out all the data on the window
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.Cpu().Utilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
      ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n, int* sortOrder) {
  // Set row and column values
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{18};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};

  enum sortOrders {
    PID = 0,
    xPID,
    USER,
    xUSER,
    CPU,
    xCPU,
    RAM,
    xRAM,
    TIME,
    xTIME,
    COM,
    xCOM
  };

  // Check for mouse events
  MEVENT z;
  int c = wgetch(window);
  if (c == KEY_MOUSE) {
    // If mouse event, see if mouse in is important coordinates
    if (getmouse(&z) == OK) {
      if (z.y == 10) {
        // If yes, change sort order
        if (z.x >= pid_column && z.x < user_column) {
          if (*sortOrder == PID) {
            *sortOrder = xPID;
          } else {
            *sortOrder = PID;
          }
        } else if (z.x >= user_column && z.x < cpu_column) {
          if (*sortOrder == USER) {
            *sortOrder = xUSER;
          } else {
            *sortOrder = USER;
          }
        } else if (z.x >= cpu_column && z.x < ram_column) {
          if (*sortOrder == CPU) {
            *sortOrder = xCPU;
          } else {
            *sortOrder = CPU;
          }
        } else if (z.x >= ram_column && z.x < time_column) {
          if (*sortOrder == RAM) {
            *sortOrder = xRAM;
          } else {
            *sortOrder = RAM;
          }
        } else if (z.x >= time_column && z.x < command_column) {
          if (*sortOrder == TIME) {
            *sortOrder = xTIME;
          } else {
            *sortOrder = TIME;
          }
        } else if (z.x >= command_column && z.x < command_column + 9) {
          if (*sortOrder == COM) {
            *sortOrder = xCOM;
          } else {
            *sortOrder = COM;
          }
        }
      }
    }
  }
  // Sort processes vector according to selected Sort Order
  switch (*sortOrder) {
    case 0:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparePID);
      break;
    case 1:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparexPID);
      break;
    case 2:
      std::sort(processes.begin(), processes.end(), ProcessSort::CompareUSER);
      break;
    case 3:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparexUSER);
      break;
    case 4:
      std::sort(processes.begin(), processes.end(), ProcessSort::CompareCPU);
      break;
    case 5:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparexCPU);
      break;
    case 6:
      std::sort(processes.begin(), processes.end(), ProcessSort::CompareRAM);
      break;
    case 7:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparexRAM);
      break;
    case 8:
      std::sort(processes.begin(), processes.end(), ProcessSort::CompareTIME);
      break;
    case 9:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparexTIME);
      break;
    case 10:
      std::sort(processes.begin(), processes.end(), ProcessSort::CompareCOM);
      break;
    case 11:
      std::sort(processes.begin(), processes.end(), ProcessSort::ComparexCOM);
      break;
    default:
      break;
  }

  werase(window);
  box(window, 0, 0);
  // Write out all the data on the window
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  for (int i = 0; i < n; ++i) {
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  // DEBUG THIS FIGURE OUT CONSTRUCTOR WIZARDRY
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c (now you tell me)
  start_color();  // enable color
  mousemask(BUTTON1_CLICKED, NULL);  // enable mouse events

  int x_max{
      getmaxx(stdscr)};  // stdscr is default window the size of the terminal

  // Create System window, use stdscr x_max
  WINDOW* system_window = newwin(9, x_max - 1, 0, 0);

  // Creates Process window, uses n (n = 10) in here
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);
  nodelay(process_window,
          TRUE);  // Allows for window refresh without waiting for input
  keypad(process_window,
         TRUE);  // Enables receipt of key events (including mouse)
  int sortOrder =
      4;  // Value used for sorting Processes; default is CPU Utilization

  // Infinite loop
  while (1) {
    // Color Pair 1, Foreground Color, Background Color
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    // Create them white boxes bro
    box(system_window, 0, 0);
    // Display the System window
    DisplaySystem(system, system_window);
    // Display Processes window, processes vector + no. displayed processes
    DisplayProcesses(system.Processes(), process_window, n, &sortOrder);
    // Refresh windows with data
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    // Loop every second
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}