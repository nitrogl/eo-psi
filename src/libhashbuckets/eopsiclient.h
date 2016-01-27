/*
 * The secure computation client of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_CLIENT_H
#define EOPSI_CLIENT_H
//-----------------------------------------------------------------------------

#include "eopsiparty.hpp"
#include "eopsiserver.h"
//-----------------------------------------------------------------------------

class EOPSIClient : public EOPSIParty {
public:
  EOPSIClient(const std::string& id = "");
  virtual ~EOPSIClient();
  
  virtual void receive(const EOPSIMessage& msg) throw (ProtocolException);
};
//-----------------------------------------------------------------------------

#endif // EOPSI_CLIENT_H
