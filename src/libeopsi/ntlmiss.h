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
   * Compute the number of blocks of size given as (second) argument 
   * needed to express a bit string of length given as (first) argument.
   * 
   * @param bits the number of bits
   * @param bitsPerBlock the block size in bits
   */
  unsigned long blocks(const unsigned long bits, const unsigned long bitsPerBlock);
  
  /**
   * Compute the number of bytes needed to express a bit string of
   * length given as argument.
   * 
   * @param bits the number of bits
   */
  unsigned long bytes(const unsigned long bits);
  
  /**
   * Compute the number of bytes needed to express the
   * number given as argument.
   * 
   * @param z the number in base-10
   */
  unsigned long bytes(const ZZ &z);
  
  /**
   * Compute the number of bytes needed to express the
   * number modulo p given as argument.
   * 
   * @param z the number modulo p in base-10
   */
  unsigned long bytes(const ZZ_p &zp);
  
  /**
   * Convert STL vector of integers modulo p to its corresponding NTL vector.
   * @warning The passed NTL vector will be cleared.
   * 
   * @param nv The NTL vector to fill in.
   * @param v The STL vector to read from.
   */
  void vector2VecZZp(vec_ZZ_p &zp, const std::vector<ZZ_p> &v);
  
  /**
   * Convert STL vector of integers modulo p to its corresponding NTL vector.
   * 
   * @param v The STL vector to read from.
   */
  vec_ZZ_p vector2VecZZp(const std::vector<ZZ_p> &v);
  
  /**
   * Convert C array of integers modulo p to its corresponding NTL vector.
   * 
   * @param nv The NTL vector to fill in.
   * @param v The array to read from.
   * @param dim The dimension of the array.
   */
  void array2VecZZp(vec_ZZ_p &zp, const ZZ_p *v, const size_t dim);
  
  /**
   * Convert C array of integers modulo p to its corresponding NTL vector.
   * 
   * @param v The array to read from.
   * @param dim The dimension of the array.
   */
  vec_ZZ_p array2VecZZp(const ZZ_p *v, const size_t dim);
  
  /**
   * This function adds a zero padding to a big integer up to a specific
   * padding size (-1).
   * 
   * @param z the number to pad
   * @param padsize the pad size up to which expand z
   * @return the number of zeros effectively added
   */
  ZZ zeroPad(const ZZ& z, const size_t padsize);
  
  /**
   * This function adds a zero padding to a big integer up to a specific
   * padding size (-1).
   * 
   * @param z the number to pad
   * @param padsize the pad size up to which expand z
   * @return the number of zeros effectively added
   */
  ZZ zeroPadTo(const ZZ& z, const size_t padsize);
  
  /**
   * This function counts the number of leading zeros of the binary
   * notation of a big integer.
   * 
   * @param z the number to check
   */
  size_t countLeadingZeros(const ZZ& z);

  /**
   * Pretty print a C array of big integers.
   * 
   * @param arr the array to print
   * @param n the number of elements to print
   */
  void printZZArray(const ZZ *arr, const size_t n);
}
//-----------------------------------------------------------------------------

#endif // NTL_MISS_H
