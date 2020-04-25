// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/FitPProjection2d.cc"

#include "Ravl/PProjection2d.hh"
#include "Ravl/LeastSquares.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Matrix3d.hh"

namespace RavlN {
  
  //: Fit projection to a set of points.  4 or point correspondances are required
  
  bool FitPProjection(const SArray1dC<PPoint2dC> &from,const SArray1dC<PPoint2dC> &to,PProjection2dC &proj) {
    RavlAssert(from.Size() == to.Size());
    UIntT neq = from.Size();
    if(neq < 4) return false;
    
    Matrix3dC fromNorm;
    Matrix3dC toNorm;
    
    // Normalise 'from' points.
    
    SArray1dC<PPoint2dC> fromN;
    Normalise(from,fromN,fromNorm);
    
    // Normalise 'to' points.
    
    SArray1dC<PPoint2dC> toN;
    Normalise(to,toN,toNorm);
    
    // Build matrix.
    
    MatrixC A(neq * 2,9);
    IntT i = 0;
    for(SArray1dIter2C<PPoint2dC,PPoint2dC> it(toN,fromN);it;it++) {
      const PPoint2dC &x = it.Data2();
      
      SizeBufferAccessC<RealT> row1 = A[i++];
      
      RealT r = it.Data1()[0];
      RealT c = it.Data1()[1];
      RealT w = it.Data1()[2];
      
      row1[0] = 0;
      row1[1] = 0;
      row1[2] = 0;
      
      row1[3] = x[0] * -w;
      row1[4] = x[1] * -w;
      row1[5] = x[2] * -w;
      
      row1[6] = x[0] * c;
      row1[7] = x[1] * c;
      row1[8] = x[2] * c;
      
      SizeBufferAccessC<RealT> row2 = A[i++];
      
      row2[0] = x[0] * w;
      row2[1] = x[1] * w;
      row2[2] = x[2] * w;
      
      row2[3] = 0;
      row2[4] = 0;
      row2[5] = 0;
      
      row2[6] = x[0] * -r;
      row2[7] = x[1] * -r;
      row2[8] = x[2] * -r;
    }
    
    // Should check the rank of A?
    
    VectorC v;
    if(!LeastSquaresEq0Mag1(A,v))
      return false;
    //cerr << "A=" << A << " V=" << v << "\n";
    Matrix3dC mat(v[0],v[1],v[2],
		  v[3],v[4],v[5],
		  v[6],v[7],v[8]);
    
    toNorm.InvertIP();
    mat =  toNorm * mat * fromNorm;
    proj = PProjection2dC(mat);
    return true;
  }
  
}
