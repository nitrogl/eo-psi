/*
 * A simple benchmark.
 * It requires C++11 (-std=c++11 for GNU compilers)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef SIMPLE_BENCHMARK_H
#define SIMPLE_BENCHMARK_H
//-----------------------------------------------------------------------------

#include <vector>
#include <chrono>
#include <unordered_map>

#define INIT_TIMES_SIZE 1024
//-----------------------------------------------------------------------------

/**
 * A simple benchmark to measure the time efficiency of algorithms, methods, and so forth.
 * It is a chronometer with the following functionality:
 * <ul>
 *  <li><strong>start()</strong>. First time taken, it starts the chronometer.</li>
 *  <li><strong>step()</strong>. Intermediate time taken. It creates a time step without stopping the whole chronometer.</li>
 *  <li><strong>stop()</strong>. Last time taken, it stops the chronometer.</li>
 *  <li><strong>reset()</strong>. Reset the chronometer at its initial state.</li>
 * </ul>
 */
class SimpleBenchmark {
private:
  size_t timeSize; ///< It is an initial size to optimise successive calls of step()
  
  /**
   * Take the current time.
   */
  void takeTime(const std::string cursor = "");
  
protected:
  std::vector<std::chrono::high_resolution_clock::time_point> times;             ///< All the times taken.
  bool stopped;                                                                  ///< The stopped state of the chronometer.
  std::unordered_map<std::string, size_t> cursors; ///< Associating strings to cursors
  
  /**
   * Get the numeric cursor from its corresponding string.
   * 
   * @param cursor The name of the cursor saved.
   */
  virtual size_t getNumericCursor(const std::string cursor = "") const;

public:
  static constexpr std::chrono::microseconds ZEROMS = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::duration::zero()); ///< Zero
  
  /**
   * Initialise the simple benchmark.
   * 
   * @param tSize Specify the size to reserve to optimise successive calls of step()
   */
  SimpleBenchmark(const size_t tSize = INIT_TIMES_SIZE);
  virtual ~SimpleBenchmark();
  
  /**
   * First time taken, it starts the chronometer.
   */
  void start();
  
  /**
   * Intermediate time taken. It creates a time step without stopping the whole chronometer.
   */
  void step(const std::string cursor = "");
  
  /**
   * Last time taken, it stops the chronometer.
   */
  void stop(const std::string cursor = "");
  
  /**
   * Reset the chronometer at its initial state.
   * It clears the benchmark stat.
   */
  void reset();
  
  /**
   * Get the benchmark stat in microseconds, that is the time elapsed between the given cursor and its previous time taken.
   * 
   * @param cursor The n-th time taken.
   * @return the time taken in the structure std::chrono::microseconds. You may want to call "count()" on it.
   */
  std::chrono::microseconds benchmark(const size_t cursor = 0) const;
  
  /**
   * Get the benchmark stat in microseconds, that is the time elapsed between the given cursor and its previous time taken.
   * 
   * @param cursor The name of the cursor saved.
   * @return the time taken in the structure std::chrono::microseconds. You may want to call "count()" on it.
   */
  std::chrono::microseconds benchmark(const std::string cursor) const;
  
  /**
   * Get the benchmark stat in microseconds, that is the time elapsed between the given cursor and the first time taken.
   * 
   * @param cursor The n-th time taken.
   * @return the time taken in the structure std::chrono::microseconds. You may want to call "count()" on it.
   */
  std::chrono::microseconds cumulativeBenchmark(const size_t cursor = 0) const;
  
  /**
   * Get the benchmark stat in microseconds, that is the time elapsed between the given cursor and the first time taken.
   * 
   * @param cursor The name of the cursor saved.
   * @return the time taken in the structure std::chrono::microseconds. You may want to call "count()" on it.
   */
  std::chrono::microseconds cumulativeBenchmark(const std::string cursor) const;
};
//-----------------------------------------------------------------------------

#endif // SIMPLE_BENCHMARK_H
