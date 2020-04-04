#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), cpuUtil_(0) {}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    cpuUtil_ = (float) LinuxParser::ActiveJiffies(pid_) / (float) (LinuxParser::Jiffies() - LinuxParser::IdleJiffies());
    return cpuUtil_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    int ramMb = atoi(LinuxParser::Ram(pid_).c_str()) / 1000;
    return to_string(ramMb); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->cpuUtil_ < a.cpuUtil_;
}