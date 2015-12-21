/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "zzrandgen.h"
//-----------------------------------------------------------------------------

ZZRandomGenerator::ZZRandomGenerator(NTL::ZZ &p) {
  this->p = p;
}
//-----------------------------------------------------------------------------

ZZRandomGenerator::ZZRandomGenerator(NTL::ZZ &p, NTL::ZZ &seed) : ZZRandomGenerator(p) {
  this->setSeed(seed);
}
//-----------------------------------------------------------------------------

ZZRandomGenerator::~ZZRandomGenerator() {
}
//-----------------------------------------------------------------------------

void ZZRandomGenerator::setSeed(NTL::ZZ &seed) {
  NTL::SetSeed(seed);
}
//-----------------------------------------------------------------------------

NTL::ZZ ZZRandomGenerator::next() const {
  return NTL::RandomBnd(p);
}
//-----------------------------------------------------------------------------
