/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

// #ifndef KEYGEN_TEMPLATE
// #define KEYGEN_TEMPLATE
//-----------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include "zzprf.h"
//-----------------------------------------------------------------------------

ZZPRF::ZZPRF() {
  hashInputLength = 0;
  genLength = 0;
  gen = nullptr;
  hashInput = nullptr;
}
//-----------------------------------------------------------------------------

ZZPRF::~ZZPRF() {
  if (gen != nullptr) {
    delete [] gen;
  }
  if (hashInput != nullptr) {
    delete [] hashInput;
  }
}
//-----------------------------------------------------------------------------
  
/**
 * Generate a key with a specific index
 * 
 * @param index the index-th key
 */
NTL::ZZ ZZPRF::generate(const NTL::ZZ seed, const size_t index, const size_t bits) {
  NTL::ZZ z;
  byte *digest;
  size_t k, n, r, len;
  
  n = NTL::bytes(bits);
  len = NTL::bytes(seed) + sizeof(size_t);
  if (n > genLength) {
    delete [] gen;
    
    try {
      gen = new byte[n];
    } catch (std::bad_alloc&) {
      std::cerr << "generate(). Error allocating space." << std::endl;
      exit(2);
    }
    
    genLength = n;
  }
  if (len > hashInputLength) {
    delete [] hashInput;
    
    try {
      hashInput = new byte[len];
    } catch (std::bad_alloc&) {
      std::cerr << "generate(). Error allocating space." << std::endl;
      exit(2);
    }
    
    hashInputLength = len;
  }
  
  NTL::BytesFromZZ(hashInput, seed, NTL::bytes(seed));
  std::memcpy(hashInput + NTL::bytes(seed), &index, sizeof(size_t));
  
  r = n;
  k = 0;
  digest = hashInput;
  do {
    digest = this->hashAlgorithm.hash(hashInput, len);
    
    k = (r < this->hashAlgorithm.hashSize()) ? r : this->hashAlgorithm.hashSize();
    std::memcpy(gen + n - r, digest, k);
    r -= k;
    
    hashInput = digest;
    len = this->hashAlgorithm.hashSize();
  } while (r > 0);
  
  NTL::ZZFromBytes(z, gen, n);
  
  return z;
}
//-----------------------------------------------------------------------------
