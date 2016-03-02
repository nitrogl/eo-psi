/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiserver.h"
//-----------------------------------------------------------------------------

EOPSIServer::EOPSIServer(const NTL::ZZ& fieldsize, const std::string& id) : EOPSIParty(EOPSI_PARTY_SERVER, fieldsize, id) {
  
}
//-----------------------------------------------------------------------------

EOPSIServer::~EOPSIServer() {
  
}
//-----------------------------------------------------------------------------

void EOPSIServer::receive(EOPSIMessage& msg) throw (ProtocolException) {
  EOPSIParty *sender;
  std::string msgClaimedId;
  std::string partnerId;
  std::string tmpKey;
  EOPSIMessage msgToClient;
  NTL::ZZ_p **t;
  
  if (msg.getPartyId() == this->id) {
    throw ProtocolException("Self-messaging is not allowed in EOPSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw ProtocolException("Party \"" + msg.getPartyId() + "\" is not authorised by " + this->getId() + ".");
  }
  
  // Get the sender
  sender = getPartyById(msg.getPartyId());
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      if (sender->getType() == EOPSI_PARTY_SERVER) {
        throw ProtocolException("Outsourcing computation requests between servers is not (yet) supported");
      }
      
      // Check sender and message content
      msgClaimedId = (char *) msg.getData();
      if (msgClaimedId != sender->getId()) {
        throw ProtocolException("Id mismatch between sender and message data");
      }
      partnerId = &((char *) msg.getData())[msgClaimedId.length() + 1];
      
      // Prepare message for the partner client
      tmpKey = (&((char *) msg.getData())[msgClaimedId.length() + 1 + partnerId.length() + 1]);
      t = intersectionOutput(sender->getId(), partnerId, tmpKey);
      msgToClient.setData((void *) t, 1);
      msgToClient.setType(EOPSI_MESSAGE_OUTPUT_COMPUTATION);
      msgToClient.setPartyId(this->getId());
      this->send(*getPartyById(partnerId), msgToClient);
      
      std::cout << "not fully implemented. I, " << id << ", received \"" << (&((byte *) msg.getData())[msgClaimedId.length() + 1 + partnerId.length() + 1]) << "\" from " << sender->getId() << std::endl;
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

NTL::ZZ_p ** EOPSIServer::intersectionOutput(const std::string idA, const std::string idB, const std::string tmpKey) {
  NTL::ZZ_p **t, tmp;
  NTL::ZZ_pX omegaA, omegaB;
  size_t aIdx, omegaAIdx, omegaBIdx;
  EOPSIMessage *msgA, *msgB;
  NTL::vec_ZZ_p* dataA = nullptr;
  NTL::vec_ZZ_p* dataB = nullptr;
  size_t length, height;
  
  msgA = this->storedData[idA];
  msgB = this->storedData[idB];
  dataA = (NTL::vec_ZZ_p *) msgA->getData();
  dataB = (NTL::vec_ZZ_p *) msgB->getData();
  height = dataA[0].length();
  if (height == 0) {
    std::cerr << "intersectionOutput(). Fatal error: wrong data length in message from client." << std::endl;
    exit(2);
  }
  length = msgA->length() / height;
  
  try {
    t = new NTL::ZZ_p *[length];
    for (size_t i = 0; i < length; i++) {
      t[i] = new NTL::ZZ_p[height];
    }
  } catch (std::bad_alloc &) {
    std::cerr << "intersectionOutput(). Error allocating memory." << std::endl;
    exit(2);
  }
    
  // Not secret unknowns
  unknowns = this->getUnknowns(height);
  
  // Initialise key generators
  keygen.setSecretKey(tmpKey);
  
  // Compute t
  aIdx = 0;
  omegaAIdx = length * (height);
  omegaBIdx = omegaAIdx + length * (height);
  for (size_t j = 0; j < length; j++) {
    for (size_t i = 0; i < height; i++) {
      conv(t[j][i], NTL::ZZFromBytes(keygen[aIdx++], keygen.getLength()));
      if (i == height - 1) {
        // Coefficient for highest degree is set to 1
        conv(tmp, 1);
        NTL::SetCoeff(omegaA, i, tmp);
        NTL::SetCoeff(omegaB, i, tmp);
        omegaAIdx++;
        omegaBIdx++;
      }
      conv(tmp, NTL::ZZFromBytes(keygen[omegaAIdx++], keygen.getLength()));
      NTL::SetCoeff(omegaA, i, tmp);
      conv(tmp, NTL::ZZFromBytes(keygen[omegaBIdx++], keygen.getLength()));
      NTL::SetCoeff(omegaB, i, tmp);
      
      // Reuse of variable "t" to store t dataA
//       t[j][i] = t[j][i] + dataA[j][i]*eval(omegaA, unknowns[i]) + dataB[j][i]*eval(omegaB, unknowns[i]);
      t[j][i] = t[j][i] + dataA[j][i] + dataB[j][i];
    }
  }
  
  return t;
}
//-----------------------------------------------------------------------------
