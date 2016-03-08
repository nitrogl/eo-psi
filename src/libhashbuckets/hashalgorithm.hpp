/*
 * Just the interface which guarantees the hash method
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_TEMPLATE
#define HASHALGORITHM_TEMPLATE
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "byte.h"
//-----------------------------------------------------------------------------

/**
 * Abstract template class to inherit in order to act as a hash algorithm.
 * The type of the elements which can be hashed is a generic type.
 */
template <typename T>
class HashAlgorithm
{
public:
  virtual ~HashAlgorithm() {}
  
  /**
   * Hash a specific value.
   * 
   * @param n The value to hash.
   * @param len The length of the value to hash.
   */
  virtual byte* hash(T n, const size_t len = 0) = 0;
  
  /**
   * Get the hash size in bytes.
   */
  virtual size_t hashSize() const = 0;
  
  /**
   * Get the name of the algorithm which extends this class.
   */
  virtual std::string name() const = 0;
  
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

#endif // HASHALGORITHM_TEMPLATE
