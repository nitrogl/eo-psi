/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiclient.h"
//-----------------------------------------------------------------------------

EOPSIClient::EOPSIClient(const std::string& id) : EOPSIParty(id) {
  
}
//-----------------------------------------------------------------------------

EOPSIClient::~EOPSIClient() {
  
}
//-----------------------------------------------------------------------------

void EOPSIClient::receive(const EOPSIMessage& msg) throw (ProtocolException) {
  if (msg.getPartyId() == this->id) {
    throw new ProtocolException("Self-messaging is not allowed in EO-PSI protocol");
  }
}
//-----------------------------------------------------------------------------
