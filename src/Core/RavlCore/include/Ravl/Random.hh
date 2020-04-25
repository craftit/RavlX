// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RANDOM_HEADER
#define RAVL_RANDOM_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Normal
//! file="Ravl/Core/Math/Random.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Random Numbers"

#include "Ravl/Types.hh"

namespace RavlN {  
  
  const unsigned long RandomUIntMaxValue = 0xffffffff;
  //: Maximum value of random unsigned int

  const unsigned long RandomIntMaxValue = 0x7fffffff;
  //: Maximum value of random int

  long RandomInt();
  //: Returns an integer between 0 and RandomIntMaxValue.
  // Uses RandomMersenneTwisterC
  
  double Random1(bool inclusive=true);
  //: Returns a random number in the range [0 ... 1] with a uniform distribution.
  // Note that it <i>does</i> occasionally return the value 1.0.  However, if
  // "inclusive" = false, the range is limited to [0 ... 1); i.e. the value
  // 1.0 is specifically excluded.<br>
  // Uses RandomMersenneTwisterC.<br>

  float  RandomGauss();
  //: Returns a Gaussian distributed zero mean random
  //: number with variance 1.0.
  // RandomGauss is based on Random1.
  
  void RandomSeedDefault(unsigned long seed);
  //: Set the seed for the default random number generator.
  // Uses RandomMersenneTwisterC
}

#endif
