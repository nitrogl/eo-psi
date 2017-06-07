/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "eopsiparty.h"
#include "shastr.h"
//-----------------------------------------------------------------------------
  
EOPSIParty::EOPSIParty(const EOPSIPartyType type, const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id) {
  this->setId(id);
  this->setType(type);
  this->setFieldsize(fieldsize);
  this->setDegree(degree);
  this->length = length;
  this->height = height;
  this->degree = degree;
}
//-----------------------------------------------------------------------------

EOPSIParty::~EOPSIParty() {
}
//-----------------------------------------------------------------------------
  
EOPSIParty* EOPSIParty::getPartyById(const std::string id) const {
  std::map<std::string, EOPSIParty*>::const_iterator i = parties.find(id);
  if (i != parties.end()) {
    return i->second;
  } else {
    return nullptr;
  }
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p EOPSIParty::generateUnknowns(const size_t n) {
  NTL::ZZ zero;
  NTL::ZZ_p p;
  NTL::ZZ t;
  size_t len;
  
  len = NTL::NumBits(this->fieldsize);
  zero = 4;
  unknowns.SetLength(n);
  for (size_t j = 0; j < n; j++) {
    conv(unknowns[j], this->zzprf.generate(zero, j, len));
//     std::cerr << "unk " << j << ": " << unknowns[j] << std::endl;
  }

  // [[TODO: remove me]] Runtime check for deterministic randomness
//   conv(t, "7817413366800309065086246963308251115099918315902103984340276");
//   conv(p, t);
//   if (unknowns[0] != p) {
//     std::cout << "generateUnknowns(). Wrong randomness: Unexpected " << unknowns[0] << std::endl;
//     exit(3);
//   }
  
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

void EOPSIParty::genOmega(NTL::ZZ_pX& omega, const size_t degree, const NTL::ZZ seed, const size_t index) {
  NTL::ZZ_p tmp;
  
  // [[TODO: REM debug]] 1 as constant, all other coefficients as 0
//   conv(tmp, 1);
//   NTL::SetCoeff(omega, 0, tmp);
//   conv(tmp, 0);
//   for (size_t i = 1; i < degree; i++) {
//     NTL::SetCoeff(omega, i, tmp);
//   }
//   return;
  
  // Coefficient for highest degree is set to 1
  conv(tmp, 1);
  NTL::SetCoeff(omega, degree, tmp);
  for (size_t i = 0; i < degree; i++) {
    conv(tmp, this->zzprf.generate(seed, index + i, this->fieldbitsize));
    NTL::SetCoeff(omega, i, tmp);
  }
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p * EOPSIParty::computeTOrQ(const NTL::ZZ tmpKey, NTL::vec_ZZ_p *matrixA, NTL::vec_ZZ_p *matrixB) {
  size_t aIdx, omegaAIdx, omegaBIdx;
  NTL::ZZ_pX omegaA, omegaB;
  NTL::vec_ZZ_p * tq;
  NTL::ZZ a;
  
  try {
    tq = new NTL::vec_ZZ_p[this->length];
    for (size_t i = 0; i < this->length; i++) {
      tq[i].SetLength(this->degree);
    }
  } catch (std::bad_alloc &) {
    std::cerr << "computeTOrQ(). Error allocating memory." << std::endl;
    exit(2);
  }
  
  // Not secret unknowns
  this->getUnknowns(this->degree);
  
  aIdx = 0;
  omegaAIdx = this->length * this->degree;
  omegaBIdx = omegaAIdx + this->length * this->degree;
//   std::cout << "tmpKey: \"" << tmpKey << "\"" << std::endl;
  for (size_t j = 0; j < this->length; j++) {
    //gen omegas with degree
    genOmega(omegaA, this->height, tmpKey, omegaAIdx + this->height*j);
    genOmega(omegaB, this->height, tmpKey, omegaBIdx + this->height*j);
    
    for (size_t i = 0; i < this->degree; i++) {
      conv(tq[j][i], this->zzprf.generate(tmpKey, aIdx++, this->fieldbitsize));
      
      tq[j][i] = tq[j][i] + matrixA[j][i]*eval(omegaA, this->unknowns[i]) + matrixB[j][i]*eval(omegaB, this->unknowns[i]);
    }
  }
  
  return tq;
}
//-----------------------------------------------------------------------------

void EOPSIParty::setId(const std::string id) {
  this->id = id;
}
//-----------------------------------------------------------------------------

std::string EOPSIParty::getId() const {
  return this->id;
}
//-----------------------------------------------------------------------------

void EOPSIParty::setFieldsize(const NTL::ZZ fieldsize) {
  this->fieldsize = fieldsize;
  this->fieldbitsize = NTL::NumBits(this->fieldsize);
}
//-----------------------------------------------------------------------------

NTL::ZZ EOPSIParty::getFieldsize() const {
  return this->fieldsize;
}
//-----------------------------------------------------------------------------

void EOPSIParty::setDegree(const size_t degree) {
  this->degree = degree;
}
//-----------------------------------------------------------------------------

size_t EOPSIParty::getDegree() const {
  return this->degree;
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

void EOPSIParty::send(EOPSIParty& party, EOPSIMessage* msg) {
  if (msg == nullptr) {
    throw ProtocolException("sending null messages is not allowed.");
  }
  
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

bool EOPSIParty::hasAuthenticated(const std::string id) const {
  return (getPartyById(id) != nullptr);
}
//-----------------------------------------------------------------------------

bool EOPSIParty::hasAuthenticated(const EOPSIParty& party) const {
  return hasAuthenticated(party.getId());
}
//-----------------------------------------------------------------------------
