/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiclient.h"
#include "ntlmiss.h"
#include "shastr.h"
#include "bytekeygen.h"
#include "strzzpkeygen.h"
//-----------------------------------------------------------------------------

/**
 * The function to call from each thread to evaluate polynomials
 * 
 * @param evaluations the result is stored here
 * @param polynomials the polynomials to evaluate
 * @param unknowns the unknowns to use in the evaluations
 * @param from the first index for the subset of unknowns to evaluate
 * @param from the last index (excluded) for the subset of unknowns to evaluate
 */
static void polEval(NTL::vec_ZZ_p *evaluations, NTL::ZZ_pX *polynomials, NTL::vec_ZZ_p unknowns, const size_t from, const size_t to) {
  size_t j;
  for (j = from; j < to; j++) {
    eval(evaluations[j], polynomials[j], unknowns);
  }
}
//-----------------------------------------------------------------------------

EOPSIClient::EOPSIClient(HashBuckets<NTL::ZZ_p>& hashBuckets, const NTL::ZZ& fieldsize, const std::string& id, const std::string& secret) : EOPSIParty(EOPSI_PARTY_CLIENT, id) {
  this->rawData = nullptr;
  this->rawDataSize = 0;
  this->blindedData = nullptr;
  this->hashBuckets = &hashBuckets;
  this->setFieldsize(fieldsize);
  this->setSecret(secret);
  
  try {
    this->rndZZpgen = new RandomZZpGenerator();
    this->rndZZpgen->setModulo(fieldsize);
    this->strHashAlgorithm = new SHAString(SHA1_FLAVOUR);
    this->keygen.setHashAlgorithm(this->strHashAlgorithm);
    this->prf.setHashAlgorithm(this->strHashAlgorithm);
  } catch (std::bad_alloc &) {
    std::cerr << "EOPSIClient(). Error allocating memory." << std::endl;
    exit(2);
  }
}
//-----------------------------------------------------------------------------

EOPSIClient::~EOPSIClient() {
  delete this->rawData;
  delete this->blindedData;
  delete this->hashBuckets;
}
//-----------------------------------------------------------------------------

void EOPSIClient::receive(EOPSIMessage& msg) throw (ProtocolException) {
  EOPSIParty *sender, *cloud;
  std::string msgClaimedId;
  std::string otherSecret;
  std::string rndstr;
  byte *data = nullptr;
  size_t dataLen, i;
  std::map<std::string, EOPSIParty*>::const_iterator mi;
  EOPSIMessage msgToCloud, msgToClient;
  NTL::ZZ_p **q;
  
  if (msg.getPartyId() == this->id) {
    throw ProtocolException("Self-messaging is not allowed in EO-PSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw ProtocolException("Party \"" + msg.getPartyId() + "\" is not authorised by " + this->getId() + ".");
  }
  
  // Get the sender
  sender = getPartyById(msg.getPartyId());
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
      // Check sender and message content
      msgClaimedId = (char *) msg.getData();
      if (msgClaimedId != sender->getId()) {
        throw ProtocolException("Id mismatch between sender and message data");
      }
      
      /*
       * 1. Send outsourcing request to the cloud
       */
      // Prepare message for the Cloud
      rndstr = rndStrgen.next(4);
      dataLen = this->getId().length() + 1 + sender->getId().length() + 1 + rndstr.length() + 1;
      
      try {
        data = new byte[dataLen];
      } catch (std::bad_alloc &) {
        std::cerr << "receive(). Error allocating memory." << std::endl;
        exit(1);
      }
      
      strcpy((char *) data, &this->getId()[0]);
      strcpy((char *) (data + this->getId().length() + 1), &sender->getId()[0]);
      for (i = 0; i < rndstr.length(); i++) {
        data[this->getId().length() + 1 + sender->getId().length() + 1 + i] = rndstr[i];
      }
      data[dataLen - 1] = '\0'; // Correctly end data
      msgToCloud.setData(data, dataLen);
      msgToCloud.setType(EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST);
      msgToCloud.setPartyId(this->getId());
      
      // Find some cloud among authenticated parties
      cloud = nullptr;
      mi = parties.begin();
      while (mi != parties.end() && cloud == nullptr) {
        if (mi->second->getType() == EOPSI_PARTY_SERVER) {
          cloud = mi->second;
        }
        mi++;
      }
      if (cloud == nullptr) {
        std::cerr << "receive(). Could not find any cloud to outsource data to." << std::endl;
        exit(2);
      }
      
      // Reception feedback
      otherSecret = &((char *) msg.getData())[msgClaimedId.length() + 1];
      std::cout << id << ". Received \"" << otherSecret << "\" from " << sender->getId() << std::endl;
      
      // Send the message
      try {
        std::cout << this->getId() << ". Sending cloud computation request with random string \"" << (&data[this->getId().length() + 1 + sender->getId().length() + 1]) << "\" to " << cloud->getId() << "." << std::endl;
        this->send(*cloud, msgToCloud);
      } catch (ProtocolException &e) {
        std::cerr << "receive(). " << e.what() << std::endl;
        exit(2);
      }
      
      /*
       * 2. Send q to client
       */
      q = delegationOutput(otherSecret);
      msgToClient.setData((void *) q, 1);
      msgToClient.setType(EOPSI_MESSAGE_POLYNOMIAL);
      msgToClient.setPartyId(this->getId());
      this->send(*sender, msgToClient);
      break;
      
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      break;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      break;
      
    case EOPSI_MESSAGE_POLYNOMIAL:
      // Reception feedback
      q = (NTL::ZZ_p **) msg.getData();
      std::cout << "not fully implemented. I, " << id << ", received \"" << q[0][0] << "\" from " << sender->getId() << std::endl;
      break;
      
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      throw ProtocolException("The protocol forbids passing this type of message to clients.");
      
    default:
      throw ProtocolException("Unknown message type detected.");
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

void EOPSIClient::setRawData(NTL::ZZ *rawData, const size_t size, const unsigned int nThreads) {
  if (rawData == nullptr || size == 0) {
    std::cerr << "setRawData(). WARNING: Empty data to store; nothing to do" << std::endl;
  } else {
    this->rawData = rawData;
    this->rawDataSize = size;
    blind(nThreads);
  }
}
//-----------------------------------------------------------------------------

NTL::ZZ * EOPSIClient::getRawData() const {
  return this->rawData;
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p * EOPSIClient::getBlindedData() const {
  return this->blindedData;
}
//-----------------------------------------------------------------------------

size_t EOPSIClient::getRawDataSize() const {
  return this->rawDataSize;
}
//-----------------------------------------------------------------------------

size_t EOPSIClient::getBlindedDataSize() const {
  if (this->hashBuckets != nullptr) {
    return (2*this->hashBuckets->getMaxLoad() + 1)*this->hashBuckets->getLength();
  } else {
    std::cerr << "getBlindedDataSize(). WARNING: Internal data structure hash buckets is not instantiated" << std::endl;
    return 0;
  }
}
//-----------------------------------------------------------------------------

void EOPSIClient::setSecret(const std::string& secret) {
  this->secret = secret;
  this->keygen.setSecretKey(this->secret);
}
//-----------------------------------------------------------------------------

std::string EOPSIClient::getSecret() const {
  return this->secret;
}
//-----------------------------------------------------------------------------

void EOPSIClient::setFieldsize(const NTL::ZZ& fieldsize) {
  this->fieldsize = fieldsize;
  NTL::ZZ_p::init(fieldsize);
  this->prf.setModulo(this->fieldsize);
  this->keygen.setLength((NTL::bits(this->fieldsize) + sizeof(byte) - 1)/sizeof(byte));
}
//-----------------------------------------------------------------------------

NTL::ZZ EOPSIClient::getFieldsize() const {
  return this->fieldsize;
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p EOPSIClient::generateUnknowns() {
  NTL::vec_ZZ_p unknowns;
  NTL::ZZ_p zero;
  
  conv(zero, 0);
  unknowns.SetLength(2*this->hashBuckets->getMaxLoad() + 1);
  this->rndZZpgen->setSeed(zero);
  for (size_t j = 0; j < 2*this->hashBuckets->getMaxLoad() + 1; j++) {
    append(unknowns, this->rndZZpgen->next());
  }
  
  return unknowns;
}
//-----------------------------------------------------------------------------

void EOPSIClient::blind(unsigned int nThreads) {
  size_t padsize;
  NTL::ZZ_p z;
  NTL::ZZ_pX *polynomials = nullptr;
  NTL::vec_ZZ_p unknowns;
  unsigned int cores = 0;
  size_t nSplit;
  size_t *cumulSplit;
  std::thread *threads;
  
  // This should never occur...
  if (hashBuckets == nullptr) {
    std::cerr << "blind(). WARNING: Internal data structure hash buckets is not instantiated" << std::endl;
    return;
  }
  
  // FEEDBACK
  std::cout << this->id << ". Blinding...";
  std::cout.flush();
  
  // Number of threads
  if (nThreads == 0) {
    cores = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
    nThreads = cores;
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Allocate memory
  try {
    polynomials = new NTL::ZZ_pX[this->hashBuckets->getLength()];
    this->blindedData = new NTL::vec_ZZ_p[this->hashBuckets->getLength()];
    threads = new std::thread[nThreads];
    cumulSplit = new size_t[nThreads + 1];
  } catch (std::bad_alloc &) {
    std::cerr << "blind(). Error allocating memory." << std::endl;
    exit(2);
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Pad and populate the hash table
  padsize = NTL::bits(this->fieldsize);
  for (size_t i = 0; i < this->rawDataSize; i++) {
    this->rawData[i] = NTL::zeroPad(this->rawData[i], padsize);
    conv(z, this->rawData[i]);
    this->hashBuckets->add(z);
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Fill the hash table with random elements
  this->hashBuckets->conceal(*this->rndZZpgen);
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Generating polynomials
  for (size_t i = 0; i < this->hashBuckets->getLength(); i++) {
    polynomials[i] = NTL::BuildFromRoots(NTL::vector2VecZZp(*(this->hashBuckets->getBucket(i))));
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Generating random unknowns
  unknowns = this->generateUnknowns();
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Evaluating polynomials
  if (nThreads > 1) {
    // Compute the loop boundaries
    nSplit = (this->hashBuckets->getLength() + 1) / nThreads;
    cumulSplit[0] = 0;
    for (size_t i = 1; i <= nThreads; i++) {
      cumulSplit[i] = cumulSplit[i - 1] + nSplit;
    }
    // Do not exceed
    if (cumulSplit[nThreads] > this->hashBuckets->getLength()) {
      cumulSplit[nThreads] = this->hashBuckets->getLength();
    }
    
    // Run threads
    for (size_t i = 0; i < nThreads - 1; i++) {
      threads[i] = std::thread(polEval, this->blindedData, polynomials, unknowns, cumulSplit[i], cumulSplit[i + 1]);
    }
    
    // Do something in this thread too
    polEval(this->blindedData, polynomials, unknowns, cumulSplit[nThreads - 1], cumulSplit[nThreads]);
    
    // Wait for all threads
    for (size_t i = 0; i < nThreads - 1; i++) {
      threads[i].join();
    }
  } else {
    // Evaluating polynomials (single core)
    for (size_t j = 0; j < this->hashBuckets->getLength(); j++) {
      eval(this->blindedData[j], polynomials[j], unknowns);
    }
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Initialise key generator
  keygen.setSecretKey(this->secret);
  
  // Blind evaluations
  for (size_t j = 0; j < this->hashBuckets->getLength(); j++) {
    prf.setSecretKey(std::string((char *) keygen.next()));
    for (size_t i = 0; i < 2*this->hashBuckets->getMaxLoad() + 1; i++) {
      this->blindedData[j][i] = this->blindedData[j][i] + prf.next();
    }
  }
  
  // FEEDBACK
  std::cout << ". done." << std::endl;
}
//-----------------------------------------------------------------------------

NTL::ZZ_p ** EOPSIClient::delegationOutput(const std::string secretOtherParty) {
  std::string tmpKey;
  NTL::ZZ_p **q, tmp;
  NTL::ZZ_pX omega, omegaOther;
  size_t aIdx, omegaIdx, omegaOtherIdx;
  NTL::vec_ZZ_p unknowns;
  ByteKeyGenerator keygenOtherParty;
  
  // Initialise key generator
  keygen.setHashAlgorithm(this->strHashAlgorithm);
  keygen.setSecretKey(this->secret);
  
  try {
    q = new NTL::ZZ_p *[this->hashBuckets->getLength()];
    for (size_t i = 0; i < this->hashBuckets->getLength(); i++) {
      q[i] = new NTL::ZZ_p[2*this->hashBuckets->getMaxLoad() + 1];
    }
  } catch (std::bad_alloc &) {
    std::cerr << "delegationOutput(). Error allocating memory." << std::endl;
    exit(2);
  }
    
  // Not secret unknowns
  unknowns = generateUnknowns();
  
  // Initialise key generator
  keygenOtherParty.setHashAlgorithm(this->strHashAlgorithm);
  keygenOtherParty.setSecretKey(secretOtherParty);
  
  // Compute q
  aIdx = 0;
  omegaIdx = this->hashBuckets->getLength() * (2*this->hashBuckets->getMaxLoad() + 1);
  omegaOtherIdx = omegaIdx + this->hashBuckets->getLength() * (2*this->hashBuckets->getMaxLoad() + 1);
  for (size_t i = 0; i < this->hashBuckets->getLength(); i++) {
    for (size_t j = 0; j < 2*this->hashBuckets->getMaxLoad() + 1; j++) {
      conv(q[i][j], NTL::ZZFromBytes(keygen.generate(aIdx++), keygen.getLength()));
      if (j == 2*this->hashBuckets->getMaxLoad()) {
        conv(tmp, NTL::ZZFromBytes((const byte *) "\1", 1));
        NTL::SetCoeff(omega, j, tmp);
        NTL::SetCoeff(omegaOther, j, tmp);
        omegaIdx++;
        omegaOtherIdx++;
      }
      conv(tmp, NTL::ZZFromBytes(keygen.generate(omegaIdx++), keygen.getLength()));
      NTL::SetCoeff(omega, j, tmp);
      conv(tmp, NTL::ZZFromBytes(keygen.generate(omegaOtherIdx++), keygen.getLength()));
      NTL::SetCoeff(omegaOther, j, tmp);
      
      // Reuse of variable "q" to store q data
      q[i][j] = q[i][j] + eval(omega, unknowns[j]) + eval(omegaOther, unknowns[j]);
    }
  }
  
  return q;
}
//-----------------------------------------------------------------------------
