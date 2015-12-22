/*
 * Big integers random generator.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZ_RANDOM_GENERATOR_H
#define ZZ_RANDOM_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include <NTL/ZZ.h>
#include "randgen.h"
//-----------------------------------------------------------------------------

/**
 * @example examples/RandomZZGenerator.cpp
 */

/**
 * Random generator for big integers.
 * This class is useful to generate random big integers with the NTL library.
 */
class RandomZZGenerator : public RandomGenerator<NTL::ZZ>
{
protected:
  NTL::ZZ sup; ///< This is the supremum, the least number which is greater than all generated numbers
  
public:
  RandomZZGenerator();
  
  /**
   * Set the supremum at construction time.
   * 
   * @param sup The supremum (least number which is greater than all generated numbers)
   */
  RandomZZGenerator(const NTL::ZZ &sup);
  
  /**
   * Both set the supremum and specify the random seed at construction time.
   * 
   * @param sup The supremum (least number which is greater than all generated numbers)
   * @param seed The seed used for the random generation.
   */
  RandomZZGenerator(const NTL::ZZ &sup, const NTL::ZZ &seed);
  virtual ~RandomZZGenerator();
  
  void setSeed(const NTL::ZZ &seed);
  NTL::ZZ next() const;
  
  /**
   * Get the supremum.
   */
  NTL::ZZ getSupremum() const;
  
  /**
   * Set the supremum.
   * 
   * @param sup The supremum (least number which is greater than all generated numbers)
   */
  void setSupremum(const NTL::ZZ &sup);
};
//-----------------------------------------------------------------------------

#endif // ZZ_RANDOM_GENERATOR_H
