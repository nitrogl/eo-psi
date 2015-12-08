/*
 * Implementation of a hash table with buckets as element containers.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHBUCKETS_H
#define HASHBUCKETS_H
//-----------------------------------------------------------------------------

#include "hashtable.tpp"
#include "bucket.h"
#include "strint.h"
//-----------------------------------------------------------------------------

template <class T> class HashBuckets: public HashTable<T, Bucket<T>>
{
protected:
  StrInt128 hash128;
  size_t maxLoad; // maximum (fixed) length for each bucket
  
public:
  HashBuckets(size_t length, size_t maxLoad);
  virtual ~HashBuckets();
  
  size_t getLength() const;
  size_t getMaxLoad() const;
  
  void add(T element);
  void addToBucket(T element, int i);
  
  void printStats(bool full = false) const;
};
//-----------------------------------------------------------------------------

#endif // HASHBUCKETS_H
