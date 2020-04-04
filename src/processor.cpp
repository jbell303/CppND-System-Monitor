#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {}

// DONE: Return the aggregate CPU utilization
// cpu utilization = delta active jiffies / delta total jiffies
float Processor::Utilization() { 
    long total =  LinuxParser::Jiffies();
    long active = LinuxParser::ActiveJiffies();

    long delta_t = total - totalJiffies_;
    long delta_a = active - activeJiffies_;

    totalJiffies_ = total;
    activeJiffies_ = active;

    return (float) (delta_a) / (float) (delta_t);
}