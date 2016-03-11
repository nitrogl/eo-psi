/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <NTL/ZZ_pX.h>
#include "eopsiserver.h"
//-----------------------------------------------------------------------------

EOPSIServer::EOPSIServer(const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id) : EOPSIParty(EOPSI_PARTY_SERVER, fieldsize, length, height, degree, id) {
  
}
//-----------------------------------------------------------------------------

EOPSIServer::~EOPSIServer() {
  
}
//-----------------------------------------------------------------------------

void EOPSIServer::receive(EOPSIMessage* msg) throw (ProtocolException) {
  EOPSIParty *sender;
  std::string msgClaimedId;
  std::string partnerId;
  NTL::ZZ tmpKey;
  byte *tmpKeyBytes;
  NTL::vec_ZZ_p *t;
  
  if (msg->getPartyId() == this->id) {
    throw ProtocolException("Self-messaging is not allowed in EOPSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw ProtocolException("Party \"" + msg->getPartyId() + "\" is not authorised by " + this->getId() + ".");
  }
  
  // Get the sender
  sender = getPartyById(msg->getPartyId());
  
  switch(msg->getType()) {
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw ProtocolException("Outsourcing computation requests between servers is not (yet) supported");
      }
      
      // Check sender and message content
      msgClaimedId = (char *) msg->getData();
      if (msgClaimedId != sender->getId()) {
        throw ProtocolException("Id mismatch between sender and message data");
      }
      partnerId = &((char *) msg->getData())[msgClaimedId.length() + 1];
      
      // Prepare message for the partner client
      tmpKeyBytes = (&((byte *) msg->getData())[msgClaimedId.length() + 1 + partnerId.length() + 1]);
      tmpKey = NTL::ZZFromBytes(tmpKeyBytes, NTL::bytes(DEFAULT_KEY_BITSIZE));
      std::cout << "not fully implemented. I, " << id << ", received key \"" << tmpKey << "\" from " << sender->getId() << std::endl;
      
      t = intersectionOutput(sender->getId(), partnerId, tmpKey);
      msgToClient.setData((void *) t, 1);
      msgToClient.setType(EOPSI_MESSAGE_OUTPUT_COMPUTATION);
      msgToClient.setPartyId(this->getId());
      
      this->send(*getPartyById(partnerId), &msgToClient);
      break;
      
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw ProtocolException("Outsourcing data between servers is not (yet) supported");
      }
      this->storedData[sender->getId()] = msg;
      std::cout << id << ". Data from " << sender->getId() << " stored (size " << msg->length() << ")." << std::endl;
      
      // [[TODO: REM debug]]
//       std::cout << id << ". Data from " << sender->getId() << " stored [";
//       for (size_t i = 0; i < this->degree; i++) {
//         std::cout << " " << ((NTL::vec_ZZ_p *) msg->getData())[0][i];
//       }
//       std::cout << " ]." << std::endl;
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

bool EOPSIServer::isAuthorised(const EOPSIMessage* msg) const {
  EOPSIParty *sender;
  
  if (msg == nullptr) {
    std::cerr << "isAuthorised(). WARNING: Null pointer as message has passed." << std::endl;
    return false;
  }
  
  sender = getPartyById(msg->getPartyId());
  if (sender == nullptr || !hasAuthenticated(*sender)) {
    return false;
  }
  
  switch(msg->getType()) {
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      return (sender->getType() == EOPSI_PARTY_SERVER) || (sender->getType() == EOPSI_PARTY_CLIENT);
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      return sender->getType() == EOPSI_PARTY_SERVER;
      
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_POLYNOMIAL:
    default:
      return false;
  }
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p * EOPSIServer::intersectionOutput(const std::string idA, const std::string idB, const NTL::ZZ tmpKey) {
  NTL::ZZ_p tmp;
  NTL::ZZ_pX omegaA, omegaB;
  EOPSIMessage *msgA, *msgB;
  NTL::vec_ZZ_p* dataA = nullptr;
  NTL::vec_ZZ_p* dataB = nullptr;
  NTL::ZZ zzTmpKey;
  
  msgA = this->storedData[idA];
  msgB = this->storedData[idB];
  dataA = (NTL::vec_ZZ_p *) msgA->getData();
  dataB = (NTL::vec_ZZ_p *) msgB->getData();
  
  return computeTOrQ(tmpKey, dataA, dataB);
}
//-----------------------------------------------------------------------------
