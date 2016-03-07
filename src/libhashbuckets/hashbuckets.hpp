/*
 * Implementation of a hash table with buckets as element containers.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHBUCKETS_TEMPLATE
#define HASHBUCKETS_TEMPLATE
//-----------------------------------------------------------------------------

#include <iostream>
#include "hashtable.hpp"

#include "markedvector.hpp"
#include "randgen.hpp"
#include "strint.h"
#include "prf.hpp"

#define DEFAULT_NSHOW 30
//-----------------------------------------------------------------------------

/**
 * This is an implementation of a hash table with specified maximum load for
 * each bucket and a fixed number of buckets.
 * Introspectively, it can do some statistics on its state.
 * Also a conceal method is given to fill all the not-full buckets, <i>concealing</i>
 * the original added elements.
 */
template <typename T> class HashBuckets: public HashTable<T, MarkedVector<T>>
{
protected:
  StrInt *hashStrInt; ///< This is basically used to get the remainder of a string.
  size_t maxLoad;     ///< The maximum (fixed) length for each bucket
  
public:
  /**
   * Initialise the hash table with a specified number of buckets and 
   * a specified number of cells for each bucket.
   * 
   * @param length The number of buckets.
   * @param maxLoad The maximum number of elements for each bucket.
   */
  HashBuckets(size_t length, size_t maxLoad, const HashAlgorithm<T>* hashAlgorithm = nullptr);
  virtual ~HashBuckets();
  
  /**
   * Get the number of buckets.
   */
  size_t getLength() const;
  
  /**
   * Get the maximum elements to store in each bucket.
   */
  size_t getMaxLoad() const;
  
  void add(const T& element) throw (OutOfBoundException);
  void addToBucket(const T& element, size_t i) throw (OutOfBoundException);
  void setHashAlgorithm(const HashAlgorithm<T>* hashAlgorithm);
  
  /**
   * Fill all the not-full buckets with random elements,
   * <i>concealing</i> the originally added elements.
   * 
   * @param prf The generator of random elements.
   */
  void conceal(PseudoRandom<T, T>& prf);
  
  /**
   * Do some introspective statistics on its state.
   * 
   * @param full Print stats of all the buckets (even if they are a lot).
   */
  void printStats(bool full = false) const;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template <typename T> HashBuckets<T>::HashBuckets(size_t length, size_t maxLoad, const HashAlgorithm<T>* hashAlgorithm) {
  this->k = length;
  this->maxLoad = maxLoad;
  this->hashStrInt = nullptr;
  this->setHashAlgorithm(hashAlgorithm);
  
  try {
    this->buckets = new MarkedVector<T>[length];
  } catch (std::bad_alloc&) {
    std::cerr << "HashBuckets(). Fatal error allocating space." << std::endl;
    exit(2);
  }
  
  // Reserving space enhances efficiency (it sped up! cool!! ~0.1 seconds of total)
  for (size_t i = 0; i < length; i++) {
    // Reserving 3/4 of maxLoad
    this->buckets[i].reserve(1 + 3 * maxLoad / 4);
  }
}
//-----------------------------------------------------------------------------

template <typename T> HashBuckets<T>::~HashBuckets() {
  delete [] this->buckets;
  if (this->hashStrInt != nullptr) {
    delete this->hashStrInt;
  }
}
//-----------------------------------------------------------------------------

template <typename T> void HashBuckets<T>::setHashAlgorithm(const HashAlgorithm<T>* hashAlgorithm) {
  if (hashAlgorithm == nullptr) {
    std::cerr << "setHashAlgorithm(). WARNING: null pointer given as algorithm, nothing to do." << std::endl;
    return;
  }
  
  try {
    hashStrInt = new StrInt(hashAlgorithm->hashSize());
  } catch (std::bad_alloc&) {
    std::cerr << "setHashAlgorithm(). Fatal error allocating space." << std::endl;
    exit(2);
  }
  this->hashAlgorithm = (HashAlgorithm<T>*) hashAlgorithm;
}
//-----------------------------------------------------------------------------

template <typename T> void HashBuckets<T>::add(const T& element) throw (OutOfBoundException) {
  size_t index;
  
  if (this->hashStrInt != nullptr) {
    this->hashStrInt->set(this->hashAlgorithm->hash(element), this->hashAlgorithm->hashSize());
    index = this->hashStrInt->rem(this->k);
    
    if (this->buckets[index].size() == this->maxLoad) {
      throw OutOfBoundException("HashBuckets<T>::addToBucket(). MarkedVector full.", OutOfBoundException::FATAL);
    }
  } else {
    index = 0;
    std::cerr << "HashBuckets<T>::add()" << ". Element index completely invented (you did not set a hash algorithm)." << std::endl;
  }
    
  //   if (index == 0)
  //   std::cerr << "HashBuckets<T>::add()" << ". Index of " << element << " is " << index << "." << std::endl; // Debug purposes
    this->buckets[index].push_back(element);
}
//-----------------------------------------------------------------------------

template <typename T> void HashBuckets<T>::addToBucket(const T& element, const size_t i) throw (OutOfBoundException) {
  size_t index = i;
  if (index < this->k) {
    this->buckets[index].push_back(element);
  } else {
    throw OutOfBoundException("HashBuckets<T>::addToBucket(). Index out of bound.", OutOfBoundException::FATAL);
  }
}
//-----------------------------------------------------------------------------

template <typename T> void HashBuckets<T>::conceal(PseudoRandom<T, T>& pr) {
  size_t i, j;
  
  // Mark buckets (this tracks empty buckets)
  for (i = 0; i < this->k; i++) {
    if (this->buckets[i].empty()) {
      this->buckets[i].mark();
    }
    
    // Fill empty cells
    for (j = this->buckets[i].size(); j < maxLoad; j++) {
      this->buckets[i][j] = pr.generate(j);
    }
  }
}
//-----------------------------------------------------------------------------

template <typename T> size_t HashBuckets<T>::getLength() const {
  return this->k;
}
//-----------------------------------------------------------------------------

template <typename T> size_t HashBuckets<T>::getMaxLoad() const {
  return this->maxLoad;
}
//-----------------------------------------------------------------------------

template <typename T> void HashBuckets<T>::printStats(bool full) const {
  size_t minSize, minBucket, maxSize, maxBucket, total, diff, nshow;
  double avgSize, varSize;
  
  // Integer stuff
  total = minSize = maxSize = this->buckets[0].size();
  minBucket = maxBucket = 0;
  for (size_t i = 1; i < this->k; i++) {
    if (this->buckets[i].size() < minSize) {
      minSize = this->buckets[i].size();
      minBucket = i;
    }
    if (this->buckets[i].size() > maxSize) {
      maxSize = this->buckets[i].size();
      maxBucket = i;
    }
    total += this->buckets[i].size();
  }
  
  // Momentum 1 and 2
  avgSize = (double) total / this->k;
  varSize = 0.0;
  for (size_t i = 0; i < this->k; i++) {
    diff = (avgSize > this->buckets[i].size()) ? avgSize - this->buckets[i].size() : this->buckets[i].size() - avgSize;
    varSize += (double) diff * diff / this->k;
  }
  
  // Print stats
  std::cout << "HashBuchets - " << this->k << " buckets of size " << this->maxLoad << std::endl;
  std::cout << "Total elements. N = " << total << " (" << total * 100. / this->k / this->maxLoad << "%)" << std::endl;
  std::cout << "Minimum size.   min = " << minSize << " (bucket " << minBucket << ")" << std::endl;
  std::cout << "Maximum size.   max = " << maxSize << " (bucket " << maxBucket << ")" << std::endl;
  std::cout << "Average.   m = " << avgSize << std::endl;
  std::cout << "Variance.  v = " << varSize << std::endl;
  std::cout << "Std dev.   d = " << sqrt(varSize) << std::endl;
  std::cout << std::endl;
  
  // Show each buckets
  nshow = full || (this->k < DEFAULT_NSHOW) ? this->k : DEFAULT_NSHOW;
  for (size_t i = 0; i < nshow; i++) {
    std::cout << "(" << i << ", " << this->buckets[i].size() << ")  \t";
    if (i % 5 == 4) {
      std::cout << std::endl;
    }
  }
  std::cout << (full || (this->k < DEFAULT_NSHOW) ? "" : "...") << std::endl;
}
//-----------------------------------------------------------------------------

#endif // HASHBUCKETS_TEMPLATE
