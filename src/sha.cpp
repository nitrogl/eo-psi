/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "sha.h"

//-----------------------------------------------------------------------------

SHA::SHA() {
  this->flavour = DEFAULT_SHA_FLAFOUR;
  this->hashSize = 0;
}
//-----------------------------------------------------------------------------

SHA::SHA(SHAFlavour flavour) {
  this->flavour = flafour;
  this->hashSize = 0;
}
//-----------------------------------------------------------------------------

SHA::~SHA() {
}
//-----------------------------------------------------------------------------

char* SHA::hash(const std::string str) {
  CryptoPP::HashTransformation* sha;
  
  switch (this->flavour) {
    case SHA256_FLAVOUR:
      sha = new CryptoPP::SHA256();
      this->hashSize = 40;
      break;
    
    case SHA1_FLAVOUR:
      sha = new CryptoPP::SHA1();
      this->hashSize = 64;
    default:
      break;
  }
  
  CryptoPP::StringSource(str, true, new CryptoPP::HashFilter(*sha, new CryptoPP::HexEncoder(new CryptoPP::StringSink(this->lastHash))));
  
  return this->lastHash;
}
//-----------------------------------------------------------------------------

size_t SHA::getHashSize() const {
  return this->hashSize;
}
//-----------------------------------------------------------------------------

