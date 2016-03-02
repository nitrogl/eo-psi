/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef KEYGEN_TEMPLATE
#define KEYGEN_TEMPLATE
//-----------------------------------------------------------------------------

#include "hashalgorithm.hpp"
#include "randgen.hpp"
//-----------------------------------------------------------------------------

/**
 * This class acts as a key generator.
 * The secret key of type S of this generator changes the seed is supposed to
 * work together with some index to generate the key corresponding to the
 * index.
 */
template <class S, class T> class KeyGenerator
{
protected:
  HashAlgorithm<S>* hashAlgorithm; ///< The hash algorithm to use for the generated keys.
  S secret;                        ///< The secret key
  size_t index;                    ///< Internal index
  
public:
  KeyGenerator() {
    this->hashAlgorithm = nullptr;
    this->index = 0;
  }
  
  KeyGenerator(const S& secret) : KeyGenerator() {
    this->setSecretKey(secret);
  }
  KeyGenerator(const S& secret, const HashAlgorithm<S>* hashAlgorithm) : KeyGenerator(secret) {
    this->setHashAlgorithm(hashAlgorithm);
  }
  virtual ~KeyGenerator() {};
  
  /**
   * Set the hash algorithm to use to generate new keys.
   * 
   * @param hashAlgorithm the hash algorithm to use
   */
  virtual void setHashAlgorithm(const HashAlgorithm<S>* hashAlgorithm) {
    this->hashAlgorithm = (HashAlgorithm<S>*) hashAlgorithm;
  }
  
  /**
   * The secret for the generation.
   * WARNING: A side effect of this method is that it resets the internal index
   * 
   * @param secret the secret key of the generator
   */
  virtual void setSecretKey(const S secret) {
    this->secret = secret;
    this->index = 0;
  }
  
  /**
   * Reset the internal index
   */
  virtual void reset() {
    this->index = 0;
  }
  
  /**
   * Get the next key generated according to the internal index incrementing it
   * 
   * @return the next key
   */
  virtual T next() {
    return generate(this->index++);
  }
  
  /**
   * Generate a key with a specific index
   * 
   * @param index the index-th key
   */
  virtual T generate(const size_t index) = 0;
  
  /**
   * This operator gives back a key with the index embraced by the
   * square brackets.
   * @see generate
   */
  const T operator[](size_t index) {
    return this->generate(index);
  }
};
//-----------------------------------------------------------------------------

#endif // KEYGEN_TEMPLATE
