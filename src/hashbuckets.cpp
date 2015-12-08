/*
 * Implementation of a hash table with buckets as element containers.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "hashbuckets.h"
//-----------------------------------------------------------------------------

template <class T> HashBuckets<T>::HashBuckets(size_t length, size_t maxLoad) {
  this->k = length;
  this->maxLoad = maxLoad;
  
  this->buckets = new Bucket<T>[length];
  if (this->buckets == nullptr) {
    std::cerr << "HashBuckets()" << ". Unable to initialise buckets." << std::endl;
  }
};
//-----------------------------------------------------------------------------

template <class T> HashBuckets<T>::~HashBuckets() {
  //delete(this->buckets);
}
//-----------------------------------------------------------------------------

template <class T> void HashBuckets<T>::add(T element) {
  char *hash;
  strint128 h128;
  size_t i, index;
  
  hash = this->hashAlgorithm->hash(element);
  for (i = 0; i < 16; i++) {
    h128.str[i] = hash[i];
  }
  this->hash128.set(h128);
  index = this->hash128.rem(this->k);
//   if (index == 0)
//   std::cerr << "HashBuckets<T>::addToBucket()" << ". Index of " << element << " is " << index << "." << std::endl; // Debug purposes
  this->buckets[index].push_back(element);
};
//-----------------------------------------------------------------------------

template <class T> void HashBuckets<T>::addToBucket(T element, int i) {
  if (i >= 0 && i < this->k) {
    this->buckets[i].push_back(element);
  } else {
    std::cerr << "HashBuckets<T>::addToBucket()" << ". Index out of bound." << std::endl;
  }
};
//-----------------------------------------------------------------------------

template <class T> size_t HashBuckets<T>::getLength() const {
  return this->k;
};
//-----------------------------------------------------------------------------

template <class T> size_t HashBuckets<T>::getMaxLoad() const {
  return this->maxLoad;
};
//-----------------------------------------------------------------------------

template <class T> void HashBuckets<T>::printStats(bool full) const {
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
  std::cout << "Total elements. N = " << total << std::endl;
  std::cout << "Minimum size.   min = " << minSize << " (bucket " << minBucket << ")" << std::endl;
  std::cout << "Maximum size.   max = " << maxSize << " (bucket " << maxBucket << ")" << std::endl;
  std::cout << "Average.   m = " << avgSize << std::endl;
  std::cout << "Variance.  v = " << varSize << std::endl;
  std::cout << "Std dev.   d = " << sqrt(varSize) << std::endl;
  std::cout << std::endl;
  
  // Show each buckets
  nshow = full ? this->k : 50;
  for (size_t i = 0; i < nshow; i++) {
    std::cout << "(" << i << ", " << this->buckets[i].size() << ") \t";
    if (i % 10 == 9) {
      std::cout << std::endl;
    }
  }
  std::cout << (full ? "" : "...") << std::endl;
};
//-----------------------------------------------------------------------------

// Explicit template instances
#include <NTL/ZZ_p.h>
template class HashBuckets<NTL::ZZ_p>;