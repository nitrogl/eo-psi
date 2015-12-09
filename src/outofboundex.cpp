/*
 * A bucket (vector)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include "outofboundex.h"
//-----------------------------------------------------------------------------

OutOfBoundException::OutOfBoundException(std::string message, bool fatal) {
  if (message.compare("") == 0) {
    this->message = "You have tried to insert or read an element out of allowed bound of some container";
  } else {
    this->message = message;
  }
  
  if (fatal) {
    std::cout << "Fatal exception uncaught (OutOfBoundException). " << this->message << std::endl;
    exit(1);
  }
}
//-----------------------------------------------------------------------------

const char* OutOfBoundException::what() const throw() {
  return message.c_str();
}
//-----------------------------------------------------------------------------
