/*
 * Independent random stream for NTL ZZ big integers.
 * Compatibility with NTL 5.5.2.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef INDEPENDENT_ZZ_RANDOM_STREAM_H
#define INDEPENDENT_ZZ_RANDOM_STREAM_H
//-----------------------------------------------------------------------------

#include <NTL/ZZ.h>
#include <NTL/version.h>
#include "byte.h"
#include "ntlmiss.h"
#include "indzzrndstream_retrocomp.h"
//-----------------------------------------------------------------------------

/**
 * This class implements an independent NTL random stream for custom
 * generations of ZZ numbers (independent from usual calls).
 */
class IndependentZZRandomStream: public NTL::RandomStream
{
private:
  /**
   * Interpret a big integer as a seed for the random stream
   */
  byte* zzSeed2byte(const NTL::ZZ& seed) const;
  
public:
  /**
   * Make a new random stream initialised with the specified seed.
   * 
   * @param seed the seed to set.
   * @return the random stream initialised with the seed.
   */
  IndependentZZRandomStream(const NTL::ZZ& seed);
  virtual ~IndependentZZRandomStream();
  
  /**
   * Get a random natural number less than bnd from a given (custom) stream.
   * 
   * @param bnd the smallest number which cannot be generated.
   * @param stream the random stream from which generate the number.
   */
  virtual NTL::ZZ randomBnd(const NTL::ZZ& bnd);
};
//-----------------------------------------------------------------------------

#endif // INDEPENDENT_ZZ_RANDOM_STREAM_H
