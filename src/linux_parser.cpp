#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memTotal, memFree;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = atof(value.c_str());
        } else if (key == "MemFree:") {
          memFree = atof(value.c_str());
        }
      } 
    }
  }
  return (memTotal - memFree) / memTotal; 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, timestr;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> timestr;
    return round(atol(timestr.c_str())); // round to nearest second
  }
  return 0;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpuUtil = LinuxParser::CpuUtilization();
  long sum = 0;
  for (string jiffy : cpuUtil) {
    long value = atol(jiffy.c_str());
    sum += value;
  }
  return sum; 
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line, utime, stime, cutime, cstime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    string buffer;
    for (int i = 0; i < 13; i++) {
      linestream >> buffer;
    }
    linestream >> utime >> stime >> cutime >> cstime;
    // return the sum of process jiffies
    return atol(utime.c_str()) + atol(stime.c_str()) +
      atol(cutime.c_str()) + atol(cstime.c_str());
  }
  return 0; 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  // get a vector of cpu usage in string format
  vector<string> cpuUtil = LinuxParser::CpuUtilization();
  
  // convert strings to long and store in separate vector
  vector<long> usage = {};
  for (string jiffy : cpuUtil) {
    long value = atol(jiffy.c_str());
    usage.push_back(value);
  }
  // sum of all active processes
  return usage[kUser_] + usage[kNice_] + usage[kSystem_] + usage[kIRQ_] + usage[kSoftIRQ_] +
    usage[kSteal_] + usage[kGuest_] + usage[kGuestNice_];
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  // get a vector of cpu usage in string format
  vector<string> cpuUtil = LinuxParser::CpuUtilization();
  
  // convert strings to long and store in separate vector
  vector<long> usage = {};
  for (string jiffy : cpuUtil) {
    long value = atol(jiffy.c_str());
    usage.push_back(value);
  }
  // sum of all idle processes
  return usage[kIdle_] + usage[kIOwait_];
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  // create a vector of strings representing CPU usage
  vector<string> cpuUtil;
  string cpu, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string buffer;
    while (linestream >> buffer) {
      cpuUtil.push_back(buffer);
    }
    return cpuUtil;
  }
  return {};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes")
          return atoi(value.c_str());
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running")
          return atoi(value.c_str());
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);;
    return line;
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return value;
        }
      }
    }
  }
  return string();  
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user, x, pid_value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> pid_value) {
        if (std::to_string(pid) == pid_value) {
          return user;
        }
      }
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, buffer;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++) { // (22) stime
      linestream >> buffer;
    }
    // divide ticks by cpu speed
    long stime = atol(buffer.c_str()) / sysconf(_SC_CLK_TCK); 

    // process uptime = system uptime - process start time 
    return LinuxParser::UpTime() - stime;
  }
  return 0; 
  }