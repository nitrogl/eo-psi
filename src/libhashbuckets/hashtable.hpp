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

/**
 * Abstract class template with generic types specifying the behaviour of a hash table.
 * Hash table of a generic T elements, stored in U-type containers as hash buckets.
 */
template <class T, class U> class HashTable
{
protected:
  size_t n = 0;                              ///< Number of entries (size)
  size_t k = 0;                              ///< Number of buckets
  U* buckets = nullptr;                      ///< Array of buckets
  HashAlgorithm<T>* hashAlgorithm = nullptr; ///< The hash algorithm used to hash the elements (the hash should determine which bucket to store the corresponding element).
  
public:
  virtual ~HashTable() {}
  
  /**
   * Add an element to the hash table.
   * 
   * @param element the element to add to the hash table.
   */
  virtual void add(T element) = 0;
  
  /**
   * Add an element to the hash table to a specific bucket.
   * 
   * @param element the element to add to the hash table.
   * @param i The index/key which determines the bucket to store the element to.
   */
  virtual void addToBucket(T element, int i) = 0;
  
  /**
   * Add a list of elements to the hash table.
   * 
   * @param list An array of elements to add.
   * @param dim The length of the list of elements to add.
   */
  void add(T* list, int dim) {
    for (int i = 0; i < dim; i++) {
      add(list[i]);
    }
  };
  
  /**
   * Get the number of entries stored.
   */
  size_t size() {
    return n;
  }
  
  /**
   * Get a specific bucket of the hash table.
   * 
   * @param i Index/key of the bucket.
   */
  U* getBucket(size_t i) {
    return (i >= 0 && i < this->k) ? (buckets + i) : nullptr;
  }
  
  /**
   * Set a specific hash algorithm to be used to index the elements to add.
   * 
   * @param hashAlgorithm The hash algorithm to set.
   */
  void setHashAlgorithm(HashAlgorithm<T>& hashAlgorithm) {
    this->hashAlgorithm = hashAlgorithm;
  }
};

#endif // HASHTABLE_TEMPLATE
