/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_SHA_TEMPLATE
#define HASHALGORITHM_SHA_TEMPLATE
//-----------------------------------------------------------------------------

#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "hashalgorithm.tpp"
#include "hashflavour.h"
//-----------------------------------------------------------------------------

template <class T> class SHA: public HashAlgorithm<T>
{
protected:
  HashFlavour flavour;
  unsigned char *lastHash;
  size_t size;
  CryptoPP::HashTransformation* sha;
  
  void setFlavour(HashFlavour flavour) {
    this->flavour = flavour;
    
    switch (this->flavour) {
      case SHA256_FLAVOUR:
        this->sha = new CryptoPP::SHA256();
        this->size = CryptoPP::SHA256::DIGESTSIZE; // bytes
        break;
      
      case SHA1_FLAVOUR:
      default:
        this->sha = new CryptoPP::SHA1();
        this->size = CryptoPP::SHA1::DIGESTSIZE; // bytes
        break;
    }
    
    lastHash = new unsigned char[this->size];
  }
  
public:
  SHA() : HashAlgorithm<T>() {
    this->setFlavour(DEFAULT_HASH_FLAVOUR);
  }
  SHA(HashFlavour flavour) : HashAlgorithm<T>() {
    this->setFlavour(flavour);
  }
  
  size_t hashSize() const {
    return this->size;
  }
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_TEMPLATE
