/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef STRING_RANDOM_GENERATOR_H
#define STRING_RANDOM_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include "randgen.h"
#include "asciisubset.h"
//-----------------------------------------------------------------------------

class RandomStringGenerator : RandomGenerator<std::string>
{
private:
  size_t i, j, n;
  bool alphabet[256];
  
protected:
  size_t min;
  size_t max;
  
public:
  
  RandomStringGenerator();
  virtual ~RandomStringGenerator();
  
  void setSeed(std::string &seed);
  std::string next() const;
  
  void setLength(const size_t len);
  void setLength(const size_t min, const size_t max);
  size_t countAlphabetSet() const;
  void setEnabledChar(const char c, const bool enabled = true);
  void setEnabled(const AsciiSubset subset, const bool enabled = true);
};
//-----------------------------------------------------------------------------

#endif // STRING_RANDOM_GENERATOR_H
