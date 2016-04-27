/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZ_P_PRF_H
#define ZZ_P_PRF_H
//-----------------------------------------------------------------------------

#include "zzprf.h"
//-----------------------------------------------------------------------------

/**
 * This class acts as a key generator.
 * The secret key of type S of this generator changes the seed is supposed to
 * work together with some index to generate the key corresponding to the
 * index.
 */
class ZZpPRF: public PseudoRandom<NTL::ZZ_p, NTL::ZZ_p>
{
protected:
  ZZPRF prf;
  NTL::ZZ modulo;
  
public:
  ZZpPRF(NTL::ZZ modulo, ZZPRF* prf = nullptr);
  virtual ~ZZpPRF();
  
  virtual void setModulo(NTL::ZZ modulo);
  virtual NTL::ZZ getModulo() const;
  
  virtual NTL::ZZ_p generate(const NTL::ZZ_p seed, const size_t index, const size_t bits = 0);
  virtual NTL::ZZ_p randomSeed();
  virtual NTL::ZZ_p defaultSeed();
};
//-----------------------------------------------------------------------------

#endif // ZZ_P_PRF_H
