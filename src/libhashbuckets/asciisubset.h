/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ASCII_SUBSET_H
#define ASCII_SUBSET_H
//-----------------------------------------------------------------------------

typedef enum AsciiSubset {
  DIGITS = 1,
  LETTERS_LOWERCASE = 2,
  LETTERS_UPPERCASE = 4,
  SPACE = 8,
  PUNCTUATION = 16,
  
  // Mix
  LETTERS = (LETTERS_LOWERCASE | LETTERS_UPPERCASE),
  ALPHANUMERIC_LOWERCASE = (LETTERS_LOWERCASE | DIGITS),
  ALPHANUMERIC_UPPERCASE = (LETTERS_UPPERCASE | DIGITS),
  ALPHANUMERIC = (LETTERS_LOWERCASE | LETTERS_UPPERCASE | DIGITS),
} AsciiSubset;
//-----------------------------------------------------------------------------

#endif // ASCII_SUBSET_H
