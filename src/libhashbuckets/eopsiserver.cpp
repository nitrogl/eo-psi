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
  std::string msgClaimedId;
  std::string partnerId;
  std::string tmpKey;
  byte *data = nullptr;
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
      t = delegationOutput(sender->getId(), partnerId, tmpKey);
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

NTL::ZZ_p ** EOPSIServer::delegationOutput(const std::string id, const std::string idOther, const std::string tmpKey) {
  NTL::ZZ_p **t, tmp;
  NTL::ZZ_pX omega, omegaOther;
  size_t aIdx, omegaIdx, omegaOtherIdx;
  NTL::vec_ZZ_p unknowns;
  EOPSIMessage msg, msgOther;
  HashBuckets<NTL::ZZ_p>* hbParty = nullptr;
  HashBuckets<NTL::ZZ_p>* hbOtherParty = nullptr;
  
  msg = this->storedData[id];
  msgOther = this->storedData[idOther];
  hbParty = (HashBuckets<NTL::ZZ_p> *) msg.getData();
  hbOtherParty = (HashBuckets<NTL::ZZ_p> *) msgOther.getData();
  
  try {
    t = new NTL::ZZ_p *[hbParty->getLength()];
    for (size_t i = 0; i < hbParty->getLength(); i++) {
      t[i] = new NTL::ZZ_p[2*hbParty->getMaxLoad() + 1];
    }
  } catch (std::bad_alloc &) {
    std::cerr << "delegationOutput(). Error allocating memory." << std::endl;
    exit(2);
  }
    
  // Not secret unknowns
  unknowns = generateUnknowns(2*hbParty->getMaxLoad() + 1);
  
  // Initialise key generators
  keygen.setSecretKey(tmpKey);
  
  // Compute t
  aIdx = 0;
  omegaIdx = hbParty->getLength() * (2*hbParty->getMaxLoad() + 1);
  omegaOtherIdx = omegaIdx + hbParty->getLength() * (2*hbParty->getMaxLoad() + 1);
  for (size_t j = 0; j < hbParty->getLength(); j++) {
    for (size_t i = 0; i < 2*hbParty->getMaxLoad() + 1; i++) {
      conv(t[j][i], NTL::ZZFromBytes(keygen[aIdx++], keygen.getLength()));
      if (i == 2*hbParty->getMaxLoad()) {
        // Coefficient for highest degree is set to 1
        conv(tmp, 1);
        NTL::SetCoeff(omega, i, tmp);
        NTL::SetCoeff(omegaOther, i, tmp);
        omegaIdx++;
        omegaOtherIdx++;
      }
      conv(tmp, NTL::ZZFromBytes(keygen[omegaIdx++], keygen.getLength()));
      NTL::SetCoeff(omega, i, tmp);
      conv(tmp, NTL::ZZFromBytes(keygen[omegaOtherIdx++], keygen.getLength()));
      NTL::SetCoeff(omegaOther, i, tmp);
      
      // Reuse of variable "t" to store t data
      t[j][i] = t[j][i] + (*hbParty)[j][i]*eval(omega, unknowns[i]) + (*hbOtherParty)[j][i]*eval(omegaOther, unknowns[i]);
    }
  }
  
  return t;
}
//-----------------------------------------------------------------------------
