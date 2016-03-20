/*
 * A simple benchmark.
 * It requires C++11 (-std=c++11 for GNU compilers)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <math.h>
#include "simplebm.h"
//-----------------------------------------------------------------------------

SimpleBenchmark::SimpleBenchmark(const size_t tSize) {
  // Init vector of times and internal state of the benchmark
  this->times.reserve(tSize);
  this->reset();
  this->stopped = true;
}
//-----------------------------------------------------------------------------

SimpleBenchmark::~SimpleBenchmark() {
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::takeTime(const std::string cursor) {
  this->takeTime(false, cursor);
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::takeTime(const bool pausing, const std::string cursor) {
  this->times.push_back(std::chrono::high_resolution_clock::now());
  this->pauses.push_back(pausing);
  if (cursor != "") {
    this->cursors[cursor] = this->times.size() - 1;
  }
}
//-----------------------------------------------------------------------------

size_t SimpleBenchmark::getNumericCursor(const std::string cursor) const {
  std::unordered_map<std::string, size_t>::const_iterator i;
  
  i = cursors.find(cursor);
  if (i != cursors.end()) {
    return i->second;
  } else {
    if (cursor != "") {
      std::cerr << "SimpleBenchmark. WARNING: Cursor string \"" << cursor << "\" not associated to any time." << std::endl;
    }
    return 0;
  }
}
//-----------------------------------------------------------------------------

std::chrono::microseconds SimpleBenchmark::sumNotPausedTimes(size_t fromCursor, size_t toCursor) const {
  std::chrono::microseconds sum;
  
  sum = SimpleBenchmark::ZEROMS;
  for (size_t i = fromCursor; i < toCursor; i++) {
    if (!this->pauses[i]) {
      sum += std::chrono::duration_cast<std::chrono::microseconds>(this->times[i] - this->times[i - 1]);
    }
  }
  return sum;
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::start() {
  this->reset();
  this->takeTime();
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::step(const std::string cursor) {
  // Automatically start if no times are taken
  if (this->times.size() == 0) {
    if (cursor != "") {
      std::cerr << "step(). WARNING: Impossible to associate any cursor to start time." << std::endl;
    }
    this->start();
    return;
  }
  
  if (this->isPaused() && cursor != "") {
    std::cerr << "step(). WARNING: Impossible to associate any cursor just after a pause." << std::endl;
  }
  
  if (!this->stopped) {
    this->takeTime(cursor);
  }
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::reset() {
  this->times.clear();
  this->cursors.clear();
  this->pauses.clear();
  this->pauses.push_back(false);
  this->stopped = false;
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::pause(const std::string cursor) {
  // Do nothing if chrono has not started
  if (this->times.size() == 0) {
    std::cerr << "pause(). You have to start it first. Nothing to do." << std::endl;
    return;
  }
  
  // Silently ignore double pause
  if (this->isPaused()) {
    return;
  }
  
  if (!this->stopped) {
    this->takeTime(true, cursor);
  }
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::cont() {
  step();
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::stop(const std::string cursor) {
  if (this->stopped) {
    std::cerr << "stop(). You should start before stopping." << std::endl;
    return;
  }
  // Do not take the time in pause
  if (!this->isPaused()) {
    this->takeTime(cursor);
  } else {
    if (cursor != "") {
      std::cerr << "stop(). Ignoring cursor in pause condition." << std::endl;
    }
  }
  this->stopped = true;
}
//-----------------------------------------------------------------------------

bool SimpleBenchmark::isPaused() const {
  return !this->stopped && this->pauses[this->pauses.size() - 1];
}
//-----------------------------------------------------------------------------

bool SimpleBenchmark::isStopped() const {
  return this->stopped;
}
//-----------------------------------------------------------------------------

std::chrono::microseconds SimpleBenchmark::benchmark(const size_t cursor) const {
  if (cursor >= this->times.size()) {
    return SimpleBenchmark::ZEROMS;
  }
  
  if (cursor < 1) {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[this->times.size() - 2]);
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[cursor - 1]);
  }
}
//-----------------------------------------------------------------------------

std::chrono::microseconds SimpleBenchmark::benchmark(const std::string cursor) const {
  return this->benchmark(this->getNumericCursor(cursor));
}
//-----------------------------------------------------------------------------

std::chrono::microseconds SimpleBenchmark::cumulativeBenchmark(const size_t cursor, const bool noPauses) const {
  if (cursor >= this->times.size()) {
    return SimpleBenchmark::ZEROMS;
  }
  if (cursor < 1) {
    if (noPauses) {
      return this->sumNotPausedTimes(1, this->times.size());
    } else {
      return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[0]);
    }
  } else {
    if (noPauses) {
      return this->sumNotPausedTimes(1, cursor + 1);
    } else {
      return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[0]);
    }
  }
}
//-----------------------------------------------------------------------------

std::chrono::microseconds SimpleBenchmark::cumulativeBenchmark(const std::string cursor, const bool noPauses) const {
  return this->cumulativeBenchmark(this->getNumericCursor(cursor), noPauses);
}
//-----------------------------------------------------------------------------

double SimpleBenchmark::average() const {
  if (this->times.size() < 2) {
    std::cerr << "average(). Not enough points to average." << std::endl;
    return 0.;
  }
  return this->cumulativeBenchmark().count() / (double) (this->times.size() - 1);
}
//-----------------------------------------------------------------------------

double SimpleBenchmark::variance() const {
  double avg, var, diff;
  size_t n;
  
  if (this->times.size() < 2) {
    std::cerr << "variance(). Not enough points to compute standard deviation." << std::endl;
    return 0.;
  }
  
  avg = this->average();
  n = this->times.size() - 1;
  var = 0.;
  for (size_t i = 1; i < this->times.size(); i++) {
    diff = std::chrono::duration_cast<std::chrono::microseconds>(this->times[i] - this->times[i - 1]).count() - avg;
    var += diff/n*diff;
  }
  
  return var;
}
//-----------------------------------------------------------------------------

double SimpleBenchmark::standardDeviation() const {
  if (this->times.size() < 2) {
    std::cerr << "standardDeviation(). Not enough points to compute standard deviation." << std::endl;
    return 0.;
  }
  
  return sqrt(this->variance());
}
//-----------------------------------------------------------------------------
