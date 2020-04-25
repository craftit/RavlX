// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_FITAFFINEPOINTS_HEADER
#define RAVLMATH_FITAFFINEPOINTS_HEADER 1
//! userlevel=Normal
//! author="Charles Galambos"
//! date="1/10/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="AffineFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitAffine2dPoints.hh"

#include "Ravl/FitToSample.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Body class for fitting a 2D affine projection to a sample of 2D points
  class FitAffine2dPointsBodyC
    : public FitToSampleBodyC
  {
  public:
    FitAffine2dPointsBodyC();
    //: Constructor for a class to fit a 2D affine projection to (x,y) points
    
    virtual StateVectorC FitModel(DListC<ObservationC> sample);
    //: Fit 2D affine projection to sample of 2D point observations
  };

  //! userlevel=Normal
  //! autoLink=on
  //: This class fits a 2D affine projection to a sample of 2D points
  class FitAffine2dPointsC
    : public FitToSampleC
  {
  public:
    FitAffine2dPointsC()
      : FitToSampleC(*new FitAffine2dPointsBodyC())
    {}
    //: Constructor for a class to fit a 2D affine projection to point pairs.

    FitAffine2dPointsC(const FitToSampleC &fitter)
      : FitToSampleC(dynamic_cast<const FitAffine2dPointsBodyC *>(BodyPtr(fitter)))
    {}
    //: Base class constructor.
    
    FitAffine2dPointsC(FitAffine2dPointsBodyC &bod)
      : FitToSampleC(bod)
    {}
    //: Body constructor.
    
    FitAffine2dPointsC(const FitAffine2dPointsBodyC *bod)
      : FitToSampleC(bod)
    {}
    //: Body constructor.
    
  public:
    
    FitAffine2dPointsBodyC &Body()
    { return static_cast<FitAffine2dPointsBodyC &>(FitToSampleC::Body()); }
    //: Access body.

    const FitAffine2dPointsBodyC &Body() const
    { return static_cast<const FitAffine2dPointsBodyC &>(FitToSampleC::Body()); }
    //: Access body.
  };
}


#endif
