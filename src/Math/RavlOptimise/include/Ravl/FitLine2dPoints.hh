// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_FITLINE2DPOINTS_HEADER
#define RAVLMATH_FITLINE2DPOINTS_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="OrthogonalRegressionTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitLine2dPoints.hh"

#include "Ravl/FitToSample.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Body class for fitting a 2D line to a sample of 2D points
  class FitLine2dPointsBodyC
    : public FitToSampleBodyC
  {
  public:
    FitLine2dPointsBodyC();
    //: Constructor for a class to fit a 2D line to points on a plane
    
    FitLine2dPointsBodyC(RealT zh);
    //: Constructor for a class to fit a 2D line to points on a plane

    virtual StateVectorC FitModel(DListC<ObservationC> sample);
    //: Fit quadratic curve to sample of points on a plane

 private:
    RealT zh; // 3rd homogeneous coordinate of plane on which line lies
  };

  //! userlevel=Normal
  //! autoLink=on
  //: This class fits a 2D line to a sample of 2D points
  class FitLine2dPointsC
    : public FitToSampleC
  {
  public:
    FitLine2dPointsC(RealT zh)
      : FitToSampleC(*new FitLine2dPointsBodyC(zh))
    {}
    //: Constructor for a class to fit a 2D line to points on a plane
    // zh is the 3rd homogeneous coordinate of the plane on which the line and
    // the points lie.

    FitLine2dPointsC()
      : FitToSampleC(*new FitLine2dPointsBodyC())
    {}
    //: Constructor for a class to fit a 2D line to points on a plane
    // The 3rd homogeneous coordinate of the plane on which the line and points
    // lie is set to one.
    
    FitLine2dPointsC(const FitToSampleC &fitter)
      : FitToSampleC(dynamic_cast<const FitLine2dPointsBodyC *>(BodyPtr(fitter)))
    {}
    //: Base class constructor.
    
  public:
    FitLine2dPointsC(FitLine2dPointsBodyC &bod)
      : FitToSampleC(bod)
    {}
    //: Body constructor.
    
    FitLine2dPointsBodyC &Body()
    { return static_cast<FitLine2dPointsBodyC &>(FitToSampleC::Body()); }
    //: Access body.

    const FitLine2dPointsBodyC &Body() const
    { return static_cast<const FitLine2dPointsBodyC &>(FitToSampleC::Body()); }
    //: Access body.
  };
}


#endif
