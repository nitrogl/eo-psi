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
  void vector2VecZZp(vec_ZZ_p &nv, const std::vector<ZZ_p> &v);
  
  /**
   * Convert STL vector of integers modulo p to its corresponding NTL vector.
   * 
   * @param v The STL vector to read from.
   */
  vec_ZZ_p vector2VecZZp(const std::vector<ZZ_p> &v);
  
  /**
   * This function adds a zero padding to a big integer up to a specific
   * padding size (-1).
   * 
   * @param z the number to pad
   * @param padsize the pad size up to which expand z
   * @return the padded z up to padsize
   */
  NTL::ZZ zeroPad(const NTL::ZZ z, const size_t padsize);
}
//-----------------------------------------------------------------------------

#endif // NTL_MISS_H
