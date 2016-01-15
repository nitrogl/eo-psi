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
#include "randgen.hpp"
//-----------------------------------------------------------------------------

/**
 * @example RandomZZGenerator.cpp
 */

/**
 * Random generator for big integers.
 * This class is useful to generate random big integers with the NTL library.
 */
class RandomZZGenerator : public RandomGenerator<NTL::ZZ>
{
protected:
  NTL::ZZ sup;                          ///< This is the supremum, the least number which is greater than all generated numbers.
  
public:
  /**
   * New instance of the generator.
   */
  RandomZZGenerator();
  
  /**
   * Set the supremum at construction time.
   * 
   * @param sup The supremum (the smallest number which is greater than all generated numbers).
   */
  RandomZZGenerator(const NTL::ZZ &sup);
  
  /**
   * Both set the supremum and specify the random seed at construction time.
   * 
   * @param sup The supremum (the smallest number which is greater than all generated numbers).
   * @param seed The seed used for the random generation.
   */
  RandomZZGenerator(const NTL::ZZ &sup, const NTL::ZZ &seed);
  virtual ~RandomZZGenerator();
  
  virtual void setSeed(const NTL::ZZ &seed);
  virtual NTL::ZZ next();
  
  /**
   * Get the supremum.
   * 
   * @return the smallest number which is not being generated by the PRG.
   */
  virtual NTL::ZZ getSupremum() const;
  
  /**
   * Set the supremum.
   * 
   * @param sup The supremum (the smallest number which is greater than all generated numbers).
   */
  virtual void setSupremum(const NTL::ZZ &sup);
};
//-----------------------------------------------------------------------------

#endif // ZZ_RANDOM_GENERATOR_H
