/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZ_P_RANDOM_GENERATOR_H
#define ZZ_P_RANDOM_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include <NTL/ZZ.h>
#include "randgen.h"
//-----------------------------------------------------------------------------

class ZZRandomGenerator : RandomGenerator<NTL::ZZ>
{
protected:
  NTL::ZZ p; // Integers modulo p
  
public:
  ZZRandomGenerator(NTL::ZZ &p);
  ZZRandomGenerator(NTL::ZZ &p, NTL::ZZ &seed);
  virtual ~ZZRandomGenerator();
  
  void setSeed(NTL::ZZ &seed);
  NTL::ZZ next() const;
};
//-----------------------------------------------------------------------------

#endif // ZZ_P_RANDOM_GENERATOR_H
