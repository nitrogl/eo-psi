/*
 * Independent random stream for NTL ZZ big integers.
 * Compatibility with NTL 5.5.2.
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef INDEPENDENT_ZZ_RANDOM_STREAM_RETROCOMP_H
#define INDEPENDENT_ZZ_RANDOM_STREAM_RETROCOMP_H
//-----------------------------------------------------------------------------

#if NTL_MAJOR_VERSION < 9 || (NTL_MAJOR_VERSION == 9 && NTL_MINOR_VERSION < 4)
/*
 * The following code is a copy of some original you can find in
 * in NTL version 9.6.4.
 * This implementation is protected by the GPL license.
 * Copyright (C) Victor Shoup
 */

#if (NTL_BITS_PER_INT >= 32)
typedef unsigned int _ntl_uint32; // 32-bit word
#define NTL_BITS_PER_INT32 NTL_BITS_PER_INT
#else
// NOTE: C++ standard guarntees longs ar at least 32-bits wide,
// and this is also explicitly checked at builod time
typedef unsigned long _ntl_uint32; // 32-bit word
#define NTL_BITS_PER_INT32 NTL_BITS_PER_LONG
#endif

#define NTL_PRG_KEYLEN (32)

#if (NTL_BITS_PER_INT32 == 32)
#define INT32MASK(x) (x)
#else
#define INT32MASK(x) ((x) & _ntl_uint32(0xffffffff))
#endif


#define SHA256_BLOCKSIZE (64)
#define SHA256_HASHSIZE  (32)

#if (defined(__GNUC__) && (__GNUC__ >= 4))
// on relative modern versions of gcc, we can 
// decalare "restricted" pointers in C++
#define NTL_RESTRICT __restrict
#else
#define NTL_RESTRICT
#endif

// Original commented
// #define NTL_ZZRegister(x) NTL_THREAD_LOCAL static ZZ x; ZZWatcher _WATCHER__ ## x(x)
#define NTL_ZZRegister(x) ZZ x;

#define NTL_THREAD_LOCAL

namespace NTL {
  NTL_vector_decl(byte, vec_byte)
  
  void DeriveKey(unsigned char *key, long klen,  
               const unsigned char *data, long dlen);
  
  class RandomStream {
  private:
    _ntl_uint32 state[16];
    unsigned char buf[64];
    long pos;

    void do_get(unsigned char *NTL_RESTRICT res, long n);

  public:
    explicit
    RandomStream(const unsigned char *key);

    // No default constructor 
    // default copy and assignment

    void get(unsigned char *res, long n) 
    {
        // optimize short reads
        if (n >= 0 && n <= 64-pos) {
          long i;
          for (i = 0; i < n; i++) {
              res[i] = buf[pos+i];
          }
          pos += n;
        }
        else {
          do_get(res, n);
        }
    }

  };
}
#else
namespace NTL {
  typedef Vec<byte> vec_byte;
}
#endif
//-----------------------------------------------------------------------------

#endif // INDEPENDENT_ZZ_RANDOM_STREAM_RETROCOMP_H
