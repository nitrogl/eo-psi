/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef RANDOM_STRING_GENERATOR_H
#define RANDOM_STRING_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include "randgen.h"
#include "asciisubset.h"

#define DEFAULT_STRGEN_LENGTH 8
#define DEFAULT_STRGEN_ASCII_SUBSET ((AsciiSubset) (AsciiSubset::LETTERS_LOWERCASE | AsciiSubset::SPACE))
//-----------------------------------------------------------------------------

class RandomStringGenerator : public RandomGenerator<std::string>
{
protected:
  size_t length;
  bool alphabet[256];
  size_t n;
  
public:
  RandomStringGenerator(const size_t len = DEFAULT_STRGEN_LENGTH, const AsciiSubset subset = DEFAULT_STRGEN_ASCII_SUBSET);
  RandomStringGenerator(const size_t len, const int subset);
  RandomStringGenerator(const std::string &seed, const size_t len = DEFAULT_STRGEN_LENGTH, const AsciiSubset subset = DEFAULT_STRGEN_ASCII_SUBSET);
  RandomStringGenerator(const std::string &seed, const size_t len = DEFAULT_STRGEN_LENGTH, const int subset = DEFAULT_STRGEN_ASCII_SUBSET);
  virtual ~RandomStringGenerator();
  
  void setSeed(const std::string &seed);
  std::string next() const;
  std::string next(const size_t len) const;
  
  void setLength(const size_t len);
  size_t countAlphabetSet() const;
  void setEnabledChar(const char c, const bool enabled = true);
  void setEnabled(const AsciiSubset subset, const bool enabled = true);
  void setEnabled(const int subset, const bool enabled = true);
  void setEnabledOnly(const AsciiSubset subset, const bool enabled = true);
  void setEnabledOnly(const int subset, const bool enabled = true);
  void disableAllChars();
};
//-----------------------------------------------------------------------------

#endif // RANDOM_STRING_GENERATOR_H
