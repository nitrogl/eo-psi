/*
 * The secure computation client of the EO-PSI protocol.
 * 
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
#include "zzprandgen.h"
#include "strrandgen.h"
//-----------------------------------------------------------------------------

class EOPSIClient : public EOPSIParty {
protected:
  NTL::ZZ *rawData;
  size_t rawDataSize;
  NTL::vec_ZZ_p *blindedData;
  std::string secret;
  HashBuckets<NTL::ZZ_p> *hashBuckets;
  RandomStringGenerator rndStrgen;
  
  virtual void blind(unsigned int nThreads = 0);
  virtual NTL::ZZ_p ** delegationOutput(const std::string secretOtherParty, const std::string tmpKey);
  
public:
  EOPSIClient(HashBuckets<NTL::ZZ_p>& hashBuckets, const NTL::ZZ& fieldsize, const std::string& id = "", const std::string& secret = "Topsy Kretts");
  virtual ~EOPSIClient();
  
  virtual void receive(EOPSIMessage& msg) throw (ProtocolException);
  virtual bool isAuthorised(const EOPSIMessage& msg) const;
  
  virtual void setFieldsize(const NTL::ZZ& fieldsize);
  
  virtual void setRawData(NTL::ZZ *rawData, const size_t size, const unsigned int nThreads = 0);
  virtual NTL::ZZ * getRawData() const;
  virtual size_t getRawDataSize() const;
  virtual NTL::vec_ZZ_p * getBlindedData() const;
  virtual size_t getBlindedDataSize() const;
  
  virtual void setSecret(const std::string& secret);
  virtual std::string getSecret() const;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_CLIENT_H
