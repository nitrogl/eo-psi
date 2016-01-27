/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_PARTY_TEMPLATE
#define EOPSI_PARTY_TEMPLATE
//-----------------------------------------------------------------------------

#include "eopsimsg.h"
#include "protocolex.h"
//-----------------------------------------------------------------------------

class EOPSIParty {
protected:
  std::string id;
  
public:
  EOPSIParty(const std::string& id = "") {
    this->setId(id);
  }
  
  virtual ~EOPSIParty() {}
  
  virtual void setId(const std::string& id) {
    this->id = id;
  }
  
  virtual std::string getId() const {
    return this->id;
  }
  
  virtual void send(EOPSIParty& party, const EOPSIMessage& msg) const throw (ProtocolException) {
    try {
      party.receive(msg);
    } catch (ProtocolException& e) {
      throw e;
    }
  }
  
  virtual void receive(const EOPSIMessage& msg) throw (ProtocolException) = 0;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_PARTY_TEMPLATE
