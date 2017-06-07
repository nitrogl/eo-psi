/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_PARTY_H
#define EOPSI_PARTY_H
//-----------------------------------------------------------------------------

#include <string>
#include <map>
#include <NTL/ZZ_pX.h>
#include <NTL/vec_ZZ_p.h>
#include "ntlmiss.h"
#include "eopsimsg.h"
#include "protocolex.h"
#include "zzprf.h"
#include "simplebm.h"
//-----------------------------------------------------------------------------

#define DEFAULT_KEY_BITSIZE 128
//-----------------------------------------------------------------------------

/**
 * Type of parties in the EO-PSI protocol
 */
typedef enum EOPSIPartyType {
  EOPSI_PARTY_CLIENT, ///< Clients are those who want to outsource computations to other clients or servers and stores data into the cloud (server)
  EOPSI_PARTY_SERVER, ///< Servers are data holders in the cloud of clients data, they perform some computation for 
} EOPSIPartyType;
//-----------------------------------------------------------------------------

class EOPSIParty {
private:
  virtual NTL::vec_ZZ_p generateUnknowns(const size_t n);
  
protected:
  std::string id;
  EOPSIPartyType type;
  std::map<std::string, EOPSIParty*> parties;
  NTL::ZZ fieldsize;
  size_t fieldbitsize;
  ZZPRF zzprf;
  NTL::vec_ZZ_p unknowns;
  size_t degree;
  size_t length;
  size_t height;
  SimpleBenchmark bm;
  
  virtual EOPSIParty* getPartyById(const std::string id) const;
  virtual NTL::vec_ZZ_p getUnknowns(const size_t n = 0);
  virtual void genOmega(NTL::ZZ_pX& omega, const size_t degree, const NTL::ZZ seed, const size_t index);
  virtual NTL::vec_ZZ_p * computeTOrQ(const NTL::ZZ tmpKey, NTL::vec_ZZ_p *matrixA, NTL::vec_ZZ_p *matrixB);
  
public:
  EOPSIParty(const EOPSIPartyType type, const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id = "");
  virtual ~EOPSIParty();
  
  virtual void setId(const std::string id);
  virtual std::string getId() const;
  
  void setFieldsize(const NTL::ZZ fieldsize);
  virtual NTL::ZZ getFieldsize() const;
  
  void setDegree(const size_t degree);
  virtual size_t getDegree() const;
  
  virtual void setType(const EOPSIPartyType type);
  virtual EOPSIPartyType getType() const;
  
  virtual void send(EOPSIParty& party, EOPSIMessage* msg);
  virtual void receive(EOPSIMessage* msg) = 0;
  
  virtual void authenticate(EOPSIParty& party);
  virtual bool hasAuthenticated(const std::string id) const;
  virtual bool hasAuthenticated(const EOPSIParty& party) const;
  virtual bool isAuthorised(const EOPSIMessage* msg) const = 0;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_PARTY_H
