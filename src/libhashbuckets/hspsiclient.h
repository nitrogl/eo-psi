/*
 * The secure computation client of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef HSPSI_PRIVATE_SET_INTERSECTION_CLIENT_H
#define HSPSI_PRIVATE_SET_INTERSECTION_CLIENT_H
//-----------------------------------------------------------------------------

#define HSPSI_CLIENT_DEFAULT_PORT 2000
//-----------------------------------------------------------------------------

typedef enum HSPSIClientState {
  HSPSI_CLIENT_STATE_UNREADY = 0,
  HSPSI_CLIENT_STATE_READY = 1,
  HSPSI_CLIENT_STATE_BUSY = 2,
  HSPSI_CLIENT_STATE_WAITING = 3
} HSPSIClientState;
//-----------------------------------------------------------------------------

class HSPrivateSetIntersectionClient {
protected:
  HSPSIClientState state;
  
public:
  HSPrivateSetIntersectionClient();
  virtual ~HSPrivateSetIntersectionClient();
  
  virtual HSPSIClientState getState() const;
  virtual void waitForServer();
};
//-----------------------------------------------------------------------------

#endif // HSPSI_PRIVATE_SET_INTERSECTION_CLIENT_H
