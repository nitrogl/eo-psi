/*
 * A generic hash table abstract class template with only the operations
 *  - insertion
 *  - search
 * Deletion is not covered.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASHTABLE_TEMPLATE
#define HASHTABLE_TEMPLATE

#include <cstdlib>
#include "hashalgorithm.tpp"

template <class T, class U> class HashTable
{
protected:
  size_t n = 0; // number of entries (size)
  size_t k = 0; // number of buckets
  U* buckets = nullptr;
  
  HashAlgorithm<T>* hashAlgorithm = nullptr;
  
public:
  virtual ~HashTable() {}
  
  virtual void add(T element) = 0;
  virtual void addToBucket(T element, int i) = 0;
  
  void add(T* list, int dim) {
    for (int i = 0; i < dim; i++) {
      add(list[i]);
    }
  };
  
  size_t size() {
    return n;
  }
  
  U* getBucket(int i) {
    return (i >= 0 && i < k) ? (buckets + i) : nullptr;
  }
  
  void setHashAlgorithm(HashAlgorithm<T>* hashAlgorithm) {
    this->hashAlgorithm = hashAlgorithm;
  }
};

#endif // HASHTABLE_TEMPLATE
