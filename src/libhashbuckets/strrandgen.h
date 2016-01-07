/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef RANDOM_STRING_GENERATOR_H
#define RANDOM_STRING_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include "randgen.hpp"
#include "asciisubset.h"

#define DEFAULT_STRGEN_LENGTH 8
#define DEFAULT_STRGEN_ASCII_SUBSET ((AsciiSubset) (AsciiSubset::LETTERS_LOWERCASE | AsciiSubset::SPACE))
//-----------------------------------------------------------------------------

/**
 * Random generator for strings.
 */
class RandomStringGenerator : public RandomGenerator<std::string>
{
protected:
  size_t length;      ///< The length of the string to generate.
  bool alphabet[256]; ///< The alphabet set of all possible chars, true if they can be generated, false otherwise.
  size_t n;           ///< The cardinality of the alphabet subset which is active.
  
public:
  /**
   * Set length and subset in the constructor.
   * 
   * @param len the length of the strings to generate.
   * @param subset the subset of ASCII to enable.
   */
  RandomStringGenerator(const size_t len = DEFAULT_STRGEN_LENGTH, const AsciiSubset subset = DEFAULT_STRGEN_ASCII_SUBSET);
  
  /**
   * Set length and subset in the constructor.
   * 
   * @param len the length of the strings to generate.
   * @param subset the subset of ASCII to enable (as integer).
   */
  RandomStringGenerator(const size_t len, const int subset);
  
  /**
   * Set seed, length, and subset in the constructor.
   * 
   * @param seed the seed of generation.
   * @param len the length of the strings to generate.
   * @param subset the subset of ASCII to enable.
   */
  RandomStringGenerator(const std::string &seed, const size_t len = DEFAULT_STRGEN_LENGTH, const AsciiSubset subset = DEFAULT_STRGEN_ASCII_SUBSET);
  
  /**
   * Set seed, length, and subset in the constructor.
   * 
   * @param seed the seed of generation.
   * @param len the length of the strings to generate.
   * @param subset the subset of ASCII to enable (as integer).
   */
  RandomStringGenerator(const std::string &seed, const size_t len = DEFAULT_STRGEN_LENGTH, const int subset = DEFAULT_STRGEN_ASCII_SUBSET);
  virtual ~RandomStringGenerator();
  
  void setSeed(const std::string &seed);
  std::string next();
  
  /**
   * Generate a string of specified length, regardless its internal parameter of length.
   * 
   * @param len the length of the string to generate.
   */
  virtual std::string next(const size_t len);
  
  /**
   * Set the length of the strings to generate.
   * 
   * @param len the length of the strings to generate.
   */
  void setLength(const size_t len);
  
  /**
   * Get the cardinality of the enabled alphabet subset
   */
  size_t countAlphabetSet() const;
  
  /**
   * Set the enabled state of a specified character in the alphabet.
   * 
   * @param c the character to enable or disable.
   * @param enabled true to enable the character, false to disable it.
   */
  void setEnabledChar(const char c, const bool enabled = true);
  
  /**
   * Set the enabled state of a specified subset in the alphabet.
   * 
   * @param subset the subset of ASCII to enable or disable.
   * @param enabled true to enable the character, false to disable it.
   */
  void setEnabled(const AsciiSubset subset, const bool enabled = true);
  
  /**
   * Set the enabled state of a specified subset in the alphabet.
   * 
   * @param subset the subset of ASCII to enable or disable (as integer).
   * @param enabled true to enable the character, false to disable it.
   */
  void setEnabled(const int subset, const bool enabled = true);
  
  /**
   * Set the enabled state of a specified subset in the alphabet, disabling its complementary.
   * 
   * @param subset the subset of ASCII to enable or disable.
   * @param enabled true to enable the character, false to disable it.
   */
  void setEnabledOnly(const AsciiSubset subset, const bool enabled = true);
  
  /**
   * Set the enabled state of a specified subset in the alphabet, disabling its complementary.
   * 
   * @param subset the subset of ASCII to enable or disable (as integer).
   * @param enabled true to enable the character, false to disable it.
   */
  void setEnabledOnly(const int subset, const bool enabled = true);
  
  /**
   * Disable all the character of the alphabet (only empty strings can be generated).
   */
  void disableAllChars();
};
//-----------------------------------------------------------------------------

#endif // RANDOM_STRING_GENERATOR_H
