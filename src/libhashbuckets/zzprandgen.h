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

class RandomZZpGenerator : public RandomGenerator<NTL::ZZ_p>
{
private:
  RandomZZGenerator rndZZgen;
  
public:
  RandomZZpGenerator();
  RandomZZpGenerator(const NTL::ZZ &p);
  RandomZZpGenerator(const NTL::ZZ &p, const NTL::ZZ_p &seed);
  virtual ~RandomZZpGenerator();
  
  void setSeed(const NTL::ZZ_p &seed);
  NTL::ZZ_p next() const;
  
  NTL::ZZ getModulo() const;
  void setModulo(const NTL::ZZ &p);
};
//-----------------------------------------------------------------------------

#endif // ZZ_P_RANDOM_GENERATOR_H
