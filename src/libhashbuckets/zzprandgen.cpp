/*
 * Integers modulo p random generator
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "zzprandgen.h"
//-----------------------------------------------------------------------------

RandomZZpGenerator::RandomZZpGenerator() {
}
//-----------------------------------------------------------------------------

RandomZZpGenerator::RandomZZpGenerator(const NTL::ZZ &p) : RandomZZpGenerator() {
  this->setModulo(p);
}
//-----------------------------------------------------------------------------

RandomZZpGenerator::RandomZZpGenerator(const NTL::ZZ &p, const NTL::ZZ_p &seed) : RandomZZpGenerator(p) {
  this->setSeed(seed);
}
//-----------------------------------------------------------------------------

RandomZZpGenerator::~RandomZZpGenerator() {
}
//-----------------------------------------------------------------------------

NTL::ZZ RandomZZpGenerator::getModulo() const {
  return this->rndZZgen.getSupremum();
}
//-----------------------------------------------------------------------------

void RandomZZpGenerator::setModulo(const NTL::ZZ &p) {
  this->rndZZgen.setSupremum(p);
}
//-----------------------------------------------------------------------------

void RandomZZpGenerator::setSeed(const NTL::ZZ_p &seed) {
  this->rndZZgen.setSeed(rep(seed));
}
//-----------------------------------------------------------------------------

NTL::ZZ_p RandomZZpGenerator::next() {
  NTL::ZZ_p z;
  NTL::ZZ_pContext zzpContext;
  
  zzpContext.save();
  NTL::ZZ_p::init(this->rndZZgen.getSupremum());
  conv(z, this->rndZZgen.next());
  zzpContext.restore();
  return z;
}
//-----------------------------------------------------------------------------
