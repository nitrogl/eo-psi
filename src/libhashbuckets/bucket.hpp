/*
 * A bucket (vector)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef BUCKET_TEMPLATE
#define BUCKET_TEMPLATE
//-----------------------------------------------------------------------------

#include <vector>
//-----------------------------------------------------------------------------

template <class T> class Bucket: public std::vector<T>
{
protected:
  bool marked;
  
public:
  Bucket();
  
  bool isMarked();
  void mark(bool value = true);
  void unmark();
};
//-----------------------------------------------------------------------------

template <class T> Bucket<T>::Bucket() : std::vector<T>() {
  this->marked = false;
}
//-----------------------------------------------------------------------------

template <class T> bool Bucket<T>::isMarked() {
  return this->marked;
}
//-----------------------------------------------------------------------------

template <class T> void Bucket<T>::mark(bool value) {
  this->marked = value;
}
//-----------------------------------------------------------------------------

template <class T> void Bucket<T>::unmark() {
  this->marked = false;
}
//-----------------------------------------------------------------------------

#endif // BUCKET_TEMPLATE
