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
 * This class acts as a key generator/pseudo-random generator.
 * The secret key of type S of this generator changes the seed of the
 * pseudo-random generator, which generates random items of type T.
 */
template <class S, class T> class KeyGenerator : public RandomGenerator<T>
{
protected:
  HashAlgorithm<S>* hashAlgorithm; ///< The hash algorithm to use for the generated keys.
  S secret;                        ///< The secret key
//   bool memoriseKeys;
//   std::vector<T> memory;
  
public:
  KeyGenerator() {
//   KeyGenerator(const bool memoriseKeys = false) {
//     this->memoriseKeys = memoriseKeys;
//     this->memory.reserve(10);
    this->hashAlgorithm = nullptr;
  }
  
  KeyGenerator(const S& secret) : KeyGenerator() {
//   KeyGenerator(const S& secret, const bool memoriseKeys = false) : KeyGenerator(memoriseKeys) {
//     this->memoriseKeys = memoriseKeys;
//     this->memory.reserve(10);
  }
  KeyGenerator(const S& secret, const HashAlgorithm<S>* hashAlgorithm) : KeyGenerator(secret) {
//   KeyGenerator(const S& secret, const HashAlgorithm<S>* hashAlgorithm, const bool memoriseKeys = false) : KeyGenerator(secret, memoriseKeys) {
    this->setHashAlgorithm(hashAlgorithm);
  }
  virtual ~KeyGenerator() {}
  
  /**
   * Set the hash algorithm to use to generate new keys.
   * 
   * @param hashAlgorithm the hash algorithm to use
   */
  virtual void setHashAlgorithm(const HashAlgorithm<S>* hashAlgorithm) {
    this->hashAlgorithm = (HashAlgorithm<S>*) hashAlgorithm;
  }
  
  /**
   * The secret should be considered as a seed for the generation.
   * 
   * @param secret the secret key of the generator
   */
  virtual void setSecretKey(const S& secret) = 0;
};
//-----------------------------------------------------------------------------

#endif // KEYGEN_TEMPLATE
