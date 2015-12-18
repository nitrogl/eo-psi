/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "sha.tpp"
#include "shastr.h"

//-----------------------------------------------------------------------------

SHAString::SHAString() {
}
//-----------------------------------------------------------------------------

SHAString::SHAString(HashFlavour flavour) {
}
//-----------------------------------------------------------------------------

SHAString::~SHAString() {
}
//-----------------------------------------------------------------------------

char* SHAString::hash(const std::string str) {
  CryptoPP::HashTransformation* sha;
  std::string hashStr = "";
  
  switch (this->flavour) {
    case SHA256_FLAVOUR:
      sha = new CryptoPP::SHA256();
      this->hashSize = 40;
      break;
    
    case SHA1_FLAVOUR:
    default:
      sha = new CryptoPP::SHA1();
      this->hashSize = 64;
      break;
  }
  
  CryptoPP::StringSource(str, true, new CryptoPP::HashFilter(*sha, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hashStr))));
  strncpy(this->lastHash, hashStr.c_str(), hashStr.length());
  
  return this->lastHash;
}
//-----------------------------------------------------------------------------

std::string SHAString::name() const {
  return "SHA" + this->flavour;
}
//-----------------------------------------------------------------------------
