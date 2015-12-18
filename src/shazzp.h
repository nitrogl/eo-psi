/*
 * Class uses SHA-X, extending HashAlgorithm
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_SHA_ZZ_P_H
#define HASHALGORITHM_SHA_ZZ_P_H
//-----------------------------------------------------------------------------

#include <sstream>
#include <NTL/ZZ_p.h>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include "hashalgorithm.tpp"
#include "sha.tpp"
//-----------------------------------------------------------------------------

class SHAZZp: public SHA<NTL::ZZ_p>
{
private:
  std::stringstream zzStreamString;
  std::string zzString;
  
public:
  SHAZZp();
  SHAZZp(HashFlavour flavour);
  virtual ~SHAZZp();
  
  char* hash(const NTL::ZZ_p str);
  std::string name() const;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_SHA_ZZ_P_H
