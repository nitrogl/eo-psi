/*
 * A bucket (vector)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "exception.h"
//-----------------------------------------------------------------------------

Exception::Exception(const std::string& message, const bool fatal, const int code) {
  this->setMessage(message);
  this->setFatal(fatal);
  this->setExitCode(code);
  
  this->handleFatality();
}
//-----------------------------------------------------------------------------

std::string Exception::getMessage() const {
  return this->message;
}
//-----------------------------------------------------------------------------

void Exception::setMessage(const std::string& message) {
  this->message = message;
}
//-----------------------------------------------------------------------------

bool Exception::isFatal() const {
  return this->fatal;
}
//-----------------------------------------------------------------------------

void Exception::setFatal(const bool fatal) {
  this->fatal = fatal;
}
//-----------------------------------------------------------------------------

int Exception::getExitCode() const {
  return this->exitCode;
}
//-----------------------------------------------------------------------------

void Exception::setExitCode(const int code) {
  this->exitCode = code;
}
//-----------------------------------------------------------------------------

const char* Exception::what() const throw() {
  return message.c_str();
}
//-----------------------------------------------------------------------------

void Exception::handleFatality() const {
  if (this->fatal) {
    std::cerr << "Fatal exception uncaught. " << this->message << std::endl;
    exit(this->exitCode);
  }
}
//-----------------------------------------------------------------------------
