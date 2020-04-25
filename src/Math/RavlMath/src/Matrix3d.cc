// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/Matrix3d.cc"

#include "Ravl/Matrix3d.hh"

namespace RavlN {

  //: Invert this matrix.
  
  bool Matrix3dC::Invert(TFMatrixC<RealT,3,3> &op) const {
    RealT d1 = data[1][1]*data[2][2]-data[1][2]*data[2][1];
    RealT d2 = data[1][2]*data[2][0]-data[1][0]*data[2][2];
    RealT d3 = data[1][0]*data[2][1]-data[1][1]*data[2][0];
    RealT det = data[0][0]*d1 + data[0][1]*d2 + data[0][2]*d3;
    
    if(det == 0.0)
      return false; // Matrix singular.
    
    op[0][0] = d1/det; 
    op[0][1] = (data[2][1]*data[0][2]-data[2][2]*data[0][1])/det;
    op[0][2] = (data[0][1]*data[1][2]-data[0][2]*data[1][1])/det;
    op[1][0] = d2/det; 
    op[1][1] = (data[2][2]*data[0][0]-data[2][0]*data[0][2])/det;
    op[1][2] = (data[0][2]*data[1][0]-data[0][0]*data[1][2])/det;
    op[2][0] = d3/det; 
    op[2][1] = (data[2][0]*data[0][1]-data[2][1]*data[0][0])/det;
    op[2][2] = (data[0][0]*data[1][1]-data[0][1]*data[1][0])/det;
    
    return true;
  }
  
  
  // The following code is Copyright (C) 2007, OmniPerception Ltd.
  
  static RealT g_smallValue = 1e-12;
  static inline bool SolveSinCos(RealT v1,RealT v2,RealT &s,RealT &c) {
    if(IsNan(v1) || IsNan(v2))
      return false;
    if(Abs(v1) < g_smallValue) {
      s = 0;
      c = 1.0;
      return true;
    }
    RealT theta = ATan(v1 / -v2);
    c = Cos(theta);
    s = Sin(theta);
    return true;
  }
  // Solve,  v1 * c + v2 * s = 0  where Sqr(c) + Sqr(s) = 1.
  // The equations work out so that obtaining theta is trivial
  
  static inline void RotateX(const TFMatrixC<RealT,3,3> &b,RealT c,RealT s,TFMatrixC<RealT,3,3> &a) {
    a[0][0] = b[0][0];
    a[0][1] = b[0][1] * c + b[0][2] * s;
    a[0][2] = b[0][2] * c - b[0][1] * s;
    
    a[1][0] = b[1][0];
    a[1][1] = b[1][1] * c + b[1][2] * s;
    a[1][2] = b[1][2] * c - b[1][1] * s;
    
    a[2][0] = b[2][0];
    a[2][1] = b[2][1] * c + b[2][2] * s;
    a[2][2] = b[2][2] * c - b[2][1] * s;
  }
  // a = b * RotX(c,s)
  
  static inline void RotateY(const TFMatrixC<RealT,3,3> &b,RealT c,RealT s,TFMatrixC<RealT,3,3> &a) {
    a[0][0] = b[0][0] * c - b[0][2] * s;
    a[0][1] = b[0][1];
    a[0][2] = b[0][0] * s + b[0][2] * c;
    
    a[1][0] = b[1][0] * c - b[1][2] * s;
    a[1][1] = b[1][1];
    a[1][2] = b[1][0] * s + b[1][2] * c;
    
    a[2][0] = b[2][0] * c - b[2][2] * s;
    a[2][1] = b[2][1];
    a[2][2] = b[2][0] * s + b[2][2] * c;
  }
  // a = b * RotY(c,s)
  
  static inline void RotateZ(const TFMatrixC<RealT,3,3> &b,RealT c,RealT s,TFMatrixC<RealT,3,3> &a) {
    a[0][0] = b[0][0] * c + b[0][1] * s;
    a[0][1] = b[0][1] * c - b[0][0] * s;
    a[0][2] = b[0][2];
    
    a[1][0] = b[1][0] * c + b[1][1] * s;
    a[1][1] = b[1][1] * c - b[1][0] * s;
    a[1][2] = b[1][2];
    
    a[2][0] = b[2][0] * c + b[2][1] * s;
    a[2][1] = b[2][1] * c - b[2][0] * s;
    a[2][2] = b[2][2];
  }
  // a = b * RotZ(c,s)
  
  
  bool Matrix3dC::RQDecomposition(TFMatrixC<RealT,3,3> &R,TFMatrixC<RealT,3,3> &Q) const {
    const Matrix3dC &M = *this;
    const RealT verySmall = M.SumOfAbs() * 1e-12;
    
    // Check if matrix is already triangular.
    if(Abs(M[1][0]) < verySmall && Abs(M[2][0]) < verySmall && Abs(M[2][1])  < verySmall) {
      Q = Matrix3dC(1,0,0,
                    0,1,0,
                    0,0,1);
      R = M;
      return true;
    }
    
    // m21*c+m22*s == 0 to make A21 zero.
    RealT cx,sx;    
    if(!SolveSinCos(M[2][1],M[2][2],sx,cx))
      return false;
    RotateX(M,cx,sx,R);
    
    // m20*c-m22*s == 0 to make A20 zero
    RealT cy,sy;
    if(!SolveSinCos(R[2][0],-R[2][2],sy,cy))
      return false;
    Matrix3dC Ro;
    RotateY(R,cy,sy,Ro);
    
    // m10*c+m11*s == 0 to make A10 zero
    RealT cz,sz;
    if(!SolveSinCos(Ro[1][0],Ro[1][1],sz,cz))
      return false;
    RotateZ(Ro,cz,sz,R);
    
    // Q = Qz.T() * Qy.T() * Qx.T()
    Q = Matrix3dC(cz*cy  ,sz*cx+cz*sy*sx ,sz*sx-cz*sy*cx,
                  -sz*cy ,cz*cx-sz*sy*sx ,cz*sx+sz*sy*cx,
                  sy     ,-cy*sx         ,cy*cx);
    
    // Tidy up rounding errors.
    R[2][1] = 0.0;
    R[2][0] = 0.0;
    R[1][0] = 0.0;
    
    return true;
  }
  
  //: Decompose this matrix such that *this = R * Q
  //: This method generates the sin of the angles of each rotation.
  
  // See Multiple View Geometry in Computer Vision, Second Edition. R.Hartley & A.Zisserman. Pg 579
  
  bool Matrix3dC::RQDecomposition(TFMatrixC<RealT,3,3> &R,RealT &rx,RealT &ry,RealT &rz) const {
    const Matrix3dC &M = *this;
    RealT verySmall = M.SumOfAbs() * 1e-12;
    
    // Check if matrix is already triangular.
    if(Abs(M[1][0]) < verySmall && Abs(M[2][0]) < verySmall && Abs(M[2][1])  < verySmall) {
      rx = 0;
      ry = 0;
      rz = 0;
      R = M;
      return true;
    }
    
    // m21*c+m22*s == 0 to make A21 zero.
    RealT cx,sx;    
    if(!SolveSinCos(M[2][1],M[2][2],sx,cx))
      return false;
    RotateX(M,cx,sx,R);
    rx = ATan2(sx,cx);
    
    // m20*c-m22*s == 0 to make A20 zero
    RealT cy,sy;
    if(!SolveSinCos(R[2][0],-R[2][2],sy,cy))
      return false;
    Matrix3dC Ro;
    RotateY(R,cy,sy,Ro);
    ry = ATan2(sy,cy);
    
    // m10*c+m11*s == 0 to make A10 zero
    RealT cz,sz;
    if(!SolveSinCos(Ro[1][0],Ro[1][1],sz,cz))
      return false;
    rz = ATan2(sz,cz);
    
    // Tidy up rounding errors.
    R[2][1] = 0.0;
    R[2][0] = 0.0;
    R[1][0] = 0.0;
    
    return true;
  }
  
  
  //: Compose a rotation matrix for rotations around the x,y and z axis and assign it to 'matrix'
  //: Angles are in radians.
  
  Matrix3dC Matrix3dC::ComposeRotation(RealT rx,RealT ry,RealT rz) {
    Matrix3dC tmp1(1,0,0,
                   0,1,0,
                   0,0,1);
    Matrix3dC tmp2;   
    // TODO :- Merge all this into one calculation.
    RotateX(tmp1,Cos(rx),Sin(rx),tmp2); 
    RotateY(tmp2,Cos(ry),Sin(ry),tmp1);
    RotateZ(tmp1,Cos(rz),Sin(rz),tmp2);
    //Remember that the composition of a rotation matrix works as:
    //(Rx*Ry*Rz)^T = Rz^T*Ry^T*Rx^T
    return tmp2.T();
  }

}
