/*
 * Byte type definition
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef CAMEL_CASE_REFACTORING_H
#define CAMEL_CASE_REFACTORING_H
//-----------------------------------------------------------------------------

#include <string>
#include <sstring>
#include <map>
#include <cstddef>
//-----------------------------------------------------------------------------

#ifndef String
typedef std::string String;
#endif
#ifndef StringStream
typedef std::stringstream StringStream;
#endif

#ifndef OrderedMap
typedef std::map OrderedMap;
#endif

#ifndef Map
typedef std::unordered_map Map;
#endif

#ifndef Vector
typedef std::vector Vector;
#endif

#ifndef Map
typedef std::unordered_map Map;
#endif
//-----------------------------------------------------------------------------

#endif // BYTE_TYPE_H
