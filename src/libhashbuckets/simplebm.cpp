/*
 * A simple benchmark.
 * It requires C++11.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "simplebm.h"

SimpleBenchmark::SimpleBenchmark(size_t tSize) {
  // Init vector of times and internal state of the benchmark
  this->times.reserve(tSize);
  this->reset();
}

SimpleBenchmark::~SimpleBenchmark() {
}

void SimpleBenchmark::takeTime() {
  this->times.push_back(std::chrono::high_resolution_clock::now());
}

void SimpleBenchmark::start() {
  this->reset();
  this->takeTime();
}

void SimpleBenchmark::step() {
  if (!this->stopped) {
    this->takeTime();
  }
}

void SimpleBenchmark::reset() {
  this->times.clear();
  this->stopped = false;
}

void SimpleBenchmark::stop() {
  this->takeTime();
  this->stopped = true;
}

std::chrono::microseconds SimpleBenchmark::benchmark(size_t cursor) const {
  if (cursor >= this->times.size()) {
    return SimpleBenchmark::ZEROMS;
  }
  
  if (cursor < 1) {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[this->times.size() - 2]);
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[cursor - 1]);
  }
}

std::chrono::microseconds SimpleBenchmark::cumulativeBenchmark(size_t cursor) const {
  if (cursor >= this->times.size()) {
    return SimpleBenchmark::ZEROMS;
  }
  if (cursor < 1) {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[0]);
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[0]);
  }
}
