#include <string>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) { 
  long hour, min, sec = 0;

  // algorithm from http://www.programmingnotes.org/?p=2062
  hour = time / 3600;
  time %= 3600; // remainder of time will be MM:SS
  min = time / 60;
  time %= 60; // remainder will be SS
  sec = time;

  return LeadingZero(hour) + ":" + LeadingZero(min) + 
    ":" + LeadingZero(sec);
 }

 // insert leading zero if value less than 10
 // else return the value in string format
 string Format::LeadingZero (int t) {
   if (t < 10) {
   return "0" + std::to_string(t);
  }
  return std::to_string(t);
 }