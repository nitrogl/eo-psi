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
      zp[i] = v[i];
    }
  }
  //---------------------------------------------------------------------------
  
  vec_ZZ_p vector2VecZZp(const std::vector<ZZ_p> &v) {
    vec_ZZ_p res;
    
    res.SetLength(v.size());
    for (size_t i = 0; i < v.size(); i++) {
      res[i] = v[i];
    }
    
    return res;
  }
  //---------------------------------------------------------------------------
  
  void array2VecZZp(vec_ZZ_p &zp, const ZZ_p *v, const size_t dim) {    
    if (v == nullptr) {
      return;
    }
    
    zp.kill();
    zp.SetLength(dim);
    for (size_t i = 0; i < dim; i++) {
      zp[i] = v[i];
    }
  }
  //---------------------------------------------------------------------------
  
  vec_ZZ_p array2VecZZp(const ZZ_p *v, const size_t dim) {
    vec_ZZ_p res;
    
    if (v == nullptr) {
      return res;
    }
    
    res.SetLength(dim);
    for (size_t i = 0; i < dim; i++) {
      res[i] = v[i];
    }
    
    return res;
  }
  //---------------------------------------------------------------------------
  
  NTL::ZZ zeroPad(const NTL::ZZ z, const size_t padsize) {
    unsigned int bs, topad;
    NTL::ZZ pz;
    
    pz = z;
    bs = NTL::bits(z);
    topad = (padsize > bs ? padsize - bs : 0L);
    if (topad > 0L) {
  //       std::cout << "Padding: " << NTL::bits(pz) << " -> ";
      pz = pz << topad - 1;
  //       std::cout << NTL::bits(pz) << std::endl;
    }
    
    return pz;
  }
  //---------------------------------------------------------------------------

}
