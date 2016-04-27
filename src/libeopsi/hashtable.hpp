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
#include "hashalgorithm.hpp"
#include "outofboundex.h"

/**
 * A generic hash table abstract class template with only the operations
 *  - insertion (add)
 *  - search (getBucket)
 * Deletion is not covered.
 */
template <typename T, typename U>
class HashTable
{
protected:
  size_t n = 0;                              ///< The number of entries (size)
  size_t k = 0;                              ///< The number of buckets
  U* buckets = nullptr;                      ///< The buckets to fill
  HashAlgorithm<T>* hashAlgorithm = nullptr; ///< The hash algorithm should determine the index
  
public:
  virtual ~HashTable() {}
  
  /**
   * Add a single element to the hash table, its key should be determined
   * by the hash algorithm.
   * 
   * @param element the element to add to the collection
   */
  virtual void add(const T& element) = 0;
  
  /**
   * Add a single element to the hash table to a specific bucket (regardless
   * of its real index which could be found using the hash algorithm).
   * 
   * @param element the element to add
   * @param i the index of the bucket
   */
  virtual void addToBucket(const T& element, const size_t i) = 0;
  
  /**
   * Add a number of elements to the hash table from an array.
   * 
   * @param list the array of elements to add
   * @param dim the number of elements in the array to add
   */
  void add(T* list, size_t dim) {
    for (size_t i = 0; i < dim; i++) {
      add(list[i]);
    }
  };
  
  /**
   * The size of the collection.
   * 
   * @return the number of elements added to the collection
   */
  size_t size() {
    return n;
  }
  
  /**
   * Get a specific bucket by its index.
   * 
   * @param i the index of the bucket to get
   */
  U getBucket(size_t i) throw (OutOfBoundException) {
    return (i >= 0 && i < k) ? buckets[i] : throw OutOfBoundException("getBucket(). Index out of bounds.", OutOfBoundException::FATAL);;
  }
  
  /**
   * Set the hash algorithm to use to compute the index of the buckets
   * 
   * @param hashAlgorithm the hash algorithm to use
   */
  void setHashAlgorithm(const HashAlgorithm<T>& hashAlgorithm) {
    this->hashAlgorithm = hashAlgorithm;
  }
  
  /**
   * This operator gives back the bucket at the index embraced by the
   * square brackets.
   * @see getBucket
   */
  const U operator[](size_t index) {
    return this->getBucket(index);
  }
};

#endif // HASHTABLE_TEMPLATE
