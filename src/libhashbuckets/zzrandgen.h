/*
 * Integers modulo p random generator
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

class RandomZZGenerator : public RandomGenerator<NTL::ZZ>
{
protected:
  NTL::ZZ p; // Integers modulo p
  
public:
  RandomZZGenerator();
  RandomZZGenerator(const NTL::ZZ &p);
  RandomZZGenerator(const NTL::ZZ &p, const NTL::ZZ &seed);
  virtual ~RandomZZGenerator();
  
  void setSeed(const NTL::ZZ &seed);
  NTL::ZZ next() const;
  
  NTL::ZZ getSupremum() const;
  void setSupremum(const NTL::ZZ &p);
};
//-----------------------------------------------------------------------------

#endif // ZZ_RANDOM_GENERATOR_H
