/*
 * Key generator string -> integers modulo n
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef STRING_STRING_KEY_GENERATOR_H
#define STRING_STRING_KEY_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include <NTL/ZZ_p.h>
#include "zzprandgen.h"
#include "keygen.hpp"
//-----------------------------------------------------------------------------

/**
 * Key generator for big integers modulo p with secret of type string.
 */
class StringKeyGenerator : public KeyGenerator<std::string, std::string>
{
protected:
  size_t length;
  
public:
  StringKeyGenerator();
  virtual ~StringKeyGenerator() {}
  
  virtual void setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm);
  virtual void setSecretKey(const std::string& secret);
  
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
  
  virtual std::string generate(size_t index);
};
//-----------------------------------------------------------------------------

#endif // STRING_ZZ_P_KEY_GENERATOR_H
