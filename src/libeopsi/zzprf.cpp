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
  hashInputSize = 0;
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
  NTL::ZZ z;
  conv(z, rand());
  return z;
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
  size_t k, n, r, rbits, len;
  byte *digerible;
  
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
  if (len > hashInputSize) {
    delete [] hashInput;
    hashInputSize = len < this->hashAlgorithm.hashSize() ? this->hashAlgorithm.hashSize() : len;
    
    try {
      hashInput = new byte[hashInputSize];
    } catch (std::bad_alloc&) {
      std::cerr << "generate(). Error allocating space." << std::endl;
      exit(2);
    }
  }
  
  NTL::BytesFromZZ(hashInput, seed, NTL::bytes(seed));
  std::memcpy(hashInput + NTL::bytes(seed), &index, sizeof(size_t));
  
  r = n;
  k = 0;
  digerible = hashInput;
  do {
    digest = this->hashAlgorithm.hash(digerible, len);
    
    k = (r < this->hashAlgorithm.hashSize()) ? r : this->hashAlgorithm.hashSize();
    std::memcpy(gen + n - r, digest, k);
    r -= k;
    
    digerible = digest;
    len = this->hashAlgorithm.hashSize();
  } while (r > 0);
  
  // Cut bits
  rbits = n*8 - bits;
//   std::cerr << (int) (gen[0]) << " -> ";
//   gen[n - 1] = (gen[n - 1] << rbits) >> rbits; // This does not work --- bug?
  // [[WARNING]] the right order of these might be endian dependend
  gen[n - 1] <<= rbits;
  gen[n - 1] >>= rbits;
//   std::cerr << (int) (gen[0]) << std::endl;
  
  NTL::ZZFromBytes(z, gen, n);
  
  return z;
}
//-----------------------------------------------------------------------------

NTL::ZZ ZZPRF::generate(const size_t index, const size_t bits) {
  return PseudoRandom::generate(index, bits);
}
//-----------------------------------------------------------------------------
