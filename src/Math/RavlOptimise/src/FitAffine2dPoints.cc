// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitAffine2dPoints.cc"

#include "Ravl/FitAffine2dPoints.hh"
#include "Ravl/StateVectorAffine2d.hh"
#include "Ravl/ObservationAffine2dPoint.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/LeastSquares.hh"

namespace RavlN {

  //: Constructor.
  
  FitAffine2dPointsBodyC::FitAffine2dPointsBodyC()
  {}
  
  //: Fit parameters to sample of observations
  
  StateVectorC FitAffine2dPointsBodyC::FitModel(DListC<ObservationC> sample)
  {
    UIntT samples = sample.Size();
    if ( samples < 3 )
      throw ExceptionC("Sample size too small in FitAffine2dPointsBodyC::FitModel(). ");
    
    MatrixC A(samples*2,6);
    A.Fill(0.0);
    VectorC b(samples*2);
    UIntT i = 0;
    for(DLIterC<ObservationC> it(sample);it;it++,i++) {
      ObservationAffine2dPointC eobs(*it);
      RavlAssert(eobs.IsValid());
      RealT x1, y1, x2, y2;
      x1=eobs.GetZ1()[0]; y1=eobs.GetZ1()[1];
      x2=eobs.GetZ()[0];  y2=eobs.GetZ()[1];
      
      A[i*2][0] = x1; 
      A[i*2][1] = y1; 
      A[i*2][2] = 1;
      b[i*2] = x2;
      
      A[i*2+1][3] = x1; 
      A[i*2+1][4] = y1; 
      A[i*2+1][5] = 1;
      b[i*2+1] = y2;
    }
    if(A.Rows() == A.Cols()) {
      // solve for solution vector
      if(!SolveIP(A,b))
	throw ExceptionNumericalC("Dependent linear equations in FitAffine2dPointsBodyC::FitModel(DListC<ObservationC>). ");
    } else {
      RealT residual;
      if(!LeastSquaresQR_IP(A,b,residual))
	throw ExceptionNumericalC("Dependent linear equations in FitAffine2dPointsBodyC::FitModel(DListC<ObservationC>). ");
    }
    
    return StateVectorAffine2dC(b);
  }
}
