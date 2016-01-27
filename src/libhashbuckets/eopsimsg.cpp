/*
 * The secure computation message in the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "eopsimsg.h"
//-----------------------------------------------------------------------------

EOPSIMessage::EOPSIMessage(void* data, const size_t length, const EOPSIMessageType type) {
  this->setData(data, length);
  this->setType(type);
}
//-----------------------------------------------------------------------------

EOPSIMessage::~EOPSIMessage() {
}
//-----------------------------------------------------------------------------

void EOPSIMessage::setPartyId(const std::string& id) {
  this->partyId = id;
}
//-----------------------------------------------------------------------------

std::string EOPSIMessage::getPartyId() const {
  return this->partyId;
}
//-----------------------------------------------------------------------------

void EOPSIMessage::setType(const EOPSIMessageType type) {
  this->type = type;
}
//-----------------------------------------------------------------------------

EOPSIMessageType EOPSIMessage::getType() const {
  return this->type;
}
//-----------------------------------------------------------------------------

void EOPSIMessage::setData(void* data, const size_t length) {
  this->data = data;
  if (data == nullptr) {
    this->len = sizeof(data);
  } else {
    this->len = length;
  }
}
//-----------------------------------------------------------------------------

void* EOPSIMessage::getData() const {
  return this->data;
}
//-----------------------------------------------------------------------------

size_t EOPSIMessage::length() const {
  return this->len;
}
//-----------------------------------------------------------------------------

