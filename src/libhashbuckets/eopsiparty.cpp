/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "eopsiparty.h"
#include "shastr.h"
#include "bytekeygen.h"
#include "strzzpkeygen.h"
//-----------------------------------------------------------------------------
  
EOPSIParty::EOPSIParty(const EOPSIPartyType type, const NTL::ZZ& fieldsize, const std::string& id) {
  this->setId(id);
  this->setType(type);
  
  try {
    this->rndZZpgen = new RandomZZpGenerator();
    this->strHashAlgorithm = new SHAString(SHA1_FLAVOUR);
  } catch (std::bad_alloc &) {
    std::cerr << "EOPSIParty(). Error allocating memory." << std::endl;
    exit(2);
  }
  
  // Set random generator modulo
  this->rndZZpgen->setModulo(fieldsize);
  
  // Initialise generators
  this->keygen.setHashAlgorithm(this->strHashAlgorithm);
  this->prf.setHashAlgorithm(this->strHashAlgorithm);
}
//-----------------------------------------------------------------------------

EOPSIParty::~EOPSIParty() {
  delete this->rndZZpgen;
}
//-----------------------------------------------------------------------------
  
EOPSIParty* EOPSIParty::getPartyById(const std::string& id) const {
  std::map<std::string, EOPSIParty*>::const_iterator i = parties.find(id);
  if (i != parties.end()) {
    return i->second;
  } else {
    return nullptr;
  }
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p EOPSIParty::generateUnknowns(const size_t n) {
  NTL::ZZ_p zero;
  
  conv(zero, 0);
  unknowns.SetLength(n);
  this->rndZZpgen->setSeed(zero);
  for (size_t j = 0; j < n; j++) {
    unknowns[j] = this->rndZZpgen->next();
  }
  
  return unknowns;
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p EOPSIParty::getUnknowns(const size_t n) {
  if (unknowns.length() == 0 && n > 0) {
    unknowns = generateUnknowns(n);
  }
  
  if (unknowns.length() == 0 && n == 0) {
    std::cerr << "getUnknowns(). Warning: unknowns not generated." << std::endl;
  }
  
  return unknowns;
}
//-----------------------------------------------------------------------------

void EOPSIParty::setId(const std::string& id) {
  this->id = id;
}
//-----------------------------------------------------------------------------

std::string EOPSIParty::getId() const {
  return this->id;
}
//-----------------------------------------------------------------------------

void EOPSIParty::setFieldsize(const NTL::ZZ& fieldsize) {
  this->fieldsize = fieldsize;
  NTL::ZZ_p::init(fieldsize);
  this->prf.setModulo(this->fieldsize);
}
//-----------------------------------------------------------------------------

NTL::ZZ EOPSIParty::getFieldsize() const {
  return this->fieldsize;
}
//-----------------------------------------------------------------------------

void EOPSIParty::setType(const EOPSIPartyType type) {
  this->type = type;
}
//-----------------------------------------------------------------------------

EOPSIPartyType EOPSIParty::getType() const {
  return this->type;
}
//-----------------------------------------------------------------------------

void EOPSIParty::send(EOPSIParty& party, EOPSIMessage& msg) throw (ProtocolException) {
  try {
    party.receive(msg);
  } catch (ProtocolException& e) {
    throw e;
  }
}
//-----------------------------------------------------------------------------

void EOPSIParty::authenticate(EOPSIParty& party) {
  if (party.id != this->id) {
    parties[party.id] = &party;
  }
}
//-----------------------------------------------------------------------------

bool EOPSIParty::hasAuthenticated(const std::string& id) const {
  return (getPartyById(id) != nullptr);
}
//-----------------------------------------------------------------------------

bool EOPSIParty::hasAuthenticated(const EOPSIParty& party) const {
  return hasAuthenticated(party.getId());
}
//-----------------------------------------------------------------------------
