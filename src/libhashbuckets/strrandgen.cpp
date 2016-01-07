/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "strrandgen.h"
//-----------------------------------------------------------------------------

RandomStringGenerator::RandomStringGenerator(const size_t len, const AsciiSubset subset) {
  this->setEnabledOnly(subset);
  this->length = len;
}
//-----------------------------------------------------------------------------

RandomStringGenerator::RandomStringGenerator(const size_t len, const int subset) : RandomStringGenerator::RandomStringGenerator(len, (const AsciiSubset) subset) {
}
//-----------------------------------------------------------------------------

RandomStringGenerator::RandomStringGenerator(const std::string &seed, const size_t len, const AsciiSubset subset) : RandomStringGenerator::RandomStringGenerator(len, subset) {
  this->setSeed(seed);
}
//-----------------------------------------------------------------------------

RandomStringGenerator::RandomStringGenerator(const std::string &seed, const size_t len, const int subset) : RandomStringGenerator::RandomStringGenerator(seed, len, (const AsciiSubset) subset) {
}
//-----------------------------------------------------------------------------

RandomStringGenerator::~RandomStringGenerator() {
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setSeed(const std::string &seed) {
  unsigned long n = 0;
  const char *c = seed.c_str();
  size_t i;
  
  for (i = 0; i < seed.length(); i++) {
    n += c[i];
  }
  
  srand(n);
}
//-----------------------------------------------------------------------------

std::string RandomStringGenerator::next(const size_t len) {
  std::string rnd;
  unsigned char c, k, index0;
  size_t i, j;
  
  // epsilon string
  if (this->n == 0) {
    return "";
  }
  
  // Get the first allowed position in the alphabet
  for (index0 = 0; !alphabet[++index0];);
  
  for (i = 0; i < len; i++) {
    // Index of the subset of the alphabet
    c = rand() % this->n;
    
    // Find the right element of the alphabet
    k = 0;
    j = index0;
    while (j < 256 && k < c) {
      if (alphabet[++j]) {
        k++;
      }
    }
    
    // Append to result
    rnd.append(1, (char) j);
  }
  
  return rnd;
}
//-----------------------------------------------------------------------------

std::string RandomStringGenerator::next() {
  return this->next(this->length);
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setLength(const size_t len) {
  this->length = len;
}
//-----------------------------------------------------------------------------

size_t RandomStringGenerator::countAlphabetSet() const {
  return this->n;
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setEnabledChar(const char c, const bool enabled) {
  size_t index = (unsigned char) c;
  
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
  
  if (subset & AsciiSubset::DIGITS) {
    for (i = '0'; i <= '9'; i++) {
      this->setEnabledChar(i, enabled);
    }
  }
  
  if (subset & AsciiSubset::LETTERS_LOWERCASE) {
    for (i = 'a'; i <= 'z'; i++) {
      this->setEnabledChar(i, enabled);
    }
  }
  
  if (subset & AsciiSubset::LETTERS_UPPERCASE) {
    for (i = 'A'; i <= 'Z'; i++) {
      this->setEnabledChar(i, enabled);
    }
  }
  
  if (subset & AsciiSubset::SPACE) {
    this->setEnabledChar(' ', enabled);
  }
  
  if (subset & AsciiSubset::PUNCTUATION) {
    this->setEnabledChar(',', enabled);
    this->setEnabledChar('.', enabled);
    this->setEnabledChar(':', enabled);
    this->setEnabledChar(';', enabled);
    this->setEnabledChar('!', enabled);
    this->setEnabledChar('?', enabled);
    this->setEnabledChar('(', enabled);
    this->setEnabledChar(')', enabled);
    this->setEnabledChar('"', enabled);
    this->setEnabledChar('-', enabled);
    this->setEnabledChar('\'', enabled);
  }
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setEnabled(const int subset, const bool enabled) {
  this->setEnabled((AsciiSubset) subset, enabled);
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setEnabledOnly(const AsciiSubset subset, const bool enabled) {
  this->disableAllChars();
  this->setEnabled(subset, enabled);
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::setEnabledOnly(const int subset, const bool enabled) {
  this->setEnabledOnly((AsciiSubset) subset, enabled);
}
//-----------------------------------------------------------------------------

void RandomStringGenerator::disableAllChars() {
  size_t i;
  
  for (i = 0; i < 256; i++) {
    alphabet[i] = false;
  }
  this->n = 0;
}
//-----------------------------------------------------------------------------
