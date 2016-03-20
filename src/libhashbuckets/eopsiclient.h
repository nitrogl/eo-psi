/*
 * The secure computation client of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_CLIENT_H
#define EOPSI_CLIENT_H
//-----------------------------------------------------------------------------

#include <thread>
#include <NTL/ZZ.h>
#include <NTL/vec_ZZ_p.h>
#include <NTL/ZZ_pX.h>
#include "eopsiparty.h"
#include "eopsiserver.h"
#include "hashbuckets.hpp"
#include "zzpprf.h"
#include "simplebm.h"
//-----------------------------------------------------------------------------

class EOPSIClient : public EOPSIParty {
protected:
  NTL::ZZ *rawData;
  size_t rawDataSize;
  size_t rawDataBits;
  NTL::vec_ZZ_p *blindedData;
  NTL::ZZ secret;
  HashBuckets<NTL::ZZ_p> *hashBuckets;
  NTL::vec_ZZ_p *q, *t;
  ZZpPRF *zzpprf;
  EOPSIMessage msgToCloud, msgToClient;
  NTL::vec_ZZ setcap;
  SimpleBenchmark bm;
  
  virtual size_t padSize() const;
  virtual void blind(unsigned int nThreads = 0);
  virtual NTL::vec_ZZ_p * delegationOutput(const NTL::ZZ secretOtherParty, const NTL::ZZ tmpKey);
  virtual bool intersect(const bool showStats = true);
  
public:
  EOPSIClient(HashBuckets<NTL::ZZ_p>& hashBuckets, const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id = "", const NTL::ZZ secret = NTL::ZZFromBytes((byte *)"Topsy Kretts", 12L));
  EOPSIClient(HashBuckets<NTL::ZZ_p>& hashBuckets, const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id = "", const byte *secret = nullptr, const size_t secretLen = 0);
  virtual ~EOPSIClient();
  
  virtual void receive(EOPSIMessage* msg) throw (ProtocolException);
  virtual bool isAuthorised(const EOPSIMessage* msg) const;
  
  virtual void setRawData(NTL::ZZ *rawData, const size_t size, const size_t dataBits, const unsigned int nThreads = 0);
  virtual NTL::ZZ * getRawData() const;
  virtual size_t getRawDataSize() const;
  virtual NTL::vec_ZZ_p * getBlindedData() const;
  virtual size_t getBlindedDataSize() const;
  virtual NTL::vec_ZZ getIntersection() const;
  
  virtual void setSecret(const NTL::ZZ secret);
  virtual NTL::ZZ getSecret() const;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_CLIENT_H
