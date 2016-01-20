/*
 * Key generator string -> integers modulo n
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <sstream>
#include "stringkeygen.h"
#include "ntlmiss.h"
//-----------------------------------------------------------------------------

StringKeyGenerator::StringKeyGenerator() : KeyGenerator<std::string, std::string>() {
}
//-----------------------------------------------------------------------------

void StringKeyGenerator::setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm) {
  if (hashAlgorithm == nullptr) {
    std::cerr << "setHashAlgorithm(). WARNING: null pointer given as algorithm, nothing to do." << std::endl;
    return;
  }
  
  KeyGenerator<std::string, std::string>::setHashAlgorithm(hashAlgorithm);
  this->length = hashAlgorithm->hashSize();
}
//-----------------------------------------------------------------------------

void StringKeyGenerator::setSecretKey(const std::string& secret) {
  this->secret = secret;
}
//-----------------------------------------------------------------------------

size_t StringKeyGenerator::getLength() const {
  return this->length;
}
//-----------------------------------------------------------------------------

void StringKeyGenerator::setLength(const size_t length) {
  this->length = length;
}
//-----------------------------------------------------------------------------

std::string StringKeyGenerator::generate(size_t index) {
  std::string key = "";
  std::string derived;
  size_t k;
  
  if (hashAlgorithm != nullptr) {
    derived = this->secret + std::to_string(index);
    k = 0;
    do {
      derived = (char *) hashAlgorithm->hash(derived + std::to_string(k));
      key += derived;
    } while (key.length() < this->length);
    key = key.substr(0, this->length);
  } else {
    std::cerr << "setSecretKey(). Warning: a hash algorithm must be set first." << std::endl;
  }
  
  return key;
}
//-----------------------------------------------------------------------------
