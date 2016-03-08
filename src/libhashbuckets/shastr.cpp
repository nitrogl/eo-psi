/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "shastr.h"
//-----------------------------------------------------------------------------

SHAString::SHAString(HashFlavour flavour) : SHA<std::string>(flavour) {
}
//-----------------------------------------------------------------------------

SHAString::~SHAString() {
}
//-----------------------------------------------------------------------------

byte* SHAString::hash(std::string str, const size_t len) {
  size_t truncLen = len > 0 && len < str.length() ? len : str.length();
  this->sha->CalculateDigest(lastHash, (byte *) str.c_str(), truncLen);
  return this->lastHash;
}
//-----------------------------------------------------------------------------
