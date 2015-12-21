/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef KEYGEN_TEMPLATE
#define KEYGEN_TEMPLATE
//-----------------------------------------------------------------------------

#include <vector>
#include "hashalgorithm.tpp"
//-----------------------------------------------------------------------------

template <class T, class U> class KeyGenerator
{
protected:
  HashAlgorithm<T> hashAlgorithm;
  T seed;
  vector<U> memory;
  
public:
  KeyGenerator() {
    memory.reserve(10);
  }
  virtual ~KeyGenerator();
  
  void setHashAlgorithm(const HashAlgorithm<T> hashAlgorithm) const;
  T next() const {
    
  }
};
//-----------------------------------------------------------------------------

#endif // KEYGEN_TEMPLATE
