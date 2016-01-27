/*
 * The secure computation server of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_PRIVATE_SET_INTERSECTION_SERVER_H
#define EOPSI_PRIVATE_SET_INTERSECTION_SERVER_H
//-----------------------------------------------------------------------------

#include <string>
#include <list>
#include "eopsiclient.h"
//-----------------------------------------------------------------------------

typedef enum EOPSIServerState {
  EOPSI_SERVER_STATE_UNREADY = 0,
  EOPSI_SERVER_STATE_READY = 1,
  EOPSI_SERVER_STATE_OUSOURCING = 2,
  EOPSI_SERVER_STATE_WAITING = 3
} EOPSIServerState;
//-----------------------------------------------------------------------------

class EOPrivateSetIntersectionServer {
protected:
  EOPSIServerState state;
  std::list<EOPrivateSetIntersectionClient> clients;
  
public:
  EOPrivateSetIntersectionServer();
  virtual ~EOPrivateSetIntersectionServer();
  
  virtual void addClient(const EOPrivateSetIntersectionClient& client);
  virtual EOPSIServerState getState() const;
  virtual void sendToClient(const EOPrivateSetIntersectionClient& client, const std::string msg);
};
//-----------------------------------------------------------------------------

#endif // EOPSI_PRIVATE_SET_INTERSECTION_SERVER_H
