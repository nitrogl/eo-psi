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
    throw ProtocolException("Self-messaging is not allowed in EOPSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw ProtocolException("Party \"" + msg.getPartyId() + "\" is not authorised");
  }
  
  // Get the sender
  sender = getPartyById(msg.getPartyId());
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw ProtocolException("Outsourcing computation requests between servers is not (yet) supported");
      }
      break;
      
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw ProtocolException("Outsourcing data between servers is not (yet) supported");
      }
      storedData[sender->getId()] = &msg;
      std::cout << id << ". Data from " << sender->getId() << " stored (size " << msg.length() << ")." << std::endl;
      break;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      throw ProtocolException("Passing computation outputs between servers is not (yet) supported");
      
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_POLYNOMIAL:
      throw ProtocolException("The protocol forbids passing this type of messages to servers.");
      
    default:
      throw ProtocolException("Unknown message type detected.");
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
