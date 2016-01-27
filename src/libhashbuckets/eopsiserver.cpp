/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiserver.h"
//-----------------------------------------------------------------------------

EOPSIServer::EOPSIServer(const std::string& id) : EOPSIParty(id) {
  
}
//-----------------------------------------------------------------------------

EOPSIServer::~EOPSIServer() {
  
}
//-----------------------------------------------------------------------------

void EOPSIServer::receive(const EOPSIMessage& msg) throw (ProtocolException) {
  if (msg.getPartyId() == this->id) {
    throw new ProtocolException("Self-messaging is not allowed in EOPSI protocol");
  }
}
//-----------------------------------------------------------------------------
