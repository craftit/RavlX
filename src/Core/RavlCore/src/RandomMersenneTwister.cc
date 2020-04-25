// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd.
// file-header-ends-here
///////////////////////////////////////////////////////////////////////////
//! rcsid="$Id: RandomMersenneTwister.cc 3073 2003-06-13 07:18:32Z craftit $"
//! file="Ravl/Core/Math/RandomMersenneTwister.cc"
//! lib=RavlCore
//! author="James Smith"
//! date="31/10/2002"
//! license=own

// This class is based on the standard Mersenne Twister 
// implemention my19937ar.c - copyright notice follows

// A C-program for MT19937, with initialization improved 2002/1/26.
// Coded by Takuji Nishimura and Makoto Matsumoto.
// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
// All rights reserved.

#include "Ravl/RandomMersenneTwister.hh"
#include "Ravl/String.hh"

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

namespace RavlN {

  RandomMersenneTwisterC::RandomMersenneTwisterC() 
    : mti(N+1)
  {}

  RandomMersenneTwisterC::RandomMersenneTwisterC(UInt32T seed) 
    : mti(N+1)
  {
    Seed(seed);
  }

  /* initializes mt[N] with a seed */
  void RandomMersenneTwisterC::Seed(UInt32T s) {
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
      mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
      /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
      /* In the previous versions, MSBs of the seed affect   */
      /* only MSBs of the array mt[].                        */
      /* 2002/01/09 modified by Makoto Matsumoto             */
      mt[mti] &= 0xffffffffUL;
      /* for >32 bit machines */
    }
  }
  
  /* initialize by an array with array-length */
  /* init_key is the array for initializing keys */
  /* key_length is its length */
  void RandomMersenneTwisterC::Seed(const UInt32T init_key[], UInt32T key_length) {
    unsigned int i, j, k;
    Seed(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
      mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + init_key[j] + j; /* non linear */
      mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
      i++; j++;
      if (i>=N) { mt[0] = mt[N-1]; i=1; }
      if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
      mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - i; /* non linear */
      mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
      i++;
      if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
  }

  //: Initialises the random number generator with the passed character array

  void RandomMersenneTwisterC::Seed(const char *str,UInt32T key_len) {
    // FIXME:- Move
    StringC randomSeed(str,key_len,key_len);
    unsigned pad = 4 - (randomSeed.Size() % sizeof(unsigned));
    for(unsigned k = 0;k < pad;k++)
      randomSeed += ' ';
    Seed(reinterpret_cast<const unsigned *>(randomSeed.chars()),randomSeed.Size()/sizeof(unsigned));
  }

  /* generates a random number on [0,0xffffffff]-interval */
  UInt32T RandomMersenneTwisterC::UInt(void)
  {
    UInt32T y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
      unsigned int kk;

      if (mti == N+1)   /* if init_genrand() has not been called, */
        Seed(5489UL); /* a default initial seed is used */

      for (kk=0;kk<N-M;kk++) {
        y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
        mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      for (;kk<N-1;kk++) {
        y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
        mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
      }
      y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
      mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

      mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
  }

  /* generates a random number on [0,0x7fffffff]-interval */
  Int32T RandomMersenneTwisterC::Int(void)
  {
    return (long)(UInt()>>1);
  }

  /* generates a random number on [0,1]-real-interval */
  double RandomMersenneTwisterC::Double(void)
  {
    return UInt()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
  }

  /* generates a random number on [0,1)-real-interval */
  double RandomMersenneTwisterC::Double2(void)
  {
    return UInt()*(1.0/4294967296.0); 
    /* divided by 2^32 */
  }

  /* generates a random number on (0,1)-real-interval */
  double RandomMersenneTwisterC::Double3(void)
  {
    return (((double)UInt()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
  }

  /* generates a random number on [0,1) with 53-bit resolution*/
  double RandomMersenneTwisterC::Double53(void) 
  { 
    unsigned long a=UInt()>>5, b=UInt()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
  } 

  /* These real versions are due to Isaku Wada, 2002/01/09 added */

}
