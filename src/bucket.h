/*
 * A bucket (vector)
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef BUCKET_H
#define BUCKET_H
//-----------------------------------------------------------------------------

#include <vector>
//-----------------------------------------------------------------------------

template <class T> class Bucket: public std::vector<T>
{
};
//-----------------------------------------------------------------------------

#endif // BUCKET_H
