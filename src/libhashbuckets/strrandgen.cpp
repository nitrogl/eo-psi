/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "strrandgen.h"
//-----------------------------------------------------------------------------

RandomStringGenerator::RandomStringGenerator() {
  srand(time(NULL));
  
  for (i = 0; i < 256; i++) {
    this->alphabet[i] = false;
  }
  this->n = 0;
}
//-----------------------------------------------------------------------------

RandomStringGenerator::~RandomStringGenerator() {
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setSeed(std::string &seed) {
  unsigned long n = 0;
  const char *c = seed.c_str();
  
  for (i = 0; i < seed.length(); i++) {
    n += c[i];
  }
  
  srand(n);
}
//-----------------------------------------------------------------------------

std::string RandomStringGenerator::next() const {
  char rnd[this->max - this->min + 1];
  size_t i;
  
  // epsilon string
  if (this->n == 0) {
    return "";
  }
  
  for (i = 0; i < n; i++) {
    
  }
  
  
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setLength(const size_t len) {
  this->setLength(len, len);
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setLength(const size_t min, const size_t max) {
  if (min > max) {
    this->min = max;
    this->max = min;
  } else {
    this->min = min;
    this->max = max;
  }
}
//-----------------------------------------------------------------------------

size_t RandomStringGenerator::countAlphabetSet() const {
  return this->n;
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setEnabledChar(const char c, const bool enabled) {
  unsigned char index = c;
  
  if (alphabet[index] == enabled) {
    return;
  }
  
  alphabet[index] = enabled;
  
  // Update alphabet cardinality
  if (enabled) {
    this->n++;
  } else {
    this->n--;
  }
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setEnabled(const AsciiSubset subset, const bool enabled) {
  unsigned char i;
  
  if (subset | AsciiSubset::DIGITS) {
    for (i = '0'; i <= '9'; i++) {
      this->setEnabledChar(i, enabled);
    }
  }
  
  if (subset | AsciiSubset::LETTERS_LOWERCASE) {
    for (i = 'a'; i <= 'z'; i++) {
      this->setEnabledChar(i, enabled);
    }
  }
  
  if (subset | AsciiSubset::LETTERS_UPPERCASE) {
    for (i = 'A'; i <= 'Z'; i++) {
      this->setEnabledChar(i, enabled);
    }
  }
  
  if (subset | AsciiSubset::SPACE) {
    this->setEnabledChar(' ', enabled);
  }
  
  if (subset | AsciiSubset::PUNCTUATION) {
    this->setEnabledChar(',', enabled);
    this->setEnabledChar('.', enabled);
    this->setEnabledChar(':', enabled);
    this->setEnabledChar(';', enabled);
    this->setEnabledChar('!', enabled);
    this->setEnabledChar('?', enabled);
    this->setEnabledChar('(', enabled);
    this->setEnabledChar(')', enabled);
    this->setEnabledChar('\'', enabled);
    this->setEnabledChar('"', enabled);
    this->setEnabledChar('-', enabled);
  }
}
//-----------------------------------------------------------------------------
