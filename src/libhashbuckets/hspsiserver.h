/*
 * The secure computation server of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HSPSI_PRIVATE_SET_INTERSECTION_SERVER_H
#define HSPSI_PRIVATE_SET_INTERSECTION_SERVER_H
//-----------------------------------------------------------------------------

#include <string>
#include <list>
#include "hspsiclient.h"
//-----------------------------------------------------------------------------

typedef enum HSPSIServerState {
  HSPSI_SERVER_STATE_UNREADY = 0,
  HSPSI_SERVER_STATE_READY = 1,
  HSPSI_SERVER_STATE_OUSOURCING = 2,
  HSPSI_SERVER_STATE_WAITING = 3
} HSPSIServerState;
//-----------------------------------------------------------------------------

class HSPrivateSetIntersectionServer {
protected:
  HSPSIServerState state;
  std::list<HSPrivateSetIntersectionClient> clients;
  
public:
  HSPrivateSetIntersectionServer();
  virtual ~HSPrivateSetIntersectionServer();
  
  virtual void addClient(const HSPrivateSetIntersectionClient& client);
  virtual HSPSIServerState getState() const;
  virtual void sendToClient(const HSPrivateSetIntersectionClient& client, const std::string msg);
};
//-----------------------------------------------------------------------------

#endif // HSPSI_PRIVATE_SET_INTERSECTION_SERVER_H
