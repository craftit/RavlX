// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitLine2dPoints.cc"

#include "Ravl/FitLine2dPoints.hh"
#include "Ravl/StateVectorLine2d.hh"
#include "Ravl/ObservationLine2dPoint.hh"

namespace RavlN {

  //: Constructor.
  FitLine2dPointsBodyC::FitLine2dPointsBodyC(RealT nzh)
  {
    zh = nzh;
  }

  //: Constructor.
  FitLine2dPointsBodyC::FitLine2dPointsBodyC()
  {
    zh = 1.0;
  }

  //: Fit parameters to sample of observations
  StateVectorC FitLine2dPointsBodyC::FitModel(DListC<ObservationC> sample)
  {
    // we need at least two points to fit a 2D line
    if ( sample.Size() < 2 )
      throw ExceptionC("Sample size too small in FitLine2dPointsBodyC::FitModel(). ");

    if ( sample.Size() == 2 ) {
      // initialise line lx*x + ly*y + lz*zh by fitting to two points (x1,y1)
      // and (x2,y2) which gives us
      //
      //     lx = zh*(y2-y1), ly = zh*(x1-x2), lz = x2*y1-x1*y2

      ObservationLine2dPointC obs;
      RealT x1, y1, x2, y2;

      obs = sample.First(); x1 = obs.GetZ()[0]; y1 = obs.GetZ()[1];
      obs = sample.Last();  x2 = obs.GetZ()[0]; y2 = obs.GetZ()[1];
      RealT lx = zh*(y2-y1);
      RealT ly = zh*(x1-x2);
      RealT lz = x2*y1-x1*y2;
      
      StateVectorLine2dC sv(lx, ly, lz, zh);
      return sv;
    }

    // compute solution for line parameters by orthogonal regression
    RealT Sx=0.0, Sy=0.0, Sxx=0.0, Sxy=0.0, Syy=0.0;
    UIntT n=0;

    for(DLIterC<ObservationC> it(sample);it;it++) {
      ObservationLine2dPointC obs = it.Data();
      VectorC xy = obs.GetZ();

      Sx += xy[0]; Sy += xy[1];
      Sxx += xy[0]*xy[0]; Sxy += xy[0]*xy[1]; Syy += xy[1]*xy[1];
      n++;
    }

    // compute sums normalized for centroid
    RealT nr = (RealT) n;
    RealT p = Sxx - Sx*Sx/nr;
    RealT q = Sxy - Sx*Sy/nr;
    RealT r = Syy - Sy*Sy/nr;

    // compute smallest non-zero eigenvalue of covariance
    // matrix (p q 0)
    //        (q r 0)
    //        (0 0 0)
    RealT lambda = 0.5*(p + r - sqrt((p-r)*(p-r) + 4.0*q*q));

    // compute line parameters lx*x + ly*y + lz = 0
    RealT lx = q;
    RealT ly = lambda - p;
    RealT lz = -(lx*Sx + ly*Sy)/nr;

    // convert to lx*x + ly*y + lz*zh = 0
    lz /= zh;

    return StateVectorLine2dC(lx, ly, lz, zh);
  }
}
