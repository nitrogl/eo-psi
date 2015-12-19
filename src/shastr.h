/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_SHA_H
#define HASHALGORITHM_SHA_H
//-----------------------------------------------------------------------------

#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "hashalgorithm.tpp"
#include "sha.tpp"
//-----------------------------------------------------------------------------

class SHAString: public SHA<std::string>
{
public:
  SHAString();
  SHAString(HashFlavour flavour);
  virtual ~SHAString();
  
  unsigned char* hash(const std::string str);
  std::string name() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_H
