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

// AUX_MPZ should be at least 8
#define AUX_MPZ 8
//-----------------------------------------------------------------------------

typedef union strint128 {
  char     str[16];
  int8_t   i8[16]; // char...
  int16_t  i16[8];
  int32_t  i32[4];
  int64_t  i64[2];
  uint8_t  u8[16]; // uchar...
  uint16_t u16[8];
  uint32_t u32[4];
  uint64_t u64[2];
} strint128;
//-----------------------------------------------------------------------------

typedef union strint64 {
  char     str[8];
  int8_t   i8[8]; // char...
  int16_t  i16[4];
  int32_t  i32[2];
  int64_t  i64[1];
  uint8_t  u8[8]; // uchar...
  uint16_t u16[4];
  uint32_t u32[2];
  uint64_t u64[1];
} strint64;
//-----------------------------------------------------------------------------

class StrInt128
{
private:
  mpz_t b[16];
  mpz_t z;
  
  mpz_t aux[AUX_MPZ];
  mpz_t pow2[16];
  
  strint128 n;

public:
  StrInt128();
  StrInt128(const strint128 n);
  StrInt128(const char *n);
  StrInt128(const uint64_t n);
  virtual ~StrInt128();
  
  void set(const strint128 n);
  void set(const char *n);
  void set(const uint64_t n);
  unsigned long rem(const unsigned long divisor) const;
  
  std::string toString() const;
};
//-----------------------------------------------------------------------------

#endif // STRINT_H
