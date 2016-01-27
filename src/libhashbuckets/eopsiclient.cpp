/*
 * The secure computation server of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiclient.h"
//-----------------------------------------------------------------------------

EOPrivateSetIntersectionClient::EOPrivateSetIntersectionClient() {
  this->state = EOPSI_CLIENT_STATE_UNREADY;
}
//-----------------------------------------------------------------------------

EOPrivateSetIntersectionClient::~EOPrivateSetIntersectionClient() {
  
}
//-----------------------------------------------------------------------------

EOPSIClientState EOPrivateSetIntersectionClient::getState() const {
  return this->state;
}
//-----------------------------------------------------------------------------

void EOPrivateSetIntersectionClient::waitForServer() {
}
//-----------------------------------------------------------------------------
