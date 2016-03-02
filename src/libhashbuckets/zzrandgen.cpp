/*
 * Big integers random generator.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <sstream>
#include "ntlmiss.h"
#include "zzrandgen.h"
//-----------------------------------------------------------------------------

RandomZZGenerator::RandomZZGenerator() {
  this->byteKeyGenerator.setHashAlgorithm(&this->shaString);
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
  this->byteKeyGenerator.setLength(NTL::bytes(this->sup));
}
//-----------------------------------------------------------------------------

void RandomZZGenerator::setSeed(const NTL::ZZ &seed) {
  std::stringstream ss;
  ss << seed;
  this->byteKeyGenerator.setSecretKey(ss.str());
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZGenerator::next() {
  NTL::ZZ gen;
  
  do {
    NTL::ZZFromBytes(gen, this->byteKeyGenerator.next(), this->byteKeyGenerator.getLength());
  } while (gen > this->sup);
  
  return gen;
}
//-----------------------------------------------------------------------------
