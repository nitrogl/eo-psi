/*
 * Key generator string -> integers modulo n
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <string>
#include "strzzpkeygen.h"
#include "ntlmiss.h"
//-----------------------------------------------------------------------------

StringZZpKeyGenerator::StringZZpKeyGenerator() : KeyGenerator<std::string, NTL::ZZ_p>() {
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm) {
  if (hashAlgorithm == nullptr) {
    std::cerr << "setHashAlgorithm(). WARNING: null pointer given as algorithm, nothing to do." << std::endl;
    return;
  }
  
  this->byteKeyGenerator.setHashAlgorithm(hashAlgorithm);
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setSecretKey(const std::string secret) {
  this->secret = secret;
  this->byteKeyGenerator.setSecretKey(secret);
}
//-----------------------------------------------------------------------------

NTL::ZZ StringZZpKeyGenerator::getModulo() const {
  return this->p;
}
//-----------------------------------------------------------------------------

void StringZZpKeyGenerator::setModulo(const NTL::ZZ &p) {
  this->p = p;
  this->byteKeyGenerator.setLength(NTL::NumBits(p));
}
//-----------------------------------------------------------------------------


NTL::ZZ_p StringZZpKeyGenerator::generate(const size_t index) {
  NTL::ZZ_p z;
  conv(z, NTL::ZZFromBytes(this->byteKeyGenerator[index], this->byteKeyGenerator.getLength()));
  return z;
}
//-----------------------------------------------------------------------------
