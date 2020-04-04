#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // DONE: See src/processor.cpp

  // DONE: Declare any necessary private members
 private:
    long totalJiffies_;
    long idleJiffies_;
};

#endif