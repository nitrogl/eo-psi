/*
 * Implementation of a hash table with buckets as element containers.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZRNDGEN_H
#define ZZRNDGEN_H
//-----------------------------------------------------------------------------

#include <sstream>

#define SEED_MAX_LENGTH 100
#define DEFAULT_P "924319124044731127256386643194362517987965988436268856863172801"
#define DEFAULT_FILENAME "zz.dat"
#define DEFAULT_N 1000000L
//-----------------------------------------------------------------------------

static NTL::ZZ str2zz(const std::string s) {
  NTL::ZZ z;
  std::stringstream sstr;
  sstr << s;
  sstr >> z;
  return z;
}
//-----------------------------------------------------------------------------

#endif // ZZRNDGEN_H
