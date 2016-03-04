/*
 * A key generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

// #ifndef KEYGEN_TEMPLATE
// #define KEYGEN_TEMPLATE
//-----------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include "zzpprf.h"
//-----------------------------------------------------------------------------

ZZpPRF::ZZpPRF(NTL::ZZ& modulo, ZZPRF* prf) : PseudoRandom() {
  this->setModulo(modulo);
  if (prf != nullptr) {
    this->prf = *prf;
  }
}
//-----------------------------------------------------------------------------

ZZpPRF::~ZZpPRF() {
}
//-----------------------------------------------------------------------------

void ZZpPRF::setModulo(NTL::ZZ& modulo) {
  this->modulo = modulo;
}
//-----------------------------------------------------------------------------

NTL::ZZ ZZpPRF::getModulo() const {
  return this->modulo;
}
//-----------------------------------------------------------------------------
  
/**
 * Generate a key with a specific index
 * 
 * @param index the index-th key
 */
NTL::ZZ_p ZZpPRF::randomSeed() {
  NTL::ZZ_p p;
  conv(p, prf.randomSeed());
  return p;
}
//-----------------------------------------------------------------------------
  
/**
 * Generate a key with a specific index
 * 
 * @param index the index-th key
 */
NTL::ZZ_p ZZpPRF::generate(const NTL::ZZ_p seed, const size_t index, const size_t bits) {
  NTL::ZZ_p p;
  NTL::ZZ g;
  size_t genBits;
  
  genBits = ((size_t) NTL::NumBits(this->modulo) < bits) ? NTL::NumBits(this->modulo) : bits;
  if (genBits < bits) {
    std::cout << "generate(). Warning: the number of bits you need is beyond the field size capabilities." << std::endl;
  }
  
  while ((g = prf.generate(rep(seed), index, genBits)) >= this->modulo);
  conv(p, g);
  return p;
}
//-----------------------------------------------------------------------------
