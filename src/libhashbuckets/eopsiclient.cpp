/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiclient.h"
#include "ntlmiss.h"
#include "shastr.h"
#include "stringkeygen.h"
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
  EOPSIParty *sender;
  std::string msgClaimedId;
  
  if (msg.getPartyId() == this->id) {
    throw ProtocolException("Self-messaging is not allowed in EO-PSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw ProtocolException("Party \"" + msg.getPartyId() + "\" is not authorised");
  }
  
  // Get the sender
  sender = getPartyById(msg.getPartyId());
  
  switch(msg.getType()) {
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
      msgClaimedId = (char *) msg.getData();
      if (msgClaimedId != sender->getId()) {
	throw ProtocolException("Id mismatch between sender and message data");
      }
      
      std::cout << "not fully implemented. I, " << id << ", received \"" << (&((byte *) msg.getData())[msgClaimedId.length() + 1]) << "\" from " << sender->getId() << std::endl;
      break;
      
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
      break;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      break;
      
    case EOPSI_MESSAGE_POLYNOMIAL:
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

void EOPSIClient::blind(unsigned int nThreads) {
  size_t padsize;
  NTL::ZZ_p z;
  NTL::ZZ_pX *polynomials = nullptr;
  NTL::vec_ZZ_p unknowns;
  unsigned int cores = 0;
  size_t nSplit;
  size_t *cumulSplit;
  std::thread *threads;
  StringZZpKeyGenerator prf;
  StringKeyGenerator keygen;
  
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
  unknowns.SetLength(2*this->hashBuckets->getMaxLoad() + 1);
  for (size_t j = 0; j < 2*this->hashBuckets->getMaxLoad() + 1; j++) {
    append(unknowns, this->rndZZpgen->next());
  }
  
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
  
  // Initialise key generator and pseudo-random function
  keygen.setHashAlgorithm(this->strHashAlgorithm);
  keygen.setSecretKey(this->secret);
  prf.setHashAlgorithm(this->strHashAlgorithm);
  prf.setModulo(this->fieldsize);
  
  // Blind evaluations
  for (size_t j = 0; j < this->hashBuckets->getLength(); j++) {
    prf.setSecretKey(keygen.next());
    for (size_t i = 0; i < 2*this->hashBuckets->getMaxLoad() + 1; i++) {
      this->blindedData[j][i] = this->blindedData[j][i] + prf.next();
    }
  }
  
  // FEEDBACK
  std::cout << ". done." << std::endl;
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
}
//-----------------------------------------------------------------------------

std::string EOPSIClient::getSecret() const {
  return this->secret;
}
//-----------------------------------------------------------------------------

void EOPSIClient::setFieldsize(const NTL::ZZ& fieldsize) {
  this->fieldsize = fieldsize;
  NTL::ZZ_p::init(fieldsize);
}
//-----------------------------------------------------------------------------

NTL::ZZ EOPSIClient::getFieldsize() const {
  return this->fieldsize;
}
//-----------------------------------------------------------------------------
