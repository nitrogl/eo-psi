/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZ_P_RANDOM_GENERATOR_H
#define ZZ_P_RANDOM_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include <NTL/ZZ_p.h>
#include "zzrandgen.h"
//-----------------------------------------------------------------------------

/**
 * Random generator for big integers modulo p.
 * This class is useful to generate random big integers modulo p with the NTL library.
 * It basically uses the random generator for integers with supremum p.
 * @see RandomZZGenerator
 */
class RandomZZpGenerator : public RandomGenerator<NTL::ZZ_p>
{
protected:
  RandomZZGenerator rndZZgen;
  
public:
  RandomZZpGenerator();
  
  /**
   * Set the modulo p at construction time.
   * 
   * @param p The modulo p.
   */
  RandomZZpGenerator(const NTL::ZZ &p);
  
  /**
   * Both set the modulo p and specify the random seed at construction time.
   * 
   * @param p The modulo p.
   * @param seed The seed used for the random generation.
   */
  RandomZZpGenerator(const NTL::ZZ &p, const NTL::ZZ_p &seed);
  virtual ~RandomZZpGenerator();
  
  void setSeed(const NTL::ZZ_p &seed);
  NTL::ZZ_p next();
  
  /**
   * Get the modulo p used for the generation.
   */
  NTL::ZZ getModulo() const;
  
  /**
   * Set the modulo p for the generation.
   * 
   * @param p The modulo p.
   */
  void setModulo(const NTL::ZZ &p);
};
//-----------------------------------------------------------------------------

#endif // ZZ_P_RANDOM_GENERATOR_H
