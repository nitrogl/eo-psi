/*
 * String union with integers
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef STRINT_H
#define STRINT_H
//-----------------------------------------------------------------------------

#include <string>
#include <gmpxx.h>
//-----------------------------------------------------------------------------

/**
 * This class implements a kind of union of strings and big integers.
 * A string can be then interpreted as a big natural number.
 * The only operations provided are:
 * <ul>
 *  <li>the remainder with an unsigned long divisor</li>
 * </ul>
 * To do so, it makes use of GMP library.
 */
class StrInt
{
private:
  mpz_t *b;      ///< used to build 
  mpz_t z;       ///< the number to store
  mpz_t *pow2;   ///< caching of powers of two
  size_t length; ///< the maximum length in bytes of the number

public:
  /**
   * Initialise the class with a maximum number of bytes.
   * @warning You can not modify this later.
   * 
   * @param bytes The maximum length in bytes of the number.
   * @param n The number in string format to set (not a c-string, '\0' is not the string terminator).
   * @param dim The number of bytes in the string to read.
   */
  StrInt(const size_t bytes, const unsigned char *n = nullptr, const size_t dim = 0);
  virtual ~StrInt();
  
  /**
   * Set a number in string format to be interpreted as big natural number.
   * 
   * @param n The number in string format to set (not a c-string, '\0' is not the string terminator).
   * @param dim The number of bytes in the string to read.
   */
  void set(const unsigned char *n, const size_t dim);
  
  /**
   * Compute the remainder of the big natural number with a given divisor.
   * 
   * @param divisor The divisor to get the remainder of.
   * @return The remainder modulo divisor.
   */
  unsigned long rem(const unsigned long divisor) const;
  
  /**
   * Increment the internal number by 1
   */
  void inc();
  
  /**
   * Increment the internal number by a number given as argument
   * 
   * @param n the number to add
   */
  void add(const unsigned long n);
  
  /**
   * Give the string of the number.
   */
  std::string toString() const;
  
  /**
   * Output
   */
  friend std::ostream& operator<<(std::ostream& os, const StrInt& strint);
};
//-----------------------------------------------------------------------------

#endif // STRINT_H
