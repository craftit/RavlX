// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorHomog2d.cc"

#include "Ravl/StateVectorHomog2d.hh"

namespace RavlN {

  //: static method to build state vector from homography matrix
  VectorC StateVectorHomog2dBodyC::StateVecFromHomog(const Matrix3dC &P)
  {
    VectorC x(9);

    x[0] = P[0][0]; x[1] = P[0][1]; x[2] = P[0][2];
    x[3] = P[1][0]; x[4] = P[1][1]; x[5] = P[1][2];
    x[6] = P[2][0]; x[7] = P[2][1]; x[8] = P[2][2];
    return x;
  }

  //: Constructor
  StateVectorHomog2dBodyC::StateVectorHomog2dBodyC(const VectorC &nx,
						   RealT nzh1, RealT nzh2,
						   RealT ngaugeWeight)
    : StateVectorBodyC(nx)
  {
    zh1 = nzh1;
    zh2 = nzh2;
    gaugeWeight = ngaugeWeight;
    Postprocess();
  }

  //: Constructor
  StateVectorHomog2dBodyC::StateVectorHomog2dBodyC(const VectorC &nx,
						   RealT nzh1, RealT nzh2,
						   RealT ngaugeWeight,
						   const VectorC &nxstep)
    : StateVectorBodyC(nx,nxstep)
  {
    zh1 = nzh1;
    zh2 = nzh2;
    gaugeWeight = ngaugeWeight;
    Postprocess();
  }

  //: Constructor
  StateVectorHomog2dBodyC::StateVectorHomog2dBodyC(const Matrix3dC &P,
						   RealT nzh1, RealT nzh2,
						   RealT ngaugeWeight)
    : StateVectorBodyC(StateVecFromHomog(P))
  {
    zh1 = nzh1;
    zh2 = nzh2;
    gaugeWeight = ngaugeWeight;
    Postprocess();
  }

  //: Constructor
  StateVectorHomog2dBodyC::StateVectorHomog2dBodyC(const Matrix3dC &P,
						   RealT nzh1, RealT nzh2,
						   RealT ngaugeWeight,
						   const VectorC &nxstep)
    : StateVectorBodyC(StateVecFromHomog(P),nxstep)
  {
    zh1 = nzh1;
    zh2 = nzh2;
    gaugeWeight = ngaugeWeight;
    Postprocess();
  }

  //: Constructor.
  StateVectorHomog2dBodyC::StateVectorHomog2dBodyC(const VectorC &nx,
						   RealT nzh1, RealT nzh2,
						   RealT ngaugeWeight,
						   const VectorC &nxstep,
						   const Matrix3dC &Pnew) 
    : StateVectorBodyC(nx,nxstep),
      P(Pnew)
  {
    zh1 = nzh1;
    zh2 = nzh2;
    gaugeWeight = ngaugeWeight;
    Postprocess();
  }
  
  //: Increment the linear system
  // This assumes that the homography is normalised to unit Frobenius norm,
  // so there is nothing to add to vector a.
  bool StateVectorHomog2dBodyC::IncrementLS(MatrixRSC &A, VectorC &a)
  {
    MatrixC xxT = x.OuterProduct();
    MatrixRSC xxTS(xxT);
    
    xxTS *= gaugeWeight;
    A += xxTS;
    return true;
  }

  //: Postprocess 2D homography state vector after an adjustment to the
  //: parameters.
  bool StateVectorHomog2dBodyC::Postprocess()
  {
    // normalise to unit Frobenius norm
    x /= sqrt(x.SumOfSqr());

    // construct new 2D homography matrix
    P = Matrix3dC(x[0], x[1], x[2],
		  x[3], x[4], x[5],
		  x[6], x[7], x[8]);

    return true;
  }

}
