/*
 * A simple benchmark.
 * It requires C++11.
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
  size_t timeSize; // Just initial
  void takeTime();
  
protected:
  std::vector<std::chrono::high_resolution_clock::time_point> times;
  bool stopped;

public:
  static constexpr std::chrono::microseconds ZEROMS = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::duration::zero());
  
  SimpleBenchmark(size_t tSize = INIT_TIMES_SIZE);
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
