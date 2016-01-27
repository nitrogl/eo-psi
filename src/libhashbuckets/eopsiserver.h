/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_SERVER_H
#define EOPSI_SERVER_H
//-----------------------------------------------------------------------------

#include <string>
#include <list>
#include "eopsiparty.hpp"
#include "eopsiclient.h"
//-----------------------------------------------------------------------------

class EOPSIClient;

class EOPSIServer : public EOPSIParty {
protected:
  std::list<EOPSIMessage*> storedData;
  
  
public:
  EOPSIServer(const std::string& id = "");
  virtual ~EOPSIServer();
  
  virtual void receive(const EOPSIMessage& msg) throw (ProtocolException);
};
//-----------------------------------------------------------------------------

#endif // EOPSI_SERVER_H
