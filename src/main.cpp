#include "ncurses_display.h"
#include "system.h"

int main() {
  // Create system object
  System system;
  // Display calls System methods every second to update data
  NCursesDisplay::Display(system);
}