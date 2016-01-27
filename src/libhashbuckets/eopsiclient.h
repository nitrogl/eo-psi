/*
 * The secure computation client of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_PRIVATE_SET_INTERSECTION_CLIENT_H
#define EOPSI_PRIVATE_SET_INTERSECTION_CLIENT_H
//-----------------------------------------------------------------------------

#define EOPSI_CLIENT_DEFAULT_PORT 2000
//-----------------------------------------------------------------------------

typedef enum EOPSIClientState {
  EOPSI_CLIENT_STATE_UNREADY = 0,
  EOPSI_CLIENT_STATE_READY = 1,
  EOPSI_CLIENT_STATE_BUSY = 2,
  EOPSI_CLIENT_STATE_WAITING = 3
} EOPSIClientState;
//-----------------------------------------------------------------------------

class EOPrivateSetIntersectionClient {
protected:
  EOPSIClientState state;
  
public:
  EOPrivateSetIntersectionClient();
  virtual ~EOPrivateSetIntersectionClient();
  
  virtual EOPSIClientState getState() const;
  virtual void waitForServer();
};
//-----------------------------------------------------------------------------

#endif // EOPSI_PRIVATE_SET_INTERSECTION_CLIENT_H
