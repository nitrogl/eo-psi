/*
 * Big integers random generator.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "zzrandgen.h"
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator() {
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
  NTL::SetSeed(seed);
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZGenerator::next() {
  return NTL::RandomBnd(sup);
}
//-----------------------------------------------------------------------------
