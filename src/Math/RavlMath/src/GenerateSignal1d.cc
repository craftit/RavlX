// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Signals/1D/GenerateSignal1d.cc"

#include "Ravl/Math.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/GenerateSignal1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/RandomGauss.hh"
#include "Ravl/Assert.hh"

namespace RavlN {

  //: Generate a 1-d guassian curve.
  
  Array1dC<RealT> GenerateGauss1d(RealT sigma,RealT mean,RealT scale,RealT size) {
    IntT s = Round(mean - size * sigma);
    IntT e = Round(mean + size * sigma);
    RavlAssert(s <= e);
    Array1dC<RealT> ret(s,e);
    RealT x = ret.IMin();
    for(Array1dIterC<RealT> it(ret);it;it++,x++)
      *it =  Exp(-0.5 * Sqr((x - mean)/sigma)) * scale/(sigma * RavlConstN::sqrt2Pi);
    return ret;
  }
  
  Array1dC<RealT> GenerateRandomGauss1d(SizeT size,RealT stdDeviation,RealT mean) {
    Array1dC<RealT> ret(size);
    for (Array1dIterC<RealT> it(ret);it;it++)
      *it = stdDeviation * RandomGauss() + mean;
    return ret;
  }
  //: Generate a 1-d Gaussian curve.


}
