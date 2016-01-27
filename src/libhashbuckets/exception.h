/*
 * Generic exception.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H
//-----------------------------------------------------------------------------

#include <string>
#include <exception>
#include "byte.h"
//-----------------------------------------------------------------------------

/**
 * This is an exception with generic message feedback, which is optional, and
 * which can be fatal, optional argument as well.
 */
class Exception: public std::exception
{
protected:
  std::string message; ///< A specific message to give as a feedback.
  bool fatal;
  int exitCode;
  
  void handleFatality() const;

public:
  static const bool FATAL = true;       ///< Fatal means that even if the exception is caught, it exits the program
  static const bool NON_FATAL = !FATAL; ///< Non fatal is a synonym of <i>standard</i> exception behaviour.
  
  /**
   * Default constructor.
   * 
   * @param message The message to set.
   * @param fatal true if launching this exception will exit the program, false otherwise
   */
  Exception(const std::string& message = "", const bool fatal = NON_FATAL, const int code = 1);
  
  /**
   * Get a message for the exception
   */
  virtual std::string getMessage() const;
  
  /**
   * Set a message for the exception
   * 
   * @param message the message to set
   */
  virtual void setMessage(const std::string& message);
  
  /**
   * Get the fatality state of the exception
   */
  virtual bool isFatal() const;
  
  /**
   * Set the fatality state of the exception
   * 
   * @param message the message to set
   */
  virtual void setFatal(const bool fatal = true);
  
  /**
   * Get the exit code of the exception
   */
  virtual int getExitCode() const;
  
  /**
   * Set the fatality state of the exception
   * 
   * @param message the message to set
   */
  virtual void setExitCode(const int code);
  
  
  /**
   * The reason why the exception was thrown.
   */
  virtual const char* what() const throw();
};
//-----------------------------------------------------------------------------

#endif // EXCEPTION_H
