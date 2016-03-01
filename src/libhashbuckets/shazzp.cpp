/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "shazzp.h"
//-----------------------------------------------------------------------------

SHAZZp::SHAZZp(HashFlavour flavour) : SHA<NTL::ZZ_p>(flavour) {}
SHAZZp::~SHAZZp() {}
//-----------------------------------------------------------------------------

byte* SHAZZp::hash(const NTL::ZZ_p n, const size_t len) {
  size_t truncLen;
  zzStreamString.str(std::string()); // Clear stream string
  zzStreamString << n;
  zzString = zzStreamString.str();
  truncLen = len > 0 && len < zzString.length() ? len : zzString.length();
  
  this->sha->CalculateDigest(lastHash, (byte *) zzString.c_str(), truncLen);
  return this->lastHash;
}
//-----------------------------------------------------------------------------

std::string SHAZZp::name() const {
  std::ostringstream sstr;
  sstr << "SHA" << (this->flavour) << " for integers modulo p";
  return sstr.str();
}
//-----------------------------------------------------------------------------

