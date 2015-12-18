/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "sha.tpp"
#include "shazzp.h"

//-----------------------------------------------------------------------------

SHAZZp::SHAZZp() {
}
//-----------------------------------------------------------------------------

SHAZZp::SHAZZp(HashFlavour flavour) {
}
//-----------------------------------------------------------------------------

SHAZZp::~SHAZZp() {
}
//-----------------------------------------------------------------------------

std::string SHAZZp::name() const {
  std::ostringstream sstr;
  sstr << "SHA" << (this->flavour) << " for integers modulo p";
  return sstr.str();
}
//-----------------------------------------------------------------------------

char* SHAZZp::hash(const NTL::ZZ_p n) {
  std::string hashStr = "";
  
  zzStreamString.str(std::string()); // Clear stream string
  zzStreamString << n;
  zzString = zzStreamString.str();
  
  CryptoPP::StringSource(zzString, true, new CryptoPP::HashFilter(*(this->sha), new CryptoPP::HexEncoder(new CryptoPP::StringSink(hashStr))));
  strncpy(this->lastHash, hashStr.c_str(), hashStr.length());
  
  return this->lastHash;
}
//-----------------------------------------------------------------------------

