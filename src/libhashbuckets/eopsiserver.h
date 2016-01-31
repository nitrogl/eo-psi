/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_SERVER_H
#define EOPSI_SERVER_H
//-----------------------------------------------------------------------------

#include <string>
#include <map>
#include "eopsiparty.hpp"
#include "eopsiclient.h"
//-----------------------------------------------------------------------------

class EOPSIClient;

class EOPSIServer : public EOPSIParty {
protected:
  std::map<std::string, EOPSIMessage*> storedData;
  
public:
  EOPSIServer(const std::string& id = "");
  virtual ~EOPSIServer();
  
  virtual void receive(EOPSIMessage& msg) throw (ProtocolException);
  virtual bool isAuthorised(const EOPSIMessage& msg) const;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_SERVER_H
