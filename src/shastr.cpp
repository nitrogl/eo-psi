/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "sha.tpp"
#include "shastr.h"

//-----------------------------------------------------------------------------

SHAString::SHAString() : SHA<std::string>() {
}
//-----------------------------------------------------------------------------

SHAString::SHAString(HashFlavour flavour) : SHA<std::string>(flavour) {
}
//-----------------------------------------------------------------------------

char* SHAString::hash(const std::string str) {
  std::string hashStr = "";
  
  CryptoPP::StringSource(str, true, new CryptoPP::HashFilter(*sha, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hashStr))));
  strncpy(this->lastHash, hashStr.c_str(), hashStr.length());
  
  return this->lastHash;
}
//-----------------------------------------------------------------------------

std::string SHAString::name() const {
  return "SHA" + this->flavour;
}
//-----------------------------------------------------------------------------
