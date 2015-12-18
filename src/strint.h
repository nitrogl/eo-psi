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

#endif // STRINT_H
