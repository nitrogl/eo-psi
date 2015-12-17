/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef KEYGEN_H
#define KEYGEN_H
//-----------------------------------------------------------------------------

#include <vector>
#include "hashalgorithm.h"
//-----------------------------------------------------------------------------

template <class T, class U> class KeyGenerator
{
private:
  HashAlgorithm<T> hashAlgorithm;
  T seed;
  
public:
  KeyGenerator() {
    memory.reserve(1 + 3 * maxLoad / 4);
  }
  virtual ~KeyGenerator();
  
  void setHashAlgorithm(const HashAlgorithm hashAlgorithm) const;
  T next() const {
    
  }
};
//-----------------------------------------------------------------------------

#endif // KEYGEN_H
