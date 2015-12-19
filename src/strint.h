/*
 * String union with integers
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef STRINT_H
#define STRINT_H
//-----------------------------------------------------------------------------

#include <cstdint>
#include <gmpxx.h>
//-----------------------------------------------------------------------------

class StrInt
{
private:
  mpz_t *b;
  mpz_t z;
  
  mpz_t *aux;
  mpz_t *pow2;
  
  size_t length;

public:
  StrInt(const size_t dim, const char *n = nullptr, const size_t len = 0);
  virtual ~StrInt();
  
  void set(const char *n, const size_t len);
  unsigned long rem(const unsigned long divisor) const;
  
  std::string toString() const;
};
//-----------------------------------------------------------------------------

class StrInt
{
private:
  mpz_t b[16];
  mpz_t z;
  
  mpz_t aux[AUX_MPZ];
  mpz_t pow2[16];
  
  strint128 n;

public:
  const size_t STRINT_64_BIT = 64;
  const size_t STRINT_128_BIT = 128;
  const size_t STRINT_256_BIT = 256;
  
  StrInt();
  StrInt(const strint128 n);
  StrInt(const char *n);
  StrInt(const uint64_t n);
  virtual ~StrInt();
  
  void set(const strint128 n);
  void set(const char *n);
  void set(const uint64_t n);
  unsigned long rem(const unsigned long divisor) const;
  
  std::string toString() const;
};
//-----------------------------------------------------------------------------

#endif // STRINT_H
