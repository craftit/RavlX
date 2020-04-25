// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// file-header-ends-here
#ifndef RAVL_RANDOMMERSENNETWISTER_HEADER
#define RAVL_RANDOMMERSENNETWISTER_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! rcsid="$Id: RandomMersenneTwister.hh 5437 2006-04-05 11:09:32Z ees1wc $"
//! userlevel=Normal
//! file="Ravl/Core/Math/RandomMersenneTwister.hh"
//! lib=RavlCore
//! author="James Smith"
//! date="31/10/2002"
//! docentry="Ravl.API.Math.Random Numbers"
//! license=own

#include "Ravl/Types.hh"
#include "Ravl/config.h"

namespace RavlN {  
  

  //: Mersenne Twister Random number generator
  // http://www.math.keio.ac.jp/~matumoto/emt.html. 
  // This generator is not cryptographically secure. 
  // You need to use a one-way (or hash) function to obtain 
  // a secure random sequence.

  // This class is based on the standard Mersenne Twister 
  // implemention my19937ar.c - copyright notice follows

  // <PRE>
  // A C-program for MT19937, with initialisation improved 2002/1/26.
  // Coded by Takuji Nishimura and Makoto Matsumoto.
  // 
  // Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
  // All rights reserved.                          
  // </PRE>

  class RandomMersenneTwisterC {
  public:

    RandomMersenneTwisterC();
    //: Constructor

    RandomMersenneTwisterC(UInt32T seed);
    //: Constructor with seed

    void Seed(UInt32T seed);
    //: Initialises the random number generator with the passed seed

    void Seed(const UInt32T init_key[], UInt32T key_length);
    //: Initialises the random number generator with the passed array
    // This is for using seed values greater than can be stored in a UInt32T

    void Seed(const char *str,UInt32T key_len);
    //: Initialises the random number generator with the passed character array

    UInt32T UInt();
    //: Returns an unsigned integer uniformly distributed between 0 and RandomIntMaxValue

    Int32T Int();
    //: Returns a signed integer
    // The number is always positive.

    double Double();
    //: Returns a double uniformly distributed between 0 and 1

    double Double2();
    //: Alternative double generation function
    // Returns a double uniformly distributed between 0 and 1

    double Double3();
    //: Alternative double generation function
    // Returns a double uniformly distributed between 0 and 1

    double Double53();
    //: Returns a 53-bit precision double uniformly distributed between 0 and 1
    
  protected:

    UInt32T mt[624]; /* the array for the state vector  */
    UInt32T mti; /* mti==N+1 means mt[N] is not initialised */

  };

}

#endif
