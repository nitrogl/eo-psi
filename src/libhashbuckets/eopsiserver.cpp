/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiserver.h"
//-----------------------------------------------------------------------------

EOPSIServer::EOPSIServer(const std::string& id) : EOPSIParty(EOPSI_PARTY_SERVER, id) {
  
}
//-----------------------------------------------------------------------------

EOPSIServer::~EOPSIServer() {
  
}
//-----------------------------------------------------------------------------

void EOPSIServer::receive(EOPSIMessage& msg) throw (ProtocolException) {
  EOPSIParty *sender;
  
  if (msg.getPartyId() == this->id) {
    throw new ProtocolException("Self-messaging is not allowed in EOPSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw new ProtocolException("Party \"" + msg.getPartyId() + "\" is not authorised");
  }
  
  // Get the sender
  sender = getPartyById(msg.getPartyId());
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw new ProtocolException("Outsourcing computation requests between servers is not (yet) supported");
      }
      break;
      
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw new ProtocolException("Outsourcing data between servers is not (yet) supported");
      }
      storedData[sender->getId()] = &msg;
      break;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      throw new ProtocolException("Passing computation outputs between servers is not (yet) supported");
      
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_POLYNOMIAL:
    default:
      break;
  }
}
//-----------------------------------------------------------------------------

bool EOPSIServer::isAuthorised(const EOPSIMessage& msg) const {
  EOPSIParty *sender;
  
  sender = getPartyById(msg.getPartyId());
  if (sender == nullptr || !hasAuthenticated(*sender)) {
    return false;
  }
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      return true;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      return sender->getType() == EOPSI_PARTY_SERVER;
      
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_POLYNOMIAL:
    default:
      return false;
  }
}
//-----------------------------------------------------------------------------
