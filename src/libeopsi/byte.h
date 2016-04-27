/*
 * Byte type definition
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef BYTE_TYPE_H
#define BYTE_TYPE_H
//-----------------------------------------------------------------------------

/*
 * This is useful for size_t which is supposed to be the correct 
 * type for sizes (as result of sizeof for example)
 */
#include <cstddef>
//-----------------------------------------------------------------------------

#ifndef byte
typedef unsigned char byte;
#endif
//-----------------------------------------------------------------------------

#endif // BYTE_TYPE_H
