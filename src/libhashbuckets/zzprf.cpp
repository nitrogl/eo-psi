/*
 * A class for pseudo-random big integers using NTL
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */
//-----------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include "zzprf.h"
//-----------------------------------------------------------------------------

ZZPRF::ZZPRF() : PseudoRandom() {
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

NTL::ZZ ZZPRF::randomSeed() {
  return NTL::RandomBits_ZZ(rand());
}
//-----------------------------------------------------------------------------

NTL::ZZ ZZPRF::defaultSeed() {
  NTL::ZZ defSeed;
  conv(defSeed, 0);
  return defSeed;
}
//-----------------------------------------------------------------------------

NTL::ZZ ZZPRF::generate(const NTL::ZZ seed, const size_t index, const size_t bits) {
  NTL::ZZ z;
  byte *digest;
  size_t k, n, r, len;
  
  if (bits == 0) {
    int rnd;
    while ((rnd = rand()) == 0);
    n = NTL::bytes(rnd);
  } else {
    n = NTL::bytes(bits);
  }
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

NTL::ZZ ZZPRF::generate(const size_t index, const size_t bits) { 
  return PseudoRandom::generate(index, bits);
}
//-----------------------------------------------------------------------------
