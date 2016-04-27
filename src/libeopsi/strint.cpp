/*
 * String union with integers
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <sstream>
#include "strint.h"
//-----------------------------------------------------------------------------

StrInt::StrInt(const size_t bytes, const byte *n, const size_t len) {
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
  } else {
    byte *zero;
    try {
      zero = new byte[len];
    } catch (std::bad_alloc&) {
      std::cerr << "StrInt(). Error allocating space." << std::endl;
      exit(2);
    }
    for (i = 0; i < len; i++) {
      zero = '\0';
    }
    this->set(zero, len);
    delete [] zero;
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
  
  delete [] b;
  delete [] pow2;
}
//-----------------------------------------------------------------------------

void StrInt::set(const unsigned long len) {
  mpz_set_ui(z, len);
}
//-----------------------------------------------------------------------------

void StrInt::set(const byte *n, const size_t len) {
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

unsigned long StrInt::lsb() const {
  return mpz_fdiv_ui(b[0], (unsigned long) (-1L));
}
//-----------------------------------------------------------------------------

void StrInt::inc() {
  mpz_add(z, z, pow2[0]);
}
//-----------------------------------------------------------------------------

void StrInt::add(const unsigned long n) {
  mpz_set_ui(b[0], n);
  mpz_add(z, z, b[0]);
}
//-----------------------------------------------------------------------------

std::string StrInt::toString() const {
  std::stringstream sstr;
  sstr << this->z;
  return sstr.str();
}
//-----------------------------------------------------------------------------

byte* StrInt::toBytes(byte *barr, size_t* bytelen) const {
  const int MSB_FIRST = 1;
  const int BIG_ENDIANNESS = 1;
  const size_t LEN = this->getByteSize();
  
  if ((barr != NULL) && (bytelen != NULL) && (*bytelen != LEN)) {
    try {
      barr = new byte[LEN];
    } catch (std::bad_alloc&) {
      std::cerr << "toBytes(). Error allocating space." << std::endl;
      exit(2);
    }
  }
  mpz_export((void *) barr, bytelen, MSB_FIRST, sizeof(byte), BIG_ENDIANNESS, 0, this->z);
  
  /*
   * If this->z is zero then the count returned will be zero and nothing 
   * written to barr. If barr is NULL in this case, no block is allocated, 
   * just NULL is returned. 
   */
  if (*bytelen == 0 && LEN == 1) {
    barr[0] = '\0';
    *bytelen = 1;
  }
  
  if (*bytelen != LEN) {
    std::cerr << "toBytes(). Something unexpected happened in conversion." << std::endl;
    exit(2);
  }
  
  return barr;
}
//-----------------------------------------------------------------------------

size_t StrInt::getByteSize() const {
  return (mpz_sizeinbase(this->z, 2) + sizeof(byte) - 1)/sizeof(byte);
}
//-----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const StrInt& strint) {
    return os << strint.toString();
}
//-----------------------------------------------------------------------------
