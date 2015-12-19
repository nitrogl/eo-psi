/*
 * Class implementing the Austin Appleby's Murmur Hash, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 * The algorithm source code is in public domain, I only have adapted it to
 * my personal taste.
 */

#ifndef HASHALGORITHM_MURMURHASH3_H
#define HASHALGORITHM_MURMURHASH3_H
//-----------------------------------------------------------------------------

#include <sstream>
#include <NTL/ZZ_p.h>
#include "hashalgorithm.tpp"

#if defined(_MSC_VER) && (_MSC_VER < 1600)
// Microsoft Visual Studio
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;
#else
// Other compilers
#include <cstdint>
#endif
//-----------------------------------------------------------------------------

class MurmurHash3: public HashAlgorithm<NTL::ZZ_p>
{
private:
  uint32_t seed;
  std::stringstream zzStreamString;
  std::string zzString;
  char hash32[4];
  char hash128[16];
  void murmurHash3_x86_32  (const void * key, int len, void * out);
  void murmurHash3_x86_128 (const void * key, int len, void * out);
  void murmurHash3_x64_128 (const void * key, int len, void * out);
  
public:
  MurmurHash3();
  MurmurHash3(uint32_t seed);
  virtual ~MurmurHash3();
  
  char* hash(const NTL::ZZ_p n);
  size_t hashSize() const;
  std::string name() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_MURMURHASH3_H
