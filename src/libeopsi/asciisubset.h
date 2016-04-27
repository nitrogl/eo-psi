/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ASCII_SUBSET_H
#define ASCII_SUBSET_H
//-----------------------------------------------------------------------------

/**
 * High level subsets of 8-bit ASCII table.
 */
typedef enum AsciiSubset {
  DIGITS = 1,            ///< All digits 0-9
  LETTERS_LOWERCASE = 2, ///< English alphabet lowercase letters
  LETTERS_UPPERCASE = 4, ///< English alphabet uppercase letters
  SPACE = 8,             ///< The space
  PUNCTUATION = 16,      ///< Punctuation used in common English text
  
  // Mix
  LETTERS = (LETTERS_LOWERCASE | LETTERS_UPPERCASE),               ///< English alphabet letters (all cases)
  ALPHANUMERIC_LOWERCASE = (LETTERS_LOWERCASE | DIGITS),           ///< English lowercase letters and numbers
  ALPHANUMERIC_UPPERCASE = (LETTERS_UPPERCASE | DIGITS),           ///< English uppercase letters and numbers
  ALPHANUMERIC = (LETTERS_LOWERCASE | LETTERS_UPPERCASE | DIGITS), ///< English letters and numbers
} AsciiSubset;
//-----------------------------------------------------------------------------

#endif // ASCII_SUBSET_H
