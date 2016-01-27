/*
 * A bucket (vector)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "outofboundex.h"
//-----------------------------------------------------------------------------

OutOfBoundException::OutOfBoundException(std::string message, bool fatal) : Exception(message, Exception::NON_FATAL, 1) {
  if (message.compare("") == 0) {
    this->message = "Did you try to insert or read an element out of allowed bound of some container?";
  } else {
    this->message = message;
  }
  
  this->setFatal(fatal);
  this->handleFatality();
}
//-----------------------------------------------------------------------------
