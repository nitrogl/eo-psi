/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "shazzp.h"
//-----------------------------------------------------------------------------

SHAZZp::SHAZZp() : SHA<NTL::ZZ_p>() {}
SHAZZp::SHAZZp(HashFlavour flavour) : SHA<NTL::ZZ_p>(flavour) {}
SHAZZp::~SHAZZp() {}
//-----------------------------------------------------------------------------

byte* SHAZZp::hash(const NTL::ZZ_p n) {
  zzStreamString.str(std::string()); // Clear stream string
  zzStreamString << n;
  zzString = zzStreamString.str();
  
  this->sha->CalculateDigest(lastHash, (byte *) zzString.c_str(), zzString.length());
  return this->lastHash;
}
//-----------------------------------------------------------------------------

std::string SHAZZp::name() const {
  std::ostringstream sstr;
  sstr << "SHA" << (this->flavour) << " for integers modulo p";
  return sstr.str();
}
//-----------------------------------------------------------------------------

