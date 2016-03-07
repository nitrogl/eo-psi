/*
 * A vector which can be marked.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef MARKED_VECTOR_TEMPLATE
#define MARKED_VECTOR_TEMPLATE
//-----------------------------------------------------------------------------

#include <vector>
//-----------------------------------------------------------------------------

/**
 * A bucket is the standard vector container which can be marked.
 */
template <typename T> class MarkedVector: public std::vector<T>
{
protected:
  bool marked;
  
public:
  MarkedVector();
  
  bool isMarked();
  void mark(bool value = true);
  void unmark();
};
//-----------------------------------------------------------------------------

template <typename T> MarkedVector<T>::MarkedVector() : std::vector<T>() {
  this->marked = false;
}
//-----------------------------------------------------------------------------

template <typename T> bool MarkedVector<T>::isMarked() {
  return this->marked;
}
//-----------------------------------------------------------------------------

template <typename T> void MarkedVector<T>::mark(bool value) {
  this->marked = value;
}
//-----------------------------------------------------------------------------

template <typename T> void MarkedVector<T>::unmark() {
  this->marked = false;
}
//-----------------------------------------------------------------------------

#endif // MARKED_VECTOR_TEMPLATE
