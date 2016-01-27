/*
 * The secure computation server of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "eopsiserver.h"
//-----------------------------------------------------------------------------

EOPrivateSetIntersectionServer::EOPrivateSetIntersectionServer() {
  this->state = EOPSI_SERVER_STATE_UNREADY;
}
//-----------------------------------------------------------------------------

EOPrivateSetIntersectionServer::~EOPrivateSetIntersectionServer() {
  
}
//-----------------------------------------------------------------------------

void EOPrivateSetIntersectionServer::addClient(const EOPrivateSetIntersectionClient& client) {
  this->clients.push_back(client);
}
//-----------------------------------------------------------------------------

EOPSIServerState EOPrivateSetIntersectionServer::getState() const {
  return this->state;
}
//-----------------------------------------------------------------------------

void EOPrivateSetIntersectionServer::sendToClient(const EOPrivateSetIntersectionClient& client, const std::string msg) {
}
//-----------------------------------------------------------------------------
