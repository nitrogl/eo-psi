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

/**
 * Abstract template class to inherit in order to act as a hash algorithm.
 * The type of the elements which can be hashed is a generic type.
 */
template <class T> class HashAlgorithm
{
public:
  virtual ~HashAlgorithm() {}
  
  /**
   * Hash a specific value.
   * 
   * @param n The value to be hashed.
   */
  virtual byte* hash(const T n) = 0;
  
  /**
   * Get the hash size in bytes.
   */
  virtual size_t hashSize() const = 0;
  
  /**
   * Get the name of the algorithm which extends this class.
   */
  virtual std::string name() const = 0;
};
//-----------------------------------------------------------------------------

#endif // HASHALGORITHM_TEMPLATE
