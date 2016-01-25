/*
 * The secure computation server of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "asio.hpp"
#include "hspsiclient.h"
//-----------------------------------------------------------------------------

HSPrivateSetIntersectionClient::HSPrivateSetIntersectionClient() {
  this->state = HSPSI_CLIENT_STATE_UNREADY;
}
//-----------------------------------------------------------------------------

HSPrivateSetIntersectionClient::~HSPrivateSetIntersectionClient() {
  
}
//-----------------------------------------------------------------------------

HSPSIClientState HSPrivateSetIntersectionClient::getState() const {
  return this->state;
}
//-----------------------------------------------------------------------------

void HSPrivateSetIntersectionClient::waitForServer() {
  this->state = HSPSI_CLIENT_STATE_READY;
  try {
    asio::io_service asioService;
    asio::ip::tcp::acceptor *acceptor;
    asio::ip::tcp::endpoint endpoint;
    asio::ip::tcp::socket *socket;

    // Synchronous listening to
    endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), HSPSI_CLIENT_DEFAULT_PORT);
    acceptor = new asio::ip::tcp::acceptor(asioService, endpoint);

    while(true) {
      std::string message;
      
      socket = new asio::ip::tcp::socket(asioService);
      acceptor->accept(*socket);

      message = "message";

      asio::error_code ignoredError;
      asio::write(*socket, asio::buffer(message), ignoredError);
    }
  } catch (std::exception& e) {
    std::cerr << "waitForServer(). " << e.what() << std::endl;
  }
}
//-----------------------------------------------------------------------------
