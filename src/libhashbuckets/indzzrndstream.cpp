/*
 * Independent random stream for NTL ZZ big integers.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include "indzzrndstream.h"
//-----------------------------------------------------------------------------

static inline unsigned long WordFromBytes(const unsigned char *buf, long n);
//-----------------------------------------------------------------------------

/*
 * The following code is an adaptation of the original SetSeed functions
 * in NTL version 9.6.2 for the purpose of using custom random streams.
 * This implementation is protected by the GPL license.
 * 
 * Copyright (C) Victor Shoup
 * Copyright (C) 2016 Roberto Metere
 */
byte* IndependentZZRandomStream::zzSeed2byte(const NTL::ZZ& seed) const {
  long nb = NumBytes(seed);

  NTL::Vec<unsigned char> buf;
  buf.SetLength(nb);

  NTL::BytesFromZZ(buf.elts(), seed, nb);
  
//     if (nb < 0) LogicError("SetSeed: bad args"); // Is that possible?

  NTL::Vec<unsigned char> key;
  key.SetLength(NTL_PRG_KEYLEN);
  NTL::DeriveKey(key.elts(), NTL_PRG_KEYLEN, buf.elts(), nb);

  return key.elts();
}
//-----------------------------------------------------------------------------

IndependentZZRandomStream::IndependentZZRandomStream(const NTL::ZZ& seed) : RandomStream(zzSeed2byte(seed)) {
}
//-----------------------------------------------------------------------------

IndependentZZRandomStream::~IndependentZZRandomStream() {}
//-----------------------------------------------------------------------------

/*
 * The following code is an adaptation of the original RandomBnd function
 * in NTL version 9.6.2 for the purpose of using custom random streams.
 * This implementation is protected by the GPL license.
 * 
 * Copyright (C) Victor Shoup
 */
NTL::ZZ IndependentZZRandomStream::randomBnd(const NTL::ZZ& bnd) {
  using namespace NTL;
  ZZ x;
  
  if (bnd <= 1) {
      x = 0;
      return x;
  }

  long l = NumBits(bnd);
  long nb = (l+7)/8;

  if (nb <= 3) {
      long lbnd = conv<long>(bnd);
      unsigned char lbuf[3];
      long ltmp;
      
      x.SetSize((l + NTL_ZZ_NBITS - 1)/NTL_ZZ_NBITS);
      // pre-allocate to ensure strong ES
      do {
        this->get(lbuf, nb);
        ltmp = long(WordFromBytes(lbuf, nb) & ((1UL << l)-1UL));
      } while (ltmp >= lbnd);

    conv(x, ltmp);
    return x;
  }

  // deal with possible alias
  NTL_ZZRegister(tmp_store);
  const ZZ& bnd_ref = ((&x == &bnd) ? (tmp_store = bnd) : bnd); 


  NTL_ZZRegister(hbnd);
  RightShift(hbnd, bnd_ref, (nb-2)*8);
  long lhbnd = conv<long>(hbnd);

  unsigned long mask = (1UL << (16 - nb*8 + l)) - 1UL;

  NTL_THREAD_LOCAL static Vec<unsigned char> buf_mem;
  Vec<unsigned char>::Watcher watch_buf_mem(buf_mem);
  buf_mem.SetLength(nb);
  unsigned char *buf = buf_mem.elts();

  unsigned char hbuf[2];

  x.SetSize((l + NTL_ZZ_NBITS - 1)/NTL_ZZ_NBITS);
  // pre-allocate to ensure strong ES
  for (;;) {
      this->get(hbuf, 2);
      long hpart = long(WordFromBytes(hbuf, 2) & mask);

      if (hpart > lhbnd) continue;

      this->get(buf, nb-2);
      buf[nb-2] = ((unsigned long) hpart);
      buf[nb-1] = ((unsigned long) hpart) >> 8; 

      ZZFromBytes(x, buf, nb);
      if (hpart < lhbnd || x < bnd_ref) break;
  }
  
  return x;
}
//-----------------------------------------------------------------------------

/*
 * The following code is a copy of the original WordFromBytes function
 * in NTL version 9.6.2.
 * This implementation is protected by the GPL license.
 * Copyright (C) Victor Shoup
 */
static inline
unsigned long WordFromBytes(const unsigned char *buf, long n)
{
   unsigned long res = 0;
   long i;

   for (i = n-1; i >= 0; i--)
      res = (res << 8) | buf[i];

   return res;
}
//-----------------------------------------------------------------------------
  