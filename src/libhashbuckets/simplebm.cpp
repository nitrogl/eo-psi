/*
 * A simple benchmark.
 * It requires C++11 (-std=c++11 for GNU compilers)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "simplebm.h"
//-----------------------------------------------------------------------------

SimpleBenchmark::SimpleBenchmark(const size_t tSize) {
  // Init vector of times and internal state of the benchmark
  this->times.reserve(tSize);
  this->reset();
}
//-----------------------------------------------------------------------------

SimpleBenchmark::~SimpleBenchmark() {
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::takeTime(const std::string cursor) {
  this->times.push_back(std::chrono::high_resolution_clock::now());
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

void SimpleBenchmark::start() {
  this->reset();
  this->takeTime();
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::step(const std::string cursor) {
  if (!this->stopped) {
    this->takeTime(cursor);
  }
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::reset() {
  this->times.clear();
  this->cursors.clear();
  this->stopped = false;
}
//-----------------------------------------------------------------------------

void SimpleBenchmark::stop(const std::string cursor) {
  this->takeTime(cursor);
  this->stopped = true;
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

std::chrono::microseconds SimpleBenchmark::cumulativeBenchmark(const size_t cursor) const {
  if (cursor >= this->times.size()) {
    return SimpleBenchmark::ZEROMS;
  }
  if (cursor < 1) {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[0]);
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[0]);
  }
}
//-----------------------------------------------------------------------------

std::chrono::microseconds SimpleBenchmark::cumulativeBenchmark(const std::string cursor) const {
  return this->cumulativeBenchmark(this->getNumericCursor(cursor));
}
//-----------------------------------------------------------------------------
