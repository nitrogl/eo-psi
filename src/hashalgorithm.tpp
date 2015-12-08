/*
 * Just the interface which guarantees the hash method
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHALGORITHM
#define HASHALGORITHM

template <class T> class HashAlgorithm
{
public:
  virtual ~HashAlgorithm() {}
  virtual char* hash(const T n) = 0;
};

#endif // HASHALGORITHM
