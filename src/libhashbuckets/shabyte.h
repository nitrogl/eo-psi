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
class SHAByteArray//: public SHA<byte *>
{
public:
//   SHAByteArray(HashFlavour flavour = SHA1_FLAVOUR);
//   virtual ~SHAByteArray();
  
  byte* hash(const byte * str, const size_t len = 0);
  std::string name() const;
  
  
  
private:
  size_t size;                       ///< The hash size in bytes.
  
protected:
  HashFlavour flavour;               ///< The flavour of the hash (it can be SHA1, etc..)
  byte *lastHash;                    ///< Cache the last hash
  CryptoPP::HashTransformation* sha; ///< The hash transformation, in accordance with the flavour
  
  /**
   * Set the flavour of the hash algorithm.
   * @warning This function is meant to be used only once-per-instance.
   * 
   * @param flavour The SHA flavour to set (SHA1, SHA-256, and so forth).
   */
  void setFlavour(HashFlavour flavour) {
    this->flavour = flavour;
    
    // Delete previous sha and lastHash
    if (this->sha != nullptr) {
      delete this->sha;
    }
    if (this->lastHash != nullptr) {
      delete this->lastHash;
    }
    
    try {
      switch (this->flavour) {
        case SHA256_FLAVOUR:
          this->sha = new CryptoPP::SHA256();
          this->size = CryptoPP::SHA256::DIGESTSIZE; // bytes
          break;
        
        default:
          std::cerr << "setFlavour(). The given flavour either is not a SHA-compatbile one or is not (yet) implemented." << std::endl;
        case SHA1_FLAVOUR:
          this->sha = new CryptoPP::SHA1();
          this->size = CryptoPP::SHA1::DIGESTSIZE; // bytes
          break;
      }
      
      lastHash = new byte[this->size];
    } catch(std::bad_alloc&) {
      std::cerr << "setFlavour(). Error allocating memory." << std::endl;
      exit(2);
    }
  }
  
public:
  SHAByteArray(HashFlavour flavour = SHA1_FLAVOUR) {
    this->sha = nullptr;
    this->lastHash = nullptr;
    this->setFlavour(flavour);
  }
  
  ~SHAByteArray() {
    if (this->sha != nullptr) {
      delete this->sha;
    }
    if (this->lastHash != nullptr) {
      delete this->lastHash;
    }
  }
  
  /**
   * Get the hash size in bytes.
   * @warning This is the digest size, not the size of its string representation.
   * For example, SHA1 use a size of 20 bytes, but its common string representation makes use of 40 bytes, because the digest uses only 4 bits for the alphanumeric characters.
   */
  size_t hashSize() const {
    return this->size;
  }
  
  static std::string readableDigest(const byte* digest, const size_t digestSize) {
    CryptoPP::HexEncoder encoder;
    std::string output;
    CryptoPP::StringSink *stringSink;
    
    try {
      stringSink = new CryptoPP::StringSink(output);
    } catch(std::bad_alloc&) {
      std::cerr << "readableHash(). Unable to allocate memory." << std::endl;
    }
    
    encoder.Attach(stringSink);
    encoder.Put(digest, digestSize);
    encoder.MessageEnd();
    
    if(encoder.MaxRetrievable())
    {
        output.resize(encoder.MaxRetrievable());
        encoder.Get((byte*) output.data(), output.size());
    }
    
    return output;
  }
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_BYTE_ARRAY_H
