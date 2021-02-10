#include "format.h"

#include <string>

using std::string;

string Format::ElapsedTime(long inputValue) {
  long hours, minutes, seconds;
  hours = inputValue / 3600;
  inputValue %= 3600;
  minutes = inputValue / 60;
  inputValue %= 60;
  seconds = inputValue;

  string h, m, s;
  h = std::to_string(hours);
  m = std::to_string(minutes);
  s = std::to_string(seconds);
  // Pad seconds with a 0 if necessary
  if (seconds < 10) {
    s = "0" + s;
  }
  if (minutes < 10) {
    m = "0" + m;
  }
  string time = h + ":" + m + ":" + s;
  return time;
}