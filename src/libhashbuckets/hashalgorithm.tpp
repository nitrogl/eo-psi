/*
 * Just the interface which guarantees the hash method
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM_TEMPLATE
#define HASHALGORITHM_TEMPLATE
//-----------------------------------------------------------------------------

#ifndef byte
typedef unsigned char byte;
#endif
//-----------------------------------------------------------------------------

#include <string>
//-----------------------------------------------------------------------------

template <class T> class HashAlgorithm
{
public:
  virtual ~HashAlgorithm() {}
  virtual byte* hash(const T n) = 0;
  virtual size_t hashSize() const = 0;
  virtual std::string name() const = 0;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_TEMPLATE