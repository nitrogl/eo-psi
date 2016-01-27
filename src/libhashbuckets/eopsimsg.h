/*
 * The secure computation message in the EO-PSI protocol.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EOPSI_MESSAGE_H
#define EOPSI_MESSAGE_H
//-----------------------------------------------------------------------------

#include <string>
#include "byte.h"
//-----------------------------------------------------------------------------

/**
 * Type of message in the EO-PSI protocol
 */
typedef enum EOPSIMessageType {
  EOPSI_MESSAGE_OUTSOURCING_DATA,
  EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST,
  EOPSI_MESSAGE_CLOUD_COMPUTATION_REQUEST,
  EOPSI_MESSAGE_OUTSOURCING_COMPUTATION,
  EOPSI_MESSAGE_POLYNOMIAL,
} EOPSIMessageType;
//-----------------------------------------------------------------------------

/**
 * Messages to send through the protocol
 */
class EOPSIMessage {
protected:
  std::string partyId;
  EOPSIMessageType type;
  void* data;
  size_t len;
  
public:
  EOPSIMessage(void* data = nullptr, const size_t length = 0, const EOPSIMessageType type = EOPSI_MESSAGE_OUTSOURCING_DATA);
  virtual ~EOPSIMessage();
  
  virtual void setPartyId(const std::string& id);
  virtual std::string getPartyId() const;
  virtual void setType(const EOPSIMessageType type);
  virtual EOPSIMessageType getType() const;
  virtual void setData(void* data, const size_t length = 0);
  virtual void* getData() const;
  virtual size_t length() const;
};
//-----------------------------------------------------------------------------

#endif // EOPSI_MESSAGE_H
