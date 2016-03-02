/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZPRF_H
#define ZZPRF_H
//-----------------------------------------------------------------------------

#include <NTL/ZZ.h>
#include "ntlmiss.h"
#include "hashalgorithm.hpp"
#include "shabyte.h"
//-----------------------------------------------------------------------------

/**
 * This class acts as a key generator.
 * The secret key of type S of this generator changes the seed is supposed to
 * work together with some index to generate the key corresponding to the
 * index.
 */
class ZZPRF
{
protected:
  SHAByteArray hashAlgorithm; ///< Hash algorithm
  byte *gen;
  byte *hashInput;
  size_t hashInputLength;
  size_t genLength;
  
public:
  ZZPRF();
  virtual ~ZZPRF();
  
  /**
   * Generate a key with a specific index
   * 
   * @param index the index-th key
   */
  virtual NTL::ZZ generate(const NTL::ZZ seed, const size_t index, const size_t bits);
};
//-----------------------------------------------------------------------------

#endif // ZZPRF_H
