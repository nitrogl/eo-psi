/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_PARTY_TEMPLATE
#define EOPSI_PARTY_TEMPLATE
//-----------------------------------------------------------------------------

#include <string>
#include <map>
#include "eopsimsg.h"
#include "protocolex.h"
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
protected:
  std::string id;
  EOPSIPartyType type;
  std::map<std::string, EOPSIParty*> parties;
  
  virtual EOPSIParty* getPartyById(const std::string& id) const {
    std::map<std::string, EOPSIParty*>::const_iterator i = parties.find(id);
    if (i != parties.end()) {
      return i->second;
    } else {
      return nullptr;
    }
  }
  
public:
  EOPSIParty(const EOPSIPartyType type, const std::string& id = "") {
    this->setId(id);
    this->setType(type);
  }
  
  virtual ~EOPSIParty() {}
  
  virtual void setId(const std::string& id) {
    this->id = id;
  }
  
  virtual std::string getId() const {
    return this->id;
  }
  
  virtual void setType(const EOPSIPartyType type) {
    this->type = type;
  }
  
  virtual EOPSIPartyType getType() const {
    return this->type;
  }
  
  virtual void send(EOPSIParty& party, EOPSIMessage& msg) throw (ProtocolException) {
    try {
      party.receive(msg);
    } catch (ProtocolException& e) {
      throw e;
    }
  }
  
  virtual void receive(EOPSIMessage& msg) throw (ProtocolException) = 0;
  
  virtual void authenticate(EOPSIParty& party) {
    if (party.id != this->id) {
      parties[party.id] = &party;
    }
  }
  
  virtual bool hasAuthenticated(const std::string& id) const {
    return (getPartyById(id) != nullptr);
  }
  
  virtual bool hasAuthenticated(const EOPSIParty& party) const {
    return hasAuthenticated(party.getId());
  }
  
  virtual bool isAuthorised(const EOPSIMessage& msg) const = 0;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_PARTY_TEMPLATE
