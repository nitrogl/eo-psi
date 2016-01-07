/*
 * Some useful functions using NTL.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef NTL_MISS_H
#define NTL_MISS_H
//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/vec_ZZ_p.h>
//-----------------------------------------------------------------------------

namespace NTL {
  /**
   * Convert a string to a big integer ZZ
   * 
   * @param s is a big number made of digits, i.e. "1491249129421"
   */
  ZZ str2zz(const std::string s);
  
  /**
   * Compute the base-2 logarithm of a big integer number
   * 
   * @param z the big integer to compute the logarithm of
   * @return the integer logarithm of z
   */
  unsigned long log2(const ZZ &z);
  
  /**
   * Compute the number of base-2 cyphers (bits) needed to express the
   * number given as argument.
   * 
   * @param z the number in base-10
   */
  unsigned long bits(const ZZ &z);
  
  /**
   * Compute the number of base-2 cyphers (bits) needed to express the
   * number modulo p given as argument.
   * 
   * @param z the number modulo p in base-10
   */
  unsigned long bits(const ZZ_p &zp);
  
  /**
   * Convert STL vector of integers modulo p to its corresponding NTL vector.
   * @warning The passed NTL vector will be cleared.
   * 
   * @param nv The NTL vector to fill in.
   * @param v The STL vector to read from.
   */
  void vector2VecZZp(Vec<ZZ_p> &nv, const std::vector<ZZ_p> &v);
  
  /**
   * Convert STL vector of integers modulo p to its corresponding NTL vector.
   * 
   * @param v The STL vector to read from.
   */
  Vec<ZZ_p> vector2VecZZp(const std::vector<ZZ_p> &v);
};
//-----------------------------------------------------------------------------

#endif // NTL_MISS_H
