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
#include "byte.h"
//-----------------------------------------------------------------------------

/**
 * This class implements a kind of union of strings and big integers.
 * A string can be then interpreted as a big natural number.
 * The only operations provided are:
 * <ul>
 *   <li>the remainder with an unsigned long divisor</li>
 *   <li>the addition to an unsigned long addend</li>
 *   <li>the increment (by 1)</li>
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
  StrInt(const size_t bytes, const byte *n = nullptr, const size_t dim = 0);
  virtual ~StrInt();
  
  /**
   * Set a relatively small unsigned long number.
   * 
   * @param n The number in unsigned long format to set.
   */
  void set(const unsigned long n);
  
  /**
   * Set a number in string format to be interpreted as big natural number.
   * The byte array is not supposed to be human readable.
   * 
   * @param n The number in string format to set (not a c-string, '\0' is not the string terminator).
   * @param dim The number of bytes in the string to read.
   */
  void set(const byte *n, const size_t dim);
  
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
   * Give the bytes of the number. If the bytelen passed greater or equal to
   * the needed number of bytes to write, no allocation will be done.
   * @see getByteSize
   * 
   * @param b the bytes to save the result to. If nullptr is passed, new memory is allocated.
   * @param bytelen this will contain the length of allocated memory
   * @return the byte array b passed but newly allocated.
   */
  byte* toBytes(byte *barr, size_t* bytelen) const;
  
  /**
   * This method says how many bytes a byte conversion of this structure needs.
   * 
   * @see toBytes
   */
  size_t getByteSize() const;
  
  /**
   * Give the string of the number in readable decimal format.
   */
  std::string toString() const;
  
  /**
   * Give the least significant (unsigned long size) bits of the number.
   */
  unsigned long lsb() const;
  
  /**
   * Output
   */
  friend std::ostream& operator<<(std::ostream& os, const StrInt& strint);
};
//-----------------------------------------------------------------------------

#endif // STRINT_H
