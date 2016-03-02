/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "shabyte.h"
//-----------------------------------------------------------------------------

// SHAByteArray::SHAByteArray(HashFlavour flavour) : SHA<byte *>(flavour) {
// }
// //-----------------------------------------------------------------------------
// 
// SHAByteArray::~SHAByteArray() {
// }
// //-----------------------------------------------------------------------------

byte* SHAByteArray::hash(const byte * str, const size_t len) {
  this->sha->CalculateDigest(lastHash, str, len);
  return this->lastHash;
}
//-----------------------------------------------------------------------------

std::string SHAByteArray::name() const {
  return "SHA" + this->flavour;
}
//-----------------------------------------------------------------------------
