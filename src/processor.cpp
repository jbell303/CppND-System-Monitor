#include "processor.h"
#include "linux_parser.h"

Processor::Processor() : totalJiffies_(0), idleJiffies_(0) {}

// DONE: Return the aggregate CPU utilization
// cpu utilization = delta active jiffies / delta total jiffies
float Processor::Utilization() { 
    long total =  LinuxParser::Jiffies();
    long idle = LinuxParser::IdleJiffies();

    long delta_t = total - totalJiffies_;
    long delta_i = idle - idleJiffies_;

    totalJiffies_ = total;
    idleJiffies_ = idle;

    return (float) (delta_t - delta_i) / (float) (delta_t);
}