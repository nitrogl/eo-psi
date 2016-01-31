/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiclient.h"
//-----------------------------------------------------------------------------

EOPSIClient::EOPSIClient(const std::string& id) : EOPSIParty(EOPSI_PARTY_CLIENT, id) {
  
}
//-----------------------------------------------------------------------------

EOPSIClient::~EOPSIClient() {
  
}
//-----------------------------------------------------------------------------

void EOPSIClient::receive(EOPSIMessage& msg) throw (ProtocolException) {
  if (msg.getPartyId() == this->id) {
    throw new ProtocolException("Self-messaging is not allowed in EO-PSI protocol");
  }
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
      break;
      
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      break;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      break;
      
    case EOPSI_MESSAGE_POLYNOMIAL:
      break;
      
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      break;
      
    default:
      break;
  }
}
//-----------------------------------------------------------------------------

bool EOPSIClient::isAuthorised(const EOPSIMessage& msg) const {
  EOPSIParty *sender;
  
  sender = getPartyById(msg.getPartyId());
  if (!hasAuthenticated(*sender)) {
    return false;
  }
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_POLYNOMIAL:
      return sender->getType() == EOPSI_PARTY_CLIENT;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      return sender->getType() == EOPSI_PARTY_SERVER;
      
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
    default:
      return false;
  }
}
//-----------------------------------------------------------------------------
