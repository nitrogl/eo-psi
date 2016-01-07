/*
 * Key generator string -> integers modulo n
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef STRING_ZZ_P_KEY_GENERATOR_H
#define STRING_ZZ_P_KEY_GENERATOR_H
//-----------------------------------------------------------------------------

#include <string>
#include <NTL/ZZ_p.h>
#include "zzprandgen.h"
#include "keygen.hpp"
#include "strint.h"
//-----------------------------------------------------------------------------

/**
 * Key generator for big integers modulo p with secret of type string.
 */
class StringZZpKeyGenerator : public KeyGenerator<std::string, NTL::ZZ_p>, public RandomZZpGenerator
{
protected:
  StrInt *secretint;
  StrInt *strint;
  
public:
  StringZZpKeyGenerator();
  virtual ~StringZZpKeyGenerator() {}
  
  virtual void setHashAlgorithm(const HashAlgorithm<std::string>* hashAlgorithm);
  virtual void setSecretKey(const std::string& secret);
  
  virtual void setSeed(const NTL::ZZ_p & seed);
  NTL::ZZ_p next();
};
//-----------------------------------------------------------------------------

#endif // STRING_ZZ_P_KEY_GENERATOR_H
