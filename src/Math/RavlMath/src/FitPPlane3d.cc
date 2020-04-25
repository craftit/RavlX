// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! lib=RavlMath

#include "Ravl/PProjection3d.hh"
#include "Ravl/PPoint3d.hh"
#include "Ravl/PPlane3d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/SArray1dIter2.hh"

#include "Ravl/LeastSquares.hh"
#include "Ravl/Matrix4d.hh"

namespace RavlN {
  
  //: Fit plane to a set of points.
 
  bool FitPPlane3d(const SArray1dC<PPoint3dC> &points,PPlane3dC &plane) {
    MatrixC A(points.Size(),4);
    int i = 0;
    for(SArray1dIterC<PPoint3dC> it(points);it;it++) {
      SizeBufferAccessC<RealT> row = A[i++];
      for(int k = 0;k < 4;k++)
	row[k] = (*it)[k];
    }
    
    VectorC v;
    if(!LeastSquaresEq0Mag1(A,v))
      return false;
    plane = PPlane3dC(v[0],v[1],v[2],v[3]);
    return true;
  }
  
  //: Fit a 3d projection such that  p2 = proj * p1
  
  bool FitPProjection(const SArray1dC<PPoint3dC> &from,const SArray1dC<PPoint3dC> &to,PProjection3dC &proj) {
    RavlAssert(from.Size() == to.Size());
    UIntT neq = from.Size();
    if(neq < 6) return false;
    
    Matrix4dC fromNorm;
    Matrix4dC toNorm;
    SArray1dC<PPoint3dC> fromN;
    SArray1dC<PPoint3dC> toN;
    
    // Normalise 'from' points.
    Normalise(from,fromN,fromNorm);
    
    // Normalise 'to' points.
    Normalise(to,toN,toNorm);
    
    // Build matrix.
    
    MatrixC A(neq * 3,16);
    IntT i = 0;
    for(SArray1dIter2C<PPoint3dC,PPoint3dC> it(toN,fromN);it;it++) {
      const PPoint3dC &x = it.Data2();
      
      RealT r = it.Data1()[0];
      RealT c = it.Data1()[1];
      RealT z = it.Data1()[2];
      RealT w = it.Data1()[3];
      
      {
        SizeBufferAccessC<RealT> row1 = A[i++];
        row1[0] = x[0] * w;
        row1[1] = x[1] * w;
        row1[2] = x[2] * w;
        row1[3] = x[3] * w;
        
        row1[4] = 0;
        row1[5] = 0;
        row1[6] = 0;
        row1[7] = 0;
        
        row1[8]  = 0;
        row1[9]  = 0;
        row1[10] = 0;
        row1[11] = 0;
        
        row1[12] = x[0] * -r;
        row1[13] = x[1] * -r;
        row1[14] = x[2] * -r;
        row1[15] = x[3] * -r;
      }
      
      {
        SizeBufferAccessC<RealT> row1 = A[i++];
        row1[0] = 0;
        row1[1] = 0;
        row1[2] = 0;
        row1[3] = 0;
        
        row1[4] = x[0] * w;
        row1[5] = x[1] * w;
        row1[6] = x[2] * w;
        row1[7] = x[3] * w;
        
        row1[8] = 0;
        row1[9] = 0;
        row1[10] = 0;
        row1[11] = 0;
        
        row1[12] = x[0] * -c;
        row1[13] = x[1] * -c;
        row1[14] = x[2] * -c;
        row1[15] = x[3] * -c;
      }
      
      {
        SizeBufferAccessC<RealT> row1 = A[i++];
        row1[0] = 0;
        row1[1] = 0;
        row1[2] = 0;
        row1[3] = 0;
        
        row1[4] = 0;
        row1[5] = 0;
        row1[6] = 0;
        row1[7] = 0;
        
        row1[8]  = x[0] * w;
        row1[9]  = x[1] * w;
        row1[10] = x[2] * w;
        row1[11] = x[3] * w;
        
        
        row1[12] = x[0] * -z;
        row1[13] = x[1] * -z;
        row1[14] = x[2] * -z;
        row1[15] = x[3] * -z;
      }
      
    }
    
    RavlAssert(i == (int) A.Rows());
    
    // Should check the rank of A?
    
    VectorC v;
    if(!LeastSquaresEq0Mag1(A,v))
      return false;
    
    //cerr << "Norm=" << toNorm << "\n";
    //cerr << "A=" << A << " V=" << v << "\n";
    
    Matrix4dC mat(v[ 0],v[ 1],v[ 2],v[ 3],
		  v[ 4],v[ 5],v[ 6],v[ 7],
		  v[ 8],v[ 9],v[10],v[11],
		  v[12],v[13],v[14],v[15]
                  );
    
    toNorm.InverseIP();
    mat =  toNorm * mat * fromNorm;
    proj = PProjection3dC(mat);
    return true;
  }
  
}
