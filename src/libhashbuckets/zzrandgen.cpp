/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "zzrandgen.h"
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator() {
}
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator(const NTL::ZZ &p) : RandomZZGenerator() {
  this->setSupremum(p);
}
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator(const NTL::ZZ &p, const NTL::ZZ &seed) : RandomZZGenerator(p) {
  this->setSeed(seed);
}
//-----------------------------------------------------------------------------

RandomZZGenerator::~RandomZZGenerator() {
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZGenerator::getSupremum() const {
  return this->p;
}
//-----------------------------------------------------------------------------

void RandomZZGenerator::setSupremum(const NTL::ZZ &p) {
  this->p = p;
}
//-----------------------------------------------------------------------------

void RandomZZGenerator::setSeed(const NTL::ZZ &seed) {
  NTL::SetSeed(seed);
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZGenerator::next() const {
  return NTL::RandomBnd(p);
}
//-----------------------------------------------------------------------------
