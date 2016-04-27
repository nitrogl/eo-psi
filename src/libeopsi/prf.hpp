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
template <typename T, typename U>
class PseudoRandom
{
protected:
  SHAByteArray hashAlgorithm; ///< Hash algorithm
  size_t autoIndex;
  T *autoSeedPtr;
  T autoSeed;
  
public:
  /**
   * Empty constructor.
   */
  PseudoRandom() {
    this->autoIndex = 0;
    this->autoSeedPtr = nullptr;
  };
  
  /**
   * Destructor.
   */
  virtual ~PseudoRandom() {
    
  };
  
  /**
   * Generate a random element with seed, index and maximum length in bits
   * 
   * @param seed the seed of the generation
   * @param index the index-th key
   * @param bits (optional) the length of generation in bits
   */
  virtual U generate(const T seed, const size_t index, const size_t bits = 0) = 0;
  
  /**
   * This method should generate the random seed for generations in which
   * the seed is not such important.
   */
  virtual T randomSeed() = 0;
  
  /**
   * This method should give a default constant seed.
   */
  virtual T defaultSeed() = 0;
  
  /**
   * Generate a random element with index and maximum length in bits.
   * With automatic seed.
   * 
   * @param index the index-th key
   * @param bits (optional) the length of generation in bits
   */
  virtual U generate(const size_t index, const size_t bits = 0) {
    if (this->autoSeedPtr == nullptr) {
      this->autoSeed = this->randomSeed();
      this->autoSeedPtr = &this->autoSeed;
    }
//     return generate(defaultSeed(), index, bits); // [[TODO: remove me (debug purposes)]] 
    return generate(this->autoSeed, index, bits);
  }
  
  
  /**
   * Generate a random element careless of seed, and other things.
   */
  virtual U generate() {
    return generate(this->autoIndex++, 0);
  }
};
//-----------------------------------------------------------------------------

#endif // PRF_TEMPLATE
