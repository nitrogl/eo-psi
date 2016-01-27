/*
 * Out of bound exception.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef PROTOCOL_EXCEPTION_H
#define PROTOCOL_EXCEPTION_H
//-----------------------------------------------------------------------------

#include "exception.h"
//-----------------------------------------------------------------------------

/**
 * This exception can be thrown when (part of) a protocol fails.
 */
class ProtocolException: public Exception
{
public:
  ProtocolException(const std::string& message = "", const bool fatal = NON_FATAL, const int code = 1) : Exception(message, fatal, code) {}
};
//-----------------------------------------------------------------------------

#endif // PROTOCOL_EXCEPTION_H
