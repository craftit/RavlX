// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RANDOMGAUSS_HEADER
#define RAVL_RANDOMGAUSS_HEADER 1
///////////////////////////////////////////////////////////////////////////
//! userlevel=Normal
//! file="Ravl/Core/Math/RandomGauss.hh"
//! lib=RavlCore
//! author="Andrew Stoddart"
//! date="26/04/1995"
//! docentry="Ravl.API.Math.Random Numbers"

#include "Ravl/Random.hh"
#include "Ravl/RandomMersenneTwister.hh"

namespace RavlN {  
  
  //! userlevel=Normal
  //: Generate random number with a Gaussian distribution.
  // The distribution has zero mean and variance = 1.0.
  
  class RandomGaussC {
  public:
    RandomGaussC();
    //: Default.

    RandomGaussC(UIntT seed);
    //: Construct with seed.

    float Generate(RandomMersenneTwisterC& generator);
    //: Generate with external generator
    
    float Generate() {
      return Generate(twister);
    }
    //: Generate number with internal generator

    RandomMersenneTwisterC &MersenneTwister()
    { return twister; }
    //: Access the underlying random number generator.

  protected:
    RandomMersenneTwisterC twister;
    bool iset;
    float gset;
    float gasdev;
  };

};


#endif
