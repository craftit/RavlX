// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/Math/Random.cc"

#include "Ravl/Random.hh"
#include "Ravl/RandomMersenneTwister.hh"
#include "Ravl/MTLocks.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  RandomMersenneTwisterC twister;

  //: Set the seed for the default random number generator.  
  void RandomSeedDefault(unsigned long seed) {
    MTWriteLockC lock(1);
    twister.Seed(seed);
  }
  
  // Returns an integer
  long int RandomInt() {
    MTWriteLockC lock(1);    
    return twister.Int();
  }
  
  // Returns a uniform random number between 0 and 1.
  double Random1(bool inclusive) {
    MTWriteLockC lock(1);
    if (inclusive) return twister.Double();
    else {
      RealT num;
      do {num = twister.Double();} while (num >= 1.0);
      return num;
    }
  }

}
