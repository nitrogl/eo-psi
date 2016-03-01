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
#include "hashalgorithm.hpp"

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

/**
 * Implementation of Murmur hash version 3, hashing integers modulo p.
 * It makes use of NTL library to handle big integers modulo p.
 * It completely ignores the modulo p, one has to handle it outside this function.
 */
class MurmurHash3: public HashAlgorithm<NTL::ZZ_p>
{
private:
  uint32_t seed;                    ///< The seed of generation
  std::stringstream zzStreamString; ///< Stream string used to (quickly?) convert strings to \f$\mathbb{Z}_p\f$
  std::string zzString;             ///< String used to (quickly?) convert strings to \f$\mathbb{Z}_p\f$
  byte hash32[4];                   ///< Caching hash at 32 bits
  byte hash128[16];                 ///< Caching hash at 128 bits
  
  /**
   * Murmur hash version 3 for x86 generating 32-bit hash.
   * 
   * @param key that to be hashed.
   * @param len the length of key.
   * @param out the result of the hash. At least 4 byte sized.
   */
  void murmurHash3_x86_32  (const void * key, int len, void * out);
  
  /**
   * Murmur hash version 3 for x86 generating 128-bit hash.
   * 
   * @param key that to be hashed.
   * @param len the length of key.
   * @param out the result of the hash. At least 16 byte sized.
   */
  void murmurHash3_x86_128 (const void * key, int len, void * out);
  
  /**
   * Murmur hash version 3 for 64-bit architectures generating 128-bit hash.
   * 
   * @param key that to be hashed.
   * @param len the length of key.
   * @param out the result of the hash. At least 16 byte sized.
   */
  void murmurHash3_x64_128 (const void * key, int len, void * out);
  
public:
  MurmurHash3();
  MurmurHash3(uint32_t seed);
  virtual ~MurmurHash3();
  
  unsigned char* hash(const NTL::ZZ_p n, const size_t len = 0);
  size_t hashSize() const;
  std::string name() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_MURMURHASH3_H
