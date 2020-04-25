// This file is part of RavlN,  
// Copyright (C) 2006, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THINPLATESPLINE_HEADER
#define RAVL_THINPLATESPLINE_HEADER 1
//! author="Charles Galambos"
//! date="29/1/2003"
//! rcsid="$Id$"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Sequences"

#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Array1d.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 1d Thin plate spline.
  
  class ThinPlateSpline1dC {
  public:
    ThinPlateSpline1dC(RealT nSigma = 1)
      : sigma(nSigma)
    {
      sigma2_2 = Sqr(sigma) * 2;
      log_sigma2 = Log(Sqr(sigma)); 
    }
    //: Constructor.

    ThinPlateSpline1dC(RealT nSigma,const Array1dC<RealT> &in,const Array1dC<RealT> &out)
      : sigma(nSigma)
    {
      sigma2_2 = Sqr(sigma) * 2;
      log_sigma2 = Log(Sqr(sigma)); 
      Design(in,out);
    }
    //: Constructor.
    
    bool Design(const Array1dC<RealT> &in,const Array1dC<RealT> &out);
    //: Design the thin plate spline.
    
    RealT Apply(const RealT &value) const;
    //: Warp 'value' 
    // Returns the matching value.
    
  protected:
    inline RealT U(RealT dist2) const { 
      if(dist2 == 0)
	return 0; // Avoid Log(0).
      return (dist2 * (Log(dist2) - log_sigma2))/ sigma2_2;
    }
    // Compute U(x).
    
    RealT sigma;
    RealT sigma2_2; // Sqr(sigma) * 2
    RealT log_sigma2; // Log(Sqr(sigma))
    
    //MatrixC wd;
    VectorC w;
    Array1dC<RealT> refPos;
  };
}


#endif
