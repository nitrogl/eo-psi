/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_SHA_H
#define HASHALGORITHM_SHA_H
//-----------------------------------------------------------------------------

#include <string>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "hashalgorithm.tpp"

#define DEFAULT_SHA_FLAFOUR SHA1_FLAFOUR
//-----------------------------------------------------------------------------

typedef enum SHAFlavour {
  SHA1_FLAFOUR = 0,
  SHA256_FLAFOUR,
} SHAFlavour;
//-----------------------------------------------------------------------------

class SHA: public HashAlgorithm<std::string>
{
private:
  SHAFlavour flavour;
  char *lastHash;
  size_t hashSize;
  
public:
  SHA(SHAFlavour flavour);
  virtual ~SHA();
  
  char* hash(const std::string str);
  size_t getHashSize() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_H
