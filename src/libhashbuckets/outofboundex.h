/*
 * Out of bound exception.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef OUT_OF_BOUND_EXCEPTION_H
#define OUT_OF_BOUND_EXCEPTION_H
//-----------------------------------------------------------------------------

#include <string>
#include <exception>
//-----------------------------------------------------------------------------

class OutOfBoundException: public std::exception
{
private:
  std::string message;

public:
  static const bool FATAL = true;
  static const bool NON_FATAL = !FATAL;
  
  OutOfBoundException(std::string message = "", bool fatal = false);
  
  virtual const char* what() const throw();
};
//-----------------------------------------------------------------------------

#endif // OUT_OF_BOUND_EXCEPTION_H
