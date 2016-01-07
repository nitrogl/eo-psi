/*
 * Key generator string -> integers modulo n
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "strzzpkeygen.h"
#include "ntlmiss.h"
//-----------------------------------------------------------------------------

StringZZpKeyGenerator::StringZZpKeyGenerator() : KeyGenerator<std::string, NTL::ZZ_p>() {
  this->strint = nullptr;
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm) {
  if (hashAlgorithm == nullptr) {
    std::cerr << "setHashAlgorithm(). WARNING: null pointer given as algorithm, nothing to do." << std::endl;
    return;
  }
  
  try {
    secretint = new StrInt(hashAlgorithm->hashSize());
    strint = new StrInt(hashAlgorithm->hashSize());
  } catch (std::bad_alloc&) {
    std::cerr << "setHashAlgorithm(). Fatal error allocating space." << std::endl;
    exit(2);
  }
  
  KeyGenerator<std::string, NTL::ZZ_p>::setHashAlgorithm(hashAlgorithm);
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setSecretKey(const std::string& secret) {
  if (hashAlgorithm != nullptr) {
    byte *hash = hashAlgorithm->hash(secret);
    secretint->set(hash, hashAlgorithm->hashSize());
    strint->set(hash, hashAlgorithm->hashSize());
  } else {
    std::cerr << "setSecretKey(). Warning: a hash algorithm must be set first." << std::endl;
  }
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setSeed(const NTL::ZZ_p& seed) {
  RandomZZpGenerator::setSeed(seed);
}
//-----------------------------------------------------------------------------

NTL::ZZ_p StringZZpKeyGenerator::next() {
  NTL::ZZ_p zp;
  NTL::ZZ z;
  
  if (hashAlgorithm != nullptr && strint != nullptr) {
    strint->inc();
    z = NTL::str2zz(strint->toString());
    rndZZgen.setSeed(z);
    return RandomZZpGenerator::next();
  } else {
    std::cerr << "next(). Warning: a hash algorithm must be set first." << std::endl;
  }
  
  return zp;
}
//-----------------------------------------------------------------------------
