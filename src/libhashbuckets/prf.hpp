/*
 * A simple generic pseudo-random function class
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef PRF_TEMPLATE
#define PRF_TEMPLATE
//-----------------------------------------------------------------------------

#include <NTL/ZZ.h>
#include "ntlmiss.h"
#include "hashalgorithm.hpp"
#include "shabyte.h"
//-----------------------------------------------------------------------------

/**
 * This class gives pseudo-random facilities.
 */
template <class T, class U> class PseudoRandom
{
protected:
  SHAByteArray hashAlgorithm; ///< Hash algorithm
  
public:
  PseudoRandom() {};
  virtual ~PseudoRandom() {};
  
  /**
   * Generate a random element with seed, index of maximum length in bits
   * 
   * @param seed the seed of the generation
   * @param index the index-th key
   * @param bits the (maximum?) length of generation in bits
   */
  virtual U generate(const T seed, const size_t index, const size_t bits = 0) = 0;
  
  /**
   * This method should generate the random seed for generations in which
   * the seed is not such important.
   */
  virtual T randomSeed() = 0;
  
  /**
   * Generate a random element with seed, index of maximum length in bits.
   * With automatic seed.
   * 
   * @param index the index-th key
   * @param bits the (maximum?) length of generation in bits
   */
  virtual U generate(const size_t index, const size_t bits = 0) {
    return generate(randomSeed(), index, bits);
  }
};
//-----------------------------------------------------------------------------

#endif // PRF_TEMPLATE
