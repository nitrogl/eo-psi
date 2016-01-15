/*
 * Some useful functions using NTL.
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <sstream>
#include "byte.h"
#include "ntlmiss.h"
//-----------------------------------------------------------------------------

namespace NTL {
  unsigned long log2(const ZZ &z) {
    ZZ one, zl;
    unsigned long l2;
    
    one = to_ZZ("1");
    zl = z;
    l2 = 0L;
    while (zl > one) {
      zl = zl >> 1;
      l2++;
    }
    return l2;
  }
  //---------------------------------------------------------------------------

  unsigned long bits(const ZZ &z) {
    return log2(z) + 1L;
  }
  //---------------------------------------------------------------------------

  unsigned long bits(const ZZ_p &zp) {
    ZZ z = rep(zp);
    return log2(z) + 1L;
  }
  //---------------------------------------------------------------------------
  
  void vector2VecZZp(vec_ZZ_p &zp, const std::vector<ZZ_p> &v) {
    zp.kill();
    zp.SetLength(v.size());
    for (size_t i = 0; i < v.size(); i++) {
      append(zp, v[i]);
    }
  }
  //---------------------------------------------------------------------------
  
  vec_ZZ_p vector2VecZZp(const std::vector<ZZ_p> &v) {
    vec_ZZ_p res;
    
    res.SetLength(v.size());
    for (size_t i = 0; i < v.size(); i++) {
      append(res, v[i]);
    }
    
    return res;
  }
  //---------------------------------------------------------------------------

};
