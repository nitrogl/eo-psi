/*
 * Key generator string -> byte array
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef STRING_STRING_KEY_GENERATOR_H
#define STRING_STRING_KEY_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include "byte.h"
#include "keygen.hpp"
//-----------------------------------------------------------------------------

/**
 * Key generator for big integers modulo p with secret of type string.
 */
class ByteKeyGenerator : public KeyGenerator<std::string, byte *>
{
protected:
  size_t length;
  
public:
  ByteKeyGenerator();
  virtual ~ByteKeyGenerator();
  
  virtual void setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm);
  
  /**
   * Get the length of the generation.
   */
  size_t getLength() const;
  
  /**
   * Set the length of the generation.
   * 
   * @param p The modulo p.
   */
  void setLength(const size_t length);
  
  virtual byte* generate(const size_t index);
};
//-----------------------------------------------------------------------------

#endif // STRING_ZZ_P_KEY_GENERATOR_H
