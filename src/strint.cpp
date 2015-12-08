/*
 * String union with integers
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <sstream>
#include "strint.h"
//-----------------------------------------------------------------------------

StrInt128::StrInt128() {
  int i;
  
  // Initialise conversion numbers
  for (i = 0; i < 16; i++) {
    mpz_init(b[i]);
  }
  mpz_init(z);
  
  // Initialise auxiliary numbers
  for (i = 0; i < AUX_MPZ; i++) {
    mpz_init(aux[i]);
  }
  
  // Initialise powers of two
  for (i = 0; i < 16; i++) {
    mpz_init(pow2[i]);
    mpz_ui_pow_ui(pow2[i], 2, 8*i);
  }
}
//-----------------------------------------------------------------------------

StrInt128::StrInt128(const strint128 n) : StrInt128() {
  this->set(n);
}
//-----------------------------------------------------------------------------

StrInt128::StrInt128(const char *n) : StrInt128() {
  this->set(n);
}
//-----------------------------------------------------------------------------

StrInt128::StrInt128(const uint64_t n) : StrInt128() {
  this->set(n);
}
//-----------------------------------------------------------------------------

StrInt128::~StrInt128() {
  int i;
  
  for (i = 0; i < 16; i++) {
    mpz_clear(b[i]);
    mpz_clear(pow2[i]);
  }
  mpz_clear(z);
  
  for (i = 0; i < AUX_MPZ; i++) {
    mpz_clear(aux[i]);
  }
}
//-----------------------------------------------------------------------------

void StrInt128::set(const strint128 n) {
  int i;
  
  this->n = n;
  
  // Initialise conversion numbers
  for (i = 0; i < 16; i++) {
    mpz_set_ui(b[i], n.u8[i]);
    mpz_mul(b[i], b[i], pow2[i]);
//     std::cout << "\nn" << i << " = " << b[i];
  }
  
  for (i = 0; i < 8; i++) {
    mpz_add(aux[i], b[i], b[8 + i]);
  }
  for (i = 0; i < 4; i++) {
    mpz_add(aux[i], aux[i], aux[4 + i]);
  }
  for (i = 0; i < 2; i++) {
    mpz_add(aux[i], aux[i], aux[2 + i]);
  }
  mpz_add(z, aux[0], aux[1]);
}
//-----------------------------------------------------------------------------

void StrInt128::set(const uint64_t n) {
  this->n.u64[0] = n;
  this->n.u64[1] = 0L;
  this->set(this->n);
}
//-----------------------------------------------------------------------------

void StrInt128::set(const char *n) {
  size_t i, dim;
  
  if (n == nullptr || strlen(n) == 0) {
    std::cerr << "StrInt128::set()" << ". WARNING: empty string ignored." << std::endl;
    return;
  }
  
  // Set characters
  dim = strlen(n);
  dim = (dim > 16) ? 16 : dim;
  for (i = 0; i < dim; i++) {
    this->n.str[i] = n[i];
  }
  // Clear the remaining slots
  for (; i < 16; i++) {
    this->n.str[i] = '\0';
  }
  this->set(this->n);
}
//-----------------------------------------------------------------------------

unsigned long StrInt128::rem(const unsigned long divisor) const {
  return mpz_fdiv_ui(z, divisor);
}
//-----------------------------------------------------------------------------

std::string StrInt128::toString() const {
  std::stringstream sstr;
  sstr << z;
  return sstr.str();
}
//-----------------------------------------------------------------------------

std::ostream& operator << (std::ostream& s, StrInt128& z)
{
    return s << z.toString();
}
//-----------------------------------------------------------------------------
