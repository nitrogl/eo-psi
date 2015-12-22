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

/**
 * This exception can be thrown when something has got bounds and operations
 * performed on it do it for some out of bound elements, accesses, etc.
 * For example, N-ary arrays have bounds 0 and (N - 1).
 */
class OutOfBoundException: public std::exception
{
private:
  std::string message; ///< A specific message to give as a feedback.

public:
  static const bool FATAL = true;       ///< Fatal means that even if the exception is caught, it exits the program
  static const bool NON_FATAL = !FATAL; ///< Non fatal is a synonym of <i>standard</i> exception behaviour.
  
  /**
   * Default constructor.
   * 
   * @param message The message to set.
   * @param fatal true if launching this exception will exit the program, false otherwise
   */
  OutOfBoundException(std::string message = "", bool fatal = NON_FATAL);
  
  /**
   * The reason why the exception was thrown.
   */
  virtual const char* what() const throw();
};
//-----------------------------------------------------------------------------

#endif // OUT_OF_BOUND_EXCEPTION_H
