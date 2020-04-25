// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! date="26.04.1995"
//! author="Andrew Stoddart"
//! file="Ravl/Core/Math/RandomGauss.cc"

#include "Ravl/StdMath.hh" // sqrt(), log()
#include "Ravl/RandomGauss.hh"
#include "Ravl/MTLocks.hh"

namespace RavlN {
  
  extern RandomMersenneTwisterC twister;
  // The default variable of generators.

  static RandomGaussC g_rg;
  
  //: Default.
  
  RandomGaussC::RandomGaussC() 
   : iset(false),
     gset(0),
     gasdev(0)
  {}
  
  //: Construct with seed.

  RandomGaussC::RandomGaussC(UIntT seed)
   : twister(seed),
     iset(false),
     gset(0),
     gasdev(0)
  {}

  //: Generate the next value.
  
  float RandomGaussC::Generate(RandomMersenneTwisterC& generator) {
    float v1,v2,fac,r;
    if (!iset) {
      do {
	v1=(float)(2.0 * generator.Double() - 1.0);
	v2=(float)(2.0 * generator.Double() - 1.0);
	r=v1*v1 + v2*v2;
      } while (r>=1.0);
      fac=(float)sqrt(-2.0 * log(r)/r);
      gset=v1 * fac;
      gasdev=v2 * fac;
      iset=true;
      return gasdev;
    }
    iset=false;
    return gset;
  }
  
  float  RandomGauss() {
    MTWriteLockC lock(1);
    return g_rg.Generate(twister);
  }
  
}
