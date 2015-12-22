/*
 * A bucket (vector)
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
template <class T> class MarkedVector: public std::vector<T>
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

template <class T> MarkedVector<T>::MarkedVector() : std::vector<T>() {
  this->marked = false;
}
//-----------------------------------------------------------------------------

template <class T> bool MarkedVector<T>::isMarked() {
  return this->marked;
}
//-----------------------------------------------------------------------------

template <class T> void MarkedVector<T>::mark(bool value) {
  this->marked = value;
}
//-----------------------------------------------------------------------------

template <class T> void MarkedVector<T>::unmark() {
  this->marked = false;
}
//-----------------------------------------------------------------------------

#endif // MARKED_VECTOR_TEMPLATE
