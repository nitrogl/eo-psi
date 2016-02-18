/*
 * Independent random stream for NTL ZZ big integers.
 * Compatibility with NTL 5.5.2.
 * 
 * Copyright (C) Victor Shoup
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <NTL/vector.h>
#include <NTL/tools.h>

#include "indzzrndstream.h"
//-----------------------------------------------------------------------------

/*
 * The following code is an adaptation of original code you can find
 * in NTL version 9.6.4 for the purpose of using custom random streams.
 * This implementation is protected by the GPL license.
 * 
 * This implementation is not thread safe.
 * 
 * Copyright (C) Victor Shoup
 */

#if NTL_MAJOR_VERSION < 9 || (NTL_MAJOR_VERSION == 9 && NTL_MINOR_VERSION < 4)

#include <cstring>

namespace NTL {
  NTL_vector_decl(byte, vec_byte)
  NTL_vector_impl(byte, vec_byte)
  
  void TerminalError(const char *s)
  {
    // Commented out due to lack of message callback in NTL 5.5.2
//     if (ErrorMsgCallback)
//         (*ErrorMsgCallback)(s);
//     else
        std::cerr << s << "\n";

    if (ErrorCallback) (*ErrorCallback)();
    abort();
  }
  
  inline void LogicError(const char *msg) { TerminalError(msg); }
  
  // SHA256 code adapted from an implementauin by Brad Conte.
  // The following is from his original source files.
  /*********************************************************************
  * Filename:   sha256.c
  * Author:     Brad Conte (brad AT bradconte.com)
  * Copyright:
  * Disclaimer: This code is presented "as is" without any guarantees.
  * Details:    Implementation of the SHA-256 hashing algorithm.
                SHA-256 is one of the three algorithms in the SHA2
                specification. The others, SHA-384 and SHA-512, are not
                offered in this implementation.
                Algorithm specification can be found here:
                * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
                This implementation uses little endian byte order.
  *********************************************************************/




  #define SHA256_BLOCKSIZE (64)
  #define SHA256_HASHSIZE  (32)

  // DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
  static inline
  void DBL_INT_ADD(_ntl_uint32& a, _ntl_uint32& b, _ntl_uint32 c)
  {
    _ntl_uint32 aa = INT32MASK(a);
    if (aa > INT32MASK(_ntl_uint32(0xffffffff) - c)) b++;
    a = aa + c;
  }

  #define ROTLEFT(a,b) (((a) << (b)) | (INT32MASK(a) >> (32-(b))))
  #define ROTRIGHT(a,b) ((INT32MASK(a) >> (b)) | ((a) << (32-(b))))

  #define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
  #define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
  #define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
  #define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
  #define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ (INT32MASK(x) >> 3))
  #define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ (INT32MASK(x) >> 10))

  struct SHA256_CTX {
    unsigned char data[64];
    _ntl_uint32 datalen;
    _ntl_uint32 bitlen[2];
    _ntl_uint32 state[8];
  };

  static const _ntl_uint32 sha256_const[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
  };


  static
  void sha256_transform(SHA256_CTX& ctx, unsigned char *data)
  {  
    _ntl_uint32 a,b,c,d,e,f,g,h,i,j,t1,t2,m[64];
        
    for (i=0,j=0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j+1] << 16) | (data[j+2] << 8) | (data[j+3]);
    for ( ; i < 64; ++i)
        m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

    a = ctx.state[0];
    b = ctx.state[1];
    c = ctx.state[2];
    d = ctx.state[3];
    e = ctx.state[4];
    f = ctx.state[5];
    g = ctx.state[6];
    h = ctx.state[7];
    
    for (i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e,f,g) + sha256_const[i] + m[i];
        t2 = EP0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    ctx.state[0] += a;
    ctx.state[1] += b;
    ctx.state[2] += c;
    ctx.state[3] += d;
    ctx.state[4] += e;
    ctx.state[5] += f;
    ctx.state[6] += g;
    ctx.state[7] += h;
  }  

  static
  void sha256_init(SHA256_CTX& ctx)
  {  
    ctx.datalen = 0; 
    ctx.bitlen[0] = 0; 
    ctx.bitlen[1] = 0; 
    ctx.state[0] = 0x6a09e667;
    ctx.state[1] = 0xbb67ae85;
    ctx.state[2] = 0x3c6ef372;
    ctx.state[3] = 0xa54ff53a;
    ctx.state[4] = 0x510e527f;
    ctx.state[5] = 0x9b05688c;
    ctx.state[6] = 0x1f83d9ab;
    ctx.state[7] = 0x5be0cd19;
  }

  static
  void sha256_update(SHA256_CTX& ctx, const unsigned char *data, _ntl_uint32 len)
  {  
    _ntl_uint32 i;
    
    for (i=0; i < len; ++i) { 
        ctx.data[ctx.datalen] = data[i]; 
        ctx.datalen++; 
        if (ctx.datalen == 64) { 
          sha256_transform(ctx,ctx.data);
          DBL_INT_ADD(ctx.bitlen[0],ctx.bitlen[1],512); 
          ctx.datalen = 0; 
        }  
    }  
  }  

  static
  void sha256_final(SHA256_CTX& ctx, unsigned char *hash, 
                    long hlen=SHA256_HASHSIZE)
  {  
    _ntl_uint32 i, j; 
    
    i = ctx.datalen; 
    
    // Pad whatever data is left in the buffer. 
    if (ctx.datalen < 56) { 
        ctx.data[i++] = 0x80; 
        while (i < 56) 
          ctx.data[i++] = 0x00; 
    }  
    else { 
        ctx.data[i++] = 0x80; 
        while (i < 64) 
          ctx.data[i++] = 0x00; 
        sha256_transform(ctx,ctx.data);
        memset(ctx.data,0,56); 
    }  
    
    // Append to the padding the total message's length in bits and transform. 
    DBL_INT_ADD(ctx.bitlen[0],ctx.bitlen[1],ctx.datalen * 8);

    ctx.data[63] = ctx.bitlen[0]; 
    ctx.data[62] = ctx.bitlen[0] >> 8; 
    ctx.data[61] = ctx.bitlen[0] >> 16; 
    ctx.data[60] = ctx.bitlen[0] >> 24; 
    ctx.data[59] = ctx.bitlen[1]; 
    ctx.data[58] = ctx.bitlen[1] >> 8; 
    ctx.data[57] = ctx.bitlen[1] >> 16;  
    ctx.data[56] = ctx.bitlen[1] >> 24; 
    sha256_transform(ctx,ctx.data);
    
    for (i = 0; i < 8; i++) {
        _ntl_uint32 w = ctx.state[i];
        for (j = 0; j < 4; j++) {
          if (hlen <= 0) break;
          hash[4*i + j] = w >> (24-j*8); 
          hlen--;
        }
    }

  }  



  static
  void sha256(const unsigned char *data, long dlen, unsigned char *hash, 
              long hlen=SHA256_HASHSIZE)
  {
    if (dlen < 0) dlen = 0;
    if (hlen < 0) hlen = 0;

    SHA256_CTX ctx;
    sha256_init(ctx);

    const long BLKSIZE = 4096;

    long i;
    for (i = 0; i <= dlen-BLKSIZE; i += BLKSIZE) 
        sha256_update(ctx, data + i, BLKSIZE);

    if (i < dlen)
        sha256_update(ctx, data + i, dlen - i);

    sha256_final(ctx, hash, hlen);
  }


  static
  void hmac_sha256(const unsigned char *key, long klen, 
                  const unsigned char *data, long dlen,
                  unsigned char *hash, long hlen=SHA256_HASHSIZE)
  {
    if (klen < 0) klen = 0;
    if (dlen < 0) dlen = 0;
    if (hlen < 0) hlen = 0;

    unsigned char K[SHA256_BLOCKSIZE];
    unsigned char tmp[SHA256_HASHSIZE];

    long i;

    if (klen <= SHA256_BLOCKSIZE) {
        for (i = 0; i < klen; i++)
          K[i] = key[i];
        for (i = klen; i < SHA256_BLOCKSIZE; i++) 
          K[i] = 0;
    }
    else {
        sha256(key, klen, K, SHA256_BLOCKSIZE); 
        for (i = SHA256_HASHSIZE; i < SHA256_BLOCKSIZE; i++)
          K[i] = 0;
    }

    for (i = 0; i < SHA256_BLOCKSIZE; i++)
        K[i] ^= 0x36;

    SHA256_CTX ctx;
    sha256_init(ctx);
    sha256_update(ctx, K, SHA256_BLOCKSIZE);
    sha256_update(ctx, data, dlen);
    sha256_final(ctx, tmp);

    for (i = 0; i < SHA256_BLOCKSIZE; i++)
        K[i] ^= (0x36 ^ 0x5C);

    sha256_init(ctx);
    sha256_update(ctx, K, SHA256_BLOCKSIZE);
    sha256_update(ctx, tmp, SHA256_HASHSIZE);
    sha256_final(ctx, hash, hlen);
  }


  // This key derivation uses HMAC with a zero key to derive
  // an intermediate key K from the data, and then uses HMAC
  // as a PRF in counter mode with key K to derive the final key

  
  void DeriveKey(unsigned char *key, long klen,  
               const unsigned char *data, long dlen)
  {
    if (dlen < 0) LogicError("DeriveKey: bad args");
    if (klen < 0) LogicError("DeriveKey: bad args");

    long i, j;


    unsigned char K[SHA256_HASHSIZE];
    hmac_sha256(0, 0, data, dlen, K); 

    // initialize 64-bit counter to zero
    unsigned char counter[8];
    for (j = 0; j < 8; j++) counter[j] = 0;

    for (i = 0; i <= klen-SHA256_HASHSIZE; i += SHA256_HASHSIZE) {
        hmac_sha256(K, SHA256_HASHSIZE, counter, 8, key+i); 

        // increment counter
        for (j = 0; j < 8; j++) {
          counter[j]++;
          if (counter[j] != 0) break; 
        }
    }

    if (i < klen) 
        hmac_sha256(K, SHA256_HASHSIZE, counter, 8, key+i, klen-i);
  }
  
  
  // ******************** ChaCha20 stuff ***********************

  static const _ntl_uint32 chacha_const[4] = 
    { 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574 };


  #define LE(p) (((_ntl_uint32)((p)[0])) + ((_ntl_uint32)((p)[1]) << 8) + \
      ((_ntl_uint32)((p)[2]) << 16) + ((_ntl_uint32)((p)[3]) << 24))

  #define FROMLE(p, x) (p)[0] = (x), (p)[1] = ((x) >> 8), \
    (p)[2] = ((x) >> 16), (p)[3] = ((x) >> 24)


  #define QUARTERROUND(x, a, b, c, d) \
      x[a] += x[b], x[d] = ROTLEFT(x[d] ^ x[a], 16), \
      x[c] += x[d], x[b] = ROTLEFT(x[b] ^ x[c], 12), \
      x[a] += x[b], x[d] = ROTLEFT(x[d] ^ x[a], 8), \
      x[c] += x[d], x[b] = ROTLEFT(x[b] ^ x[c], 7)


  static
  void salsa20_core(_ntl_uint32* data)
  {
    long i;

    for (i = 0; i < 10; i++) {
        QUARTERROUND(data, 0, 4, 8, 12);
        QUARTERROUND(data, 1, 5, 9, 13);
        QUARTERROUND(data, 2, 6, 10, 14);
        QUARTERROUND(data, 3, 7, 11, 15);
        QUARTERROUND(data, 0, 5, 10, 15);
        QUARTERROUND(data, 1, 6, 11, 12);
        QUARTERROUND(data, 2, 7, 8, 13);
        QUARTERROUND(data, 3, 4, 9, 14);
    }
  }


  // key K must be exactly 32 bytes
  static
  void salsa20_init(_ntl_uint32 *state, const unsigned char *K)  
  {
    long i;
    _ntl_uint32 j;

    for (i = 0; i < 4; i++)
        state[i] = chacha_const[i];

    for (i = 4; i < 12; i++)
        j = LE(K + 4*(i-4));
        state[i] = j;

    for (i = 12; i < 16; i++)
        state[i] = 0;
  }



  // state and data are of length 16
  static
  void salsa20_apply(_ntl_uint32 *state, _ntl_uint32 *data)
  {
    long i;

    for (i = 0; i < 16; i++) data[i] = state[i];

    salsa20_core(data);

    for (i = 0; i < 16; i++) data[i] += state[i];

    for (i = 12; i < 16; i++) {
        state[i]++;
        state[i] = INT32MASK(state[i]);
        if (state[i] != 0) break;
    }
  }


  #if 0
  // state is 16 words, data is 64 bytes
  static
  void salsa20_apply(_ntl_uint32 *state, unsigned char *data)
  {
    _ntl_uint32 wdata[16];
    salsa20_apply(state, wdata);

    long i;
    for (i = 0; i < 16; i++)
        FROMLE(data + 4*i, wdata[i]);

    // FIXME: could use memcpy for above if everything 
    // is right
  }
  #endif

  /*
   * RANDOMSTREAM FINALLY
   */
  RandomStream::RandomStream(const unsigned char *key)
  {
    salsa20_init(state, key);
    pos = 64;
  }

  void RandomStream::do_get(unsigned char *NTL_RESTRICT res, long n)
  {
    if (n < 0) LogicError("RandomStream::get: bad args");

    long i, j;

    if (n <= 64-pos) {
        for (i = 0; i < n; i++) res[i] = buf[pos+i];
        pos += n;
        return;
    }

    // read remainder of buffer
    for (i = 0; i < 64-pos; i++) res[i] = buf[pos+i];
    n -= 64-pos;
    res += 64-pos;
    pos = 64;

    _ntl_uint32 wdata[16];

    // read 64-byte chunks
    for (i = 0; i <= n-64; i += 64) {
        salsa20_apply(state, wdata);
        for (j = 0; j < 16; j++)
          FROMLE(res + i + 4*j, wdata[j]);
    }

    if (i < n) { 
        salsa20_apply(state, wdata);

        for (j = 0; j < 16; j++)
          FROMLE(buf + 4*j, wdata[j]);

        pos = n-i;
        for (j = 0; j < pos; j++)
          res[i+j] = buf[j];
    }
  }
}
#endif
//-----------------------------------------------------------------------------
