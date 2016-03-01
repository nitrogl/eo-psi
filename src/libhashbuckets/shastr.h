/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_SHA_STRING_H
#define HASHALGORITHM_SHA_STRING_H
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
class SHAString: public SHA<std::string>
{
public:
  SHAString(HashFlavour flavour = SHA1_FLAVOUR);
  virtual ~SHAString();
  
  byte* hash(const std::string str, const size_t len = 0);
  std::string name() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_STRING_H
