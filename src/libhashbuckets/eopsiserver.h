/*
 * The secure computation server of the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Changyu Dong, Glasgow <changyu.dong@strath.ac.uk>
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_SERVER_H
#define EOPSI_SERVER_H
//-----------------------------------------------------------------------------

#include <string>
#include <map>
#include "eopsiparty.h"
//-----------------------------------------------------------------------------

class EOPSIClient;

class EOPSIServer : public EOPSIParty {
protected:
  std::map<std::string, EOPSIMessage*> storedData;
  EOPSIMessage msgToClient;
  
  virtual NTL::vec_ZZ_p * intersectionOutput(const std::string id, const std::string idOther, const NTL::ZZ tmpKey);
  
public:
  EOPSIServer(const NTL::ZZ fieldsize, const size_t length, const size_t height, const size_t degree, const std::string id = "");
  virtual ~EOPSIServer();
  
  virtual void receive(EOPSIMessage* msg) throw (ProtocolException);
  virtual bool isAuthorised(const EOPSIMessage* msg) const;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_SERVER_H
