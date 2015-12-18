/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef KEYGEN_TEMPLATE
#define KEYGEN_TEMPLATE
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
    memory.reserve(10);
  }
  virtual ~KeyGenerator();
  
  void setHashAlgorithm(const HashAlgorithm hashAlgorithm) const;
  T next() const {
    
  }
};
//-----------------------------------------------------------------------------

#endif // KEYGEN_TEMPLATE
