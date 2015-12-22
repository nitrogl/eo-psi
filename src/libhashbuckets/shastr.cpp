/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "shastr.h"
//-----------------------------------------------------------------------------

SHAString::SHAString(HashFlavour flavour) : SHA<std::string>(flavour) {}
SHAString::~SHAString() {}
//-----------------------------------------------------------------------------

byte* SHAString::hash(const std::string str) {
  this->sha->CalculateDigest(lastHash, (byte *) str.c_str(), str.length());
  return this->lastHash;
}
//-----------------------------------------------------------------------------

std::string SHAString::name() const {
  return "SHA" + this->flavour;
}
//-----------------------------------------------------------------------------
