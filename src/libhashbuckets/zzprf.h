/*
 * A class for pseudo-random big integers using NTL
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZ_PRF_H
#define ZZ_PRF_H
//-----------------------------------------------------------------------------

#include <NTL/ZZ.h>
#include "ntlmiss.h"
#include "hashalgorithm.hpp"
#include "shabyte.h"
#include "prf.hpp"
//-----------------------------------------------------------------------------

/**
 * A class for pseudo-random big integers using NTL
 */
class ZZPRF: public PseudoRandom<NTL::ZZ, NTL::ZZ>
{
protected:
  SHAByteArray hashAlgorithm; ///< Hash algorithm
  byte *gen;                  ///< Generated value buffer
  size_t genLength;           ///< Generated value buffer length
  byte *hashInput;            ///< Hash input buffer
  size_t hashInputSize;       ///< Hash input buffer (allocated) size
  
public:
  ZZPRF();
  virtual ~ZZPRF();
  
  virtual NTL::ZZ generate(const NTL::ZZ seed, const size_t index, const size_t bits = 0);
  virtual NTL::ZZ generate(const size_t index, const size_t bits = 0);
  virtual NTL::ZZ randomSeed();
  virtual NTL::ZZ defaultSeed();
};
//-----------------------------------------------------------------------------

#endif // ZZ_PRF_H
