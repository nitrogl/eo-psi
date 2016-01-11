/*
 * Big integers random generator.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "zzrandgen.h"
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator() {
  this->rndstream = nullptr;
}
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator(const NTL::ZZ &sup) : RandomZZGenerator() {
  this->setSupremum(sup);
}
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator(const NTL::ZZ &sup, const NTL::ZZ &seed) : RandomZZGenerator(sup) {
  this->setSeed(seed);
}
//-----------------------------------------------------------------------------

RandomZZGenerator::~RandomZZGenerator() {
  if (this->rndstream != nullptr) {
    delete(this->rndstream);
  }
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZGenerator::getSupremum() const {
  return this->sup;
}
//-----------------------------------------------------------------------------

void RandomZZGenerator::setSupremum(const NTL::ZZ &sup) {
  this->sup = sup;
}
//-----------------------------------------------------------------------------

void RandomZZGenerator::setSeed(const NTL::ZZ &seed) {
  if (this->rndstream != nullptr) {
    delete(this->rndstream);
  }
  
  try {
    this->rndstream = new IndependentZZRandomStream(seed);
  } catch (std::bad_alloc&) {
    std::cerr << "RandomZZGenerator(). Unable to allocate memory." << std::endl;
    exit(2);
  }
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZGenerator::next() {
  if (this->rndstream == nullptr) {
    NTL::ZZ zero;
    zero = 0;
    this->setSeed(zero);
  }
  return this->rndstream->randomBnd(sup);
}
//-----------------------------------------------------------------------------
