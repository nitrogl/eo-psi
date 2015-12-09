/*
 * A simple benchmark
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "simplebm.h"

SimpleBenchmark::SimpleBenchmark() {
  this->times.reserve(INIT_TIMES_SIZE);
  this->reset();
}

SimpleBenchmark::~SimpleBenchmark() {
}

void SimpleBenchmark::start() {
  this->reset();
  this->takeTime();
}

void SimpleBenchmark::takeTime() {
  this->times.push_back(std::chrono::high_resolution_clock::now());
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
    std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t - t);
  }
  
  if (cursor < 1) {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[this->times.size() - 2]);
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[cursor - 1]);
  }
}

std::chrono::microseconds SimpleBenchmark::cumulativeBenchmark(size_t cursor) const {
  if (cursor >= this->times.size()) {
    std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t - t);
  }
  if (cursor < 1) {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[this->times.size() - 1] - this->times[0]);
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(this->times[cursor] - this->times[0]);
  }
}
