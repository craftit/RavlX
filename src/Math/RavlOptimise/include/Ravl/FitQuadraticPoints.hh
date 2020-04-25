// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_FITQUADRATICPOINTS_HEADER
#define RAVLMATH_FITQUADRATICPOINTS_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="QuadraticFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitQuadraticPoints.hh"

#include "Ravl/FitToSample.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Body class for fitting a quadratic curve to a sample of (x,y) points
  class FitQuadraticPointsBodyC
    : public FitToSampleBodyC
  {
  public:
    FitQuadraticPointsBodyC();
    //: Constructor for a class to fit a quadratic curve to points on a plane
    
    virtual StateVectorC FitModel(DListC<ObservationC> sample);
    //: Fit quadratic curve y = a*x^2 + b*x + c to (x,y) points
  };

  //! userlevel=Normal
  //! autoLink=on
  //: This class fits a quadratic curve to a sample of (x,y) points
  class FitQuadraticPointsC
    : public FitToSampleC
  {
  public:
    FitQuadraticPointsC()
      : FitToSampleC(*new FitQuadraticPointsBodyC())
    {}
    //: Constructor for a class to fit a quadratic curve to (x,y) points
    
    FitQuadraticPointsC(const FitToSampleC &fitter)
      : FitToSampleC(dynamic_cast<const FitQuadraticPointsBodyC *>(BodyPtr(fitter)))
    {}
    //: Base class constructor.
    
  protected:
    FitQuadraticPointsC(FitQuadraticPointsBodyC &bod)
      : FitToSampleC(bod)
    {}
    //: Body constructor.
    
    FitQuadraticPointsC(const FitQuadraticPointsBodyC *bod)
      : FitToSampleC(bod)
    {}
    //: Body constructor.
    
    FitQuadraticPointsBodyC &Body()
    { return static_cast<FitQuadraticPointsBodyC &>(FitToSampleC::Body()); }
    //: Access body.

    const FitQuadraticPointsBodyC &Body() const
    { return static_cast<const FitQuadraticPointsBodyC &>(FitToSampleC::Body()); }
    //: Access body.
  };
}


#endif
