/*
 * String union with integers
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <sstream>
#include "strint.h"
//-----------------------------------------------------------------------------

StrInt::StrInt(const size_t bytes, const unsigned char *n, const size_t len) {
  size_t i;
  
  // Auto-adjust/interpret bytes
  if (bytes == 0) {
    this->length = 1;
  } else {
    this->length = bytes;
  }
  
  try {
    b = new mpz_t[this->length];
    pow2 = new mpz_t[this->length];
  } catch (std::bad_alloc&) {
    std::cerr << "StrInt(). Error allocating space." << std::endl;
    exit(2);
  }
  
  // Initialise conversion numbers
  for (i = 0; i < this->length; i++) {
    mpz_init(b[i]);
  }
  mpz_init(z);
  
  // Initialise powers of two
  for (i = 0; i < this->length; i++) {
    mpz_init(pow2[i]);
    mpz_ui_pow_ui(pow2[i], 2, 8*i);
  }
  
  // Set number
  if (n != nullptr && len*bytes > 0) {
    this->set(n, len);
  }
}
//-----------------------------------------------------------------------------

StrInt::~StrInt() {
  size_t i;
  
  for (i = 0; i < this->length; i++) {
    mpz_clear(b[i]);
    mpz_clear(pow2[i]);
  }
  mpz_clear(z);
}
//-----------------------------------------------------------------------------

void StrInt::set(const unsigned char *n, const size_t len) {
  size_t i, j;
  size_t dim = (len > this->length) ? this->length : len;
  
  // Initialise conversion numbers
  for (i = 0; i < dim; i++) {
    mpz_set_ui(b[i], n[i]);
    mpz_mul(b[i], b[i], pow2[i]);
//     std::cout << "\nn" << i << " = " << b[i];
  }
  
  /*
   * Perform a bottom-up parenthesised copy.
   * j stands for the progressively decreasing length.
   */
  for (j = dim; j > 1; j = (j + 1)/2) {
    for (i = 0; i < j/2; i++) {
      mpz_add(b[i], b[i], b[j/2 + i]);
    }
    if (j % 2 == 1) {
      mpz_set(b[i], b[j - 1]);
    }
  }
  
  // Save to z
  mpz_set(z, b[0]);
}
//-----------------------------------------------------------------------------

unsigned long StrInt::rem(const unsigned long divisor) const {
  return mpz_fdiv_ui(z, divisor);
}
//-----------------------------------------------------------------------------

std::string StrInt::toString() const {
  std::stringstream sstr;
  sstr << z;
  return sstr.str();
}
//-----------------------------------------------------------------------------

std::ostream& operator << (std::ostream& s, StrInt& z)
{
    return s << z.toString();
}
//-----------------------------------------------------------------------------
