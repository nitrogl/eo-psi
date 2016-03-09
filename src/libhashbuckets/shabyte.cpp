/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "shabyte.h"
//-----------------------------------------------------------------------------

SHAByteArray::SHAByteArray(HashFlavour flavour) : SHA(flavour) {
}
//-----------------------------------------------------------------------------

SHAByteArray::~SHAByteArray() {
}
//-----------------------------------------------------------------------------

byte* SHAByteArray::hash(byte * str, const size_t len) {
  this->sha->CalculateDigest(this->lastHash, str, len);
  return this->lastHash;
}
//-----------------------------------------------------------------------------
