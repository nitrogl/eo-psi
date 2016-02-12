/*
 * Key generator string -> byte array
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <sstream>
#include "bytekeygen.h"
#include "ntlmiss.h"
#include "strint.h"
//-----------------------------------------------------------------------------

ByteKeyGenerator::ByteKeyGenerator() : KeyGenerator<std::string, byte *>() {
}
//-----------------------------------------------------------------------------

ByteKeyGenerator::~ByteKeyGenerator() {
}
//-----------------------------------------------------------------------------

void ByteKeyGenerator::setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm) {
  if (hashAlgorithm == nullptr) {
    std::cerr << "setHashAlgorithm(). WARNING: null pointer given as algorithm, nothing to do." << std::endl;
    return;
  }
  
  KeyGenerator<std::string, byte *>::setHashAlgorithm(hashAlgorithm);
  this->length = hashAlgorithm->hashSize();
}
//-----------------------------------------------------------------------------

void ByteKeyGenerator::setSecretKey(const std::string& secret) {
  this->secret = secret;
}
//-----------------------------------------------------------------------------

size_t ByteKeyGenerator::getLength() const {
  return this->length;
}
//-----------------------------------------------------------------------------

void ByteKeyGenerator::setLength(const size_t length) {
  this->length = length;
}
//-----------------------------------------------------------------------------

byte * ByteKeyGenerator::generate(size_t index) {
  byte* key;
  byte* derived;
  size_t i, k;
  StrInt kStrInt(sizeof(size_t));
  
  if (hashAlgorithm == nullptr) {
    std::cerr << "generate(). Warning: a hash algorithm must be set first." << std::endl;
    return nullptr;
  }
  
  try {
    key = new byte[this->length];
  } catch (std::bad_alloc&) {
    std::cerr << "generate(). Error allocating space." << std::endl;
    exit(2);
  }
  
  derived = (byte *) &(this->secret[0]);
  kStrInt.set(index);
  do {
    derived = hashAlgorithm->hash(kStrInt.toString() + (char *) derived);
    for (i = 0; i < hashAlgorithm->hashSize() && k < this->length; i++) {
      key[k++] = derived[i];
      kStrInt.inc();
    }
  } while (k < this->length);
    
  return key;
}
//-----------------------------------------------------------------------------
