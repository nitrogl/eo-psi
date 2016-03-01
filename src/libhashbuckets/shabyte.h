/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_SHA_BYTE_ARRAY_H
#define HASHALGORITHM_SHA_BYTE_ARRAY_H
//-----------------------------------------------------------------------------

#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "hashalgorithm.hpp"
#include "sha.hpp"
//-----------------------------------------------------------------------------

/**
 * SHA algorithm hashing strings
 */
class SHAByteArray: public SHA<byte *>
{
public:
  SHAByteArray(HashFlavour flavour = SHA1_FLAVOUR);
  virtual ~SHAByteArray();
  
  byte* hash(const byte * str, const size_t len);
  std::string name() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_BYTE_ARRAY_H
