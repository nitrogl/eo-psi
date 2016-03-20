/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <NTL/ZZ_pXFactoring.h>
#include "eopsiclient.h"
#include "ntlmiss.h"
#include "zzppoly.h"
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

EOPSIClient::EOPSIClient(HashBuckets<NTL::ZZ_p>& hashBuckets, const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id, const NTL::ZZ secret) : EOPSIParty(EOPSI_PARTY_CLIENT, fieldsize, length, height, degree, id) {
  this->rawData = nullptr;
  this->rawDataSize = 0;
  this->blindedData = nullptr;
  this->hashBuckets = &hashBuckets;
  this->setSecret(secret);
  this->q = nullptr;
  this->t = nullptr;
  
  try {
    this->zzpprf = new ZZpPRF(this->fieldsize);
  } catch(std::bad_alloc&) {
    std::cerr << "EOPSIClient(). Error allocating memory." << std::endl;
    exit(2);
  }
}
//-----------------------------------------------------------------------------

EOPSIClient::EOPSIClient(HashBuckets<NTL::ZZ_p>& hashBuckets, const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id, const byte *secret, const size_t secretLen) : EOPSIClient(hashBuckets, fieldsize, length, height, degree, id, NTL::ZZFromBytes(secret, secretLen)) {
}
//-----------------------------------------------------------------------------

EOPSIClient::~EOPSIClient() {
  delete this->zzpprf;
  
  if (this->blindedData != nullptr) {
    delete [] this->blindedData;
  }
}
//-----------------------------------------------------------------------------

void EOPSIClient::receive(EOPSIMessage* msg) throw (ProtocolException) {
  EOPSIParty *sender, *cloud;
  std::string msgClaimedId;
  NTL::ZZ otherSecret;
  byte *otherSecretBytes;
  NTL::ZZ tmpKey;
  byte *tmpKeyBytes;
  byte *data = nullptr;
  size_t dataLen, tmpKeyByteSize, i;
  std::map<std::string, EOPSIParty*>::const_iterator mi;
  NTL::vec_ZZ_p setcap;
  
  if (msg->getPartyId() == this->id) {
    throw ProtocolException("Self-messaging is not allowed in EO-PSI protocol");
  }
  
  // Is the sender authorised?
  if (!isAuthorised(msg)) {
    throw ProtocolException("Party \"" + msg->getPartyId() + "\" is not authorised by " + this->getId() + ".");
  }
  
  // Get the sender
  sender = getPartyById(msg->getPartyId());
  
  switch(msg->getType()) {
    case EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST:
      // Check sender and message content
      msgClaimedId = (char *) msg->getData();
      if (msgClaimedId != sender->getId()) {
        throw ProtocolException("Id mismatch between sender and message data");
      }
      
      /*
       * 1. Send outsourcing request to the cloud
       */
      // Prepare message for the Cloud
      tmpKeyByteSize = NTL::bytes(DEFAULT_KEY_BITSIZE);
      do {
        tmpKey = NTL::RandomBits_ZZ(DEFAULT_KEY_BITSIZE);
      } while (NTL::NumBits(tmpKey) < DEFAULT_KEY_BITSIZE);
//       tmpKey = NTL::ZZFromBytes((byte*) "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\xFF", 16L); // TODO: [[REMOVE ME]] - just useful to get always the same result
//       std::cerr << "tmpKey: " << tmpKey << std::endl;
      dataLen = this->getId().length() + 1 + sender->getId().length() + 1 + tmpKeyByteSize + 1;
//       std::cerr << "dataLen: " << dataLen << std::endl;
      
      try {
        tmpKeyBytes = new byte[tmpKeyByteSize];
        data = new byte[dataLen];
      } catch (std::bad_alloc &) {
        std::cerr << "receive(). Error allocating memory." << std::endl;
        exit(2);
      }
      
      strcpy((char *) data, &this->getId()[0]);
      strcpy((char *) (data + this->getId().length() + 1), &sender->getId()[0]);
      NTL::BytesFromZZ(tmpKeyBytes, tmpKey, tmpKeyByteSize);
      for (i = 0; i < tmpKeyByteSize; i++) {
        data[this->getId().length() + 1 + sender->getId().length() + 1 + i] = tmpKeyBytes[i];
      }
      data[dataLen - 1] = '\0'; // Correctly end data
      delete [] tmpKeyBytes;
      
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
      otherSecretBytes = &((byte *) msg->getData())[msgClaimedId.length() + 1];
      otherSecret = NTL::ZZFromBytes(otherSecretBytes, 3);
      std::cout << id << ". Received \"" << otherSecret << "\" from " << sender->getId() << "." << std::endl;
      
      // Send the message to cloud
      std::cout << this->getId() << ". Sending cloud computation request with key \"" << tmpKey << "\" to " << cloud->getId() << "." << std::endl;
      try {
        this->send(*cloud, &msgToCloud);
      } catch (ProtocolException &e) {
        std::cerr << "receive(). " << e.what() << std::endl;
        exit(2);
      }
      
      /*
       * 2. Send q to client
       */
      q = delegationOutput(otherSecret, tmpKey);
      msgToClient.setData((void *) q, 1);
      msgToClient.setType(EOPSI_MESSAGE_POLYNOMIAL);
      msgToClient.setPartyId(this->getId());
      this->send(*sender, &msgToClient);
      break;
      
    case EOPSI_MESSAGE_OUTPUT_COMPUTATION:
      t = (NTL::vec_ZZ_p *) msg->getData();
      // Reception feedback
      std::cout << "I, " << id << ", received \"" << t[0][0] << ", ...\" from " << sender->getId() << "." << std::endl;
      if (intersect()) {
        delete [] t;
        delete [] q;
      }
      break;
      
    case EOPSI_MESSAGE_POLYNOMIAL:
      q = (NTL::vec_ZZ_p *) msg->getData();
      // Reception feedback
      std::cout << "I, " << id << ", received \"" << q[0][0] << ", ...\" from " << sender->getId() << "." << std::endl;
      if (intersect()) {
        delete [] t;
        delete [] q;
      }
      break;
      
    case EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST:
    case EOPSI_MESSAGE_OUTSOURCING_DATA:
      throw ProtocolException("The protocol forbids passing this type of message to clients.");
      
    default:
      throw ProtocolException("Unknown message type detected.");
  }
}
//-----------------------------------------------------------------------------

bool EOPSIClient::isAuthorised(const EOPSIMessage *msg) const {
  EOPSIParty *sender;
  
  if (msg == nullptr) {
    std::cerr << "isAuthorised(). WARNING: Null pointer as message has passed." << std::endl;
    return false;
  }
  
  sender = getPartyById(msg->getPartyId());
  if (!hasAuthenticated(*sender)) {
    return false;
  }
  
  switch(msg->getType()) {
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

void EOPSIClient::setRawData(NTL::ZZ *rawData, const size_t size, const size_t dataBits, const unsigned int nThreads) {
  if (rawData == nullptr || size == 0) {
    std::cerr << "setRawData(). WARNING: Empty data to store; nothing to do" << std::endl;
  } else {
    this->rawData = rawData;
    this->rawDataSize = size;
    this->rawDataBits = dataBits;
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
    return (this->degree)*this->length;
  } else {
    std::cerr << "getBlindedDataSize(). WARNING: Internal data structure hash buckets is not instantiated" << std::endl;
    return 0;
  }
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ EOPSIClient::getIntersection() const {
  return this->setcap;
}
//-----------------------------------------------------------------------------

void EOPSIClient::setSecret(const NTL::ZZ secret) {
  this->secret = secret;
}
//-----------------------------------------------------------------------------

NTL::ZZ EOPSIClient::getSecret() const {
  return this->secret;
}
//-----------------------------------------------------------------------------

size_t EOPSIClient::padSize() const {
  return this->fieldbitsize - this->rawDataBits - 1;
}
//-----------------------------------------------------------------------------

void EOPSIClient::blind(unsigned int nThreads) {
  size_t index;
  NTL::ZZ_p zzpSeed, z;
  NTL::ZZ_pX *polynomials = nullptr;
  unsigned int cores = 0;
  size_t nSplit;
  size_t *cumulSplit;
  std::thread *threads;
  size_t i, j;
  
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
  
#ifndef NTL_THREADS
  std::cerr << "blind(). Your NTL library is not thread-safe: using only 1 thread." << std::endl;
#endif
  if (nThreads > 1) {
    std::cerr << "blind(). Multithreading not (yet) implemented." << std::endl;
  }
  nThreads = 1;
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Allocate memory
  try {
    polynomials = new NTL::ZZ_pX[this->length];
    this->blindedData = new NTL::vec_ZZ_p[this->length];
    threads = new std::thread[nThreads];
    cumulSplit = new size_t[nThreads + 1];
  } catch (std::bad_alloc &) {
    std::cerr << "blind(). Error allocating memory." << std::endl;
    exit(2);
  }
  
  // Set blided data length of vectors:
  for (j = 0; j < this->length; j++) {
    this->blindedData[j].SetLength(this->degree);
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Pad and populate the hash table
  for (i = 0; i < this->rawDataSize; i++) {
    conv(z, NTL::zeroPad(this->rawData[i], this->padSize()));
    this->hashBuckets->add(z);
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Fill the hash table with random elements
  this->hashBuckets->conceal(*zzpprf);
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Generating polynomials
  for (i = 0; i < this->length; i++) {
    polynomials[i] = NTL::BuildFromRoots(NTL::vector2VecZZp((*this->hashBuckets)[i]));
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Generating random unknowns
  this->unknowns = this->getUnknowns(this->degree);
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Evaluating polynomials
  if (nThreads > 1) {
    // Compute the loop boundaries
    nSplit = (this->length + 1) / nThreads;
    cumulSplit[0] = 0;
    for (i = 1; i <= nThreads; i++) {
      cumulSplit[i] = cumulSplit[i - 1] + nSplit;
    }
    // Do not exceed
    if (cumulSplit[nThreads] > this->length) {
      cumulSplit[nThreads] = this->length;
    }
    
    // Run threads
    for (i = 0; i < nThreads - 1; i++) {
      threads[i] = std::thread(polEval, this->blindedData, polynomials, unknowns, cumulSplit[i], cumulSplit[i + 1]);
      threads[i].join();
    }
    
    // Do something in this thread too
    polEval(this->blindedData, polynomials, unknowns, cumulSplit[nThreads - 1], cumulSplit[nThreads]);
    
    // Wait for all threads
    for (i = 0; i < nThreads - 1; i++) {
//       threads[i].join();
//       threads[i].join();
    }
  } else {
    // Evaluating polynomials (single core)
    for (j = 0; j < this->length; j++) {
//       std::cout << "pol: " << polynomials[j] << std::endl;
      eval(this->blindedData[j], polynomials[j], unknowns);
    }
  }
  
  // FEEDBACK
  std::cout << ".";
  std::cout.flush();
  
  // Blind evaluations
  conv(zzpSeed, this->secret);
  index = 0;
  for (j = 0; j < this->length; j++) {
    for (i = 0; i < this->degree; i++) {
      this->blindedData[j][i] = this->blindedData[j][i] + zzpprf->generate(zzpSeed, index++, this->fieldbitsize);
    }
  }
  
  // FEEDBACK
  std::cout << ". done." << std::endl;
  
  // Release memory
  delete [] polynomials;
  delete [] threads;
  delete [] cumulSplit;
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ_p * EOPSIClient::delegationOutput(const NTL::ZZ secretOtherParty, const NTL::ZZ tmpKey) {
  NTL::ZZ_p tmp;
  NTL::vec_ZZ_p *dataA, *dataB, *result;
  size_t index;
  NTL::ZZ_p zzpSeed, zzpSeedOther;
  
  try {
    dataA = new NTL::vec_ZZ_p[this->length];
    dataB = new NTL::vec_ZZ_p[this->length];
    for (size_t i = 0; i < this->length; i++) {
      dataA[i].SetLength(this->degree);
      dataB[i].SetLength(this->degree);
    }
  } catch (std::bad_alloc &) {
    std::cerr << "delegationOutput(). Error allocating memory." << std::endl;
    exit(2);
  }
  
  conv(zzpSeed, this->secret);
  conv(zzpSeedOther, secretOtherParty);
  index = 0;
  NTL::ZZ_p zero;
  conv(zero, 0);
  for (size_t j = 0; j < this->length; j++) {
    for (size_t i = 0; i < this->degree; i++) {
      dataA[j][i] = zzpprf->generate(zzpSeed, index, this->fieldbitsize);
      dataB[j][i] = zzpprf->generate(zzpSeedOther, index, this->fieldbitsize);
      index++;
    }
  }
  
  result = computeTOrQ(tmpKey, dataA, dataB);
  
  delete [] dataA;
  delete [] dataB;
  
  return result;
}
//-----------------------------------------------------------------------------

bool EOPSIClient::intersect(const bool showStats) {
  NTL::vec_ZZ_p *diff;
  ZZpPolynomials polys(this->padSize());
  
  if (this->q == nullptr || this->t == nullptr) {
    // Suppress this message...
//     std::cerr << "intersect(). Either q or t not set." << std::endl;
    return false;
  }
  
  try {
    diff = new NTL::vec_ZZ_p[this->length];
    for (size_t i = 0; i < this->length; i++) {
      diff[i].SetLength(this->degree);
    }
  } catch (std::bad_alloc &) {
    std::cerr << "intersect(). Error allocating memory." << std::endl;
    exit(2);
  }
  
  // Remove random terms
  for (size_t j = 0; j < this->length; j++) {
    for (size_t i = 0; i < this->degree; i++) {
      diff[j][i] = this->t[j][i] - this->q[j][i];
//       std::cout << "d[" << j << ", " << i << "] = " << diff[j][i] << std::endl;
    }
  }
  
  // Not secret unknowns
  unknowns = this->getUnknowns();
  
//   std::cout << "length: " << this->length << std::endl;
    
  // Compute intersection
//   bm.start();
//   polys.interpolate(unknowns, diff, this->length);
//   bm.step("interp");
//   polys.factorise();
//   bm.stop("fact");
//   this->setcap = polys.findIntersection();
//   bm.stop("fact");
//   
//   std::cout << "interp: " << bm.benchmark("interp").count()/1000000. << " s" << std::endl;
//   std::cout << "fact: " << bm.benchmark("fact").count()/1000000. << " s" << std::endl;
//   std::cout << "total: " << bm.cumulativeBenchmark().count()/1000000. << " s" << std::endl;
//   std::cout << "Intersection size: " << this->setcap.length() << std::endl;
  
  // Compute intersection
  bm.start();
  this->setcap = polys.findIntersection(unknowns, diff, this->length);
  bm.stop();
  
  if (showStats) {
    std::cout << " -- Interpolation"
              << "\n\t   Avg: " << polys.bmInt.average()/1000. << " ms"
              << "\n\tStdDev: " << polys.bmInt.standardDeviation()/1000. << " ms"
              << "\n\t Total: " << polys.bmInt.cumulativeBenchmark().count()/1000. << " ms\n" << std::endl;
    std::cout << " -- Monicisation"
              << "\n\t   Avg: " << polys.bmMon.average()/1000. << " ms"
              << "\n\tStdDev: " << polys.bmMon.standardDeviation()/1000. << " ms"
              << "\n\t Total: " << polys.bmMon.cumulativeBenchmark().count()/1000. << " ms\n" << std::endl;
    std::cout << " -- Factorisation"
              << "\n\t   Avg: " << polys.bmFac.average()/1000. << " ms"
              << "\n\tStdDev: " << polys.bmFac.standardDeviation()/1000. << " ms"
              << "\n\t Total: " << polys.bmFac.cumulativeBenchmark().count()/1000. << " ms\n" << std::endl;
    std::cout << " -- Finding Intersection"
              << "\n\t   Avg: " << polys.bmCap.average()/1000. << " ms"
              << "\n\tStdDev: " << polys.bmCap.standardDeviation()/1000. << " ms"
              << "\n\t Total: " << polys.bmCap.cumulativeBenchmark().count()/1000. << " ms\n" << std::endl;
              
    std::cout << "Polynomials: " << this->length << std::endl;
    std::cout << "Total time: " << bm.cumulativeBenchmark().count()/1000000. << " s" << std::endl;
    std::cout << "Intersection size: " << this->setcap.length() << std::endl;
  }
  
  delete [] diff;
  return true;
}
//-----------------------------------------------------------------------------
