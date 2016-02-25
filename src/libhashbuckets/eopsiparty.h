/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_PARTY_H
#define EOPSI_PARTY_H
//-----------------------------------------------------------------------------

#include <string>
#include <map>
#include "eopsimsg.h"
#include "protocolex.h"
#include "strzzpkeygen.h"
#include "zzprandgen.h"
#include "bytekeygen.h"
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
  HashAlgorithm<std::string>* strHashAlgorithm;
  ByteKeyGenerator keygen;
  RandomZZpGenerator *rndZZpgen;
  StringZZpKeyGenerator prf;
  NTL::vec_ZZ_p unknowns;
  
  virtual EOPSIParty* getPartyById(const std::string& id) const;
  virtual NTL::vec_ZZ_p getUnknowns(const size_t n = 0);
  
public:
  EOPSIParty(const EOPSIPartyType type, const NTL::ZZ& fieldsize, const std::string& id = "");
  virtual ~EOPSIParty();
  
  virtual void setId(const std::string& id);
  virtual std::string getId() const;
  
  void setFieldsize(const NTL::ZZ& fieldsize);
  virtual NTL::ZZ getFieldsize() const;
  
  virtual void setType(const EOPSIPartyType type);
  virtual EOPSIPartyType getType() const;
  
  virtual void send(EOPSIParty& party, EOPSIMessage& msg) throw (ProtocolException);
  virtual void receive(EOPSIMessage& msg) throw (ProtocolException) = 0;
  
  virtual void authenticate(EOPSIParty& party);
  virtual bool hasAuthenticated(const std::string& id) const;
  virtual bool hasAuthenticated(const EOPSIParty& party) const;
  virtual bool isAuthorised(const EOPSIMessage& msg) const = 0;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_PARTY_H
