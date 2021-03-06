/*
 * SHA flavours
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HASH_FLAVOUR_H
#define HASH_FLAVOUR_H
//-----------------------------------------------------------------------------

#define DEFAULT_HASH_FLAVOUR SHA1_FLAVOUR
//-----------------------------------------------------------------------------

/**
 * Flavour of hash code.
 */
typedef enum HashFlavour {
  SHA1_FLAVOUR = 0,              ///< SHA1
  SHA256_FLAVOUR = 256,          ///< SHA-256
  MURMURHASH3_128_FLAVOUR = 128, ///< Murmur hash v3 (128 bits)
} HashFlavour;
//-----------------------------------------------------------------------------

#endif // HASH_FLAVOUR_H
