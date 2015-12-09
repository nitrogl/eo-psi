/*
 * A bucket (vector)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef SIMPLE_BENCHMARK_H
#define SIMPLE_BENCHMARK_H
//-----------------------------------------------------------------------------

#include <vector>
#include <chrono>

#define INIT_TIMES_SIZE 1024
//-----------------------------------------------------------------------------

class SimpleBenchmark {
private:
  void takeTime();
  
protected:
  std::vector<std::chrono::high_resolution_clock::time_point> times;
  bool stopped;

public:
  SimpleBenchmark();
  virtual ~SimpleBenchmark();
  
  void start();
  void step();
  void stop();
  void reset();
  std::chrono::microseconds benchmark(size_t cursor = 0) const;
  std::chrono::microseconds cumulativeBenchmark(size_t cursor = 0) const;
};
//-----------------------------------------------------------------------------

#endif // SIMPLE_BENCHMARK_H
