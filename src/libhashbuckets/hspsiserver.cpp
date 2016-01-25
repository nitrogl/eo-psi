/*
 * The secure computation server of the protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <asio.hpp>
#include "hspsiserver.h"
//-----------------------------------------------------------------------------

HSPrivateSetIntersectionServer::HSPrivateSetIntersectionServer() {
  this->state = HSPSI_SERVER_STATE_UNREADY;
}
//-----------------------------------------------------------------------------

HSPrivateSetIntersectionServer::~HSPrivateSetIntersectionServer() {
  
}
//-----------------------------------------------------------------------------

void HSPrivateSetIntersectionServer::addClient(const HSPrivateSetIntersectionClient& client) {
  this->clients.push_back(client);
}
//-----------------------------------------------------------------------------

HSPSIServerState HSPrivateSetIntersectionServer::getState() const {
  return this->state;
}
//-----------------------------------------------------------------------------

void HSPrivateSetIntersectionServer::sendToClient(const HSPrivateSetIntersectionClient& client, const std::string msg) {
  try {
    asio::io_service io_service;

    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query("127.0.0.1", "2000");
    asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    asio::ip::tcp::socket socket(io_service);
    asio::connect(socket, endpoint_iterator);

    for (;;) {
      char buf[128];
      asio::error_code error;

      size_t len = socket.read_some(asio::buffer(buf, 128), error);

      if (error == asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw asio::system_error(error); // Some other error.

      std::cout.write(buf, len);
      std::cout.flush();
    }
  } catch (std::exception& e) {
    std::cerr << "sendToClient(). " << e.what() << std::endl;
  }
}
//-----------------------------------------------------------------------------
