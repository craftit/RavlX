// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/testSpecificFMatrix.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/TFMatrixDecomposition.hh"

using namespace RavlN;

int testMatrix2d();
int testMatrix3d();
int testCholeskyDecomposition();
int testRQDecomposition();

int main() {
  int ln;
  if((ln = testMatrix2d()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testMatrix3d()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testCholeskyDecomposition()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testRQDecomposition()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  cout << "Test passed ok. \n";
  return 0;
}

int testMatrix2d() {
  Matrix2dC x(1,2,3,4);
  Matrix2dC y;
  if(!x.Invert(y)) {
    cerr << "Failed to invert matrix. \n";
    return __LINE__;
  }
  
  if(((y * x) - Matrix2dC(1,0,0,1)).SumOfSqr() > 0.0000001)
    return __LINE__;

  Matrix2dC z(3,5,5,2);
  Vector2dC v;
  EigenVectors(z,x,v);
  //cout << "x=" << x << " v=" << v << "\n";
  
  Matrix2dC md(v[0],0,0,v[1]);
  Matrix2dC m = x * md * x.Inverse();
  //  cerr << "m=" << m << "\n";
  if((m - z).SumOfSqr() > 0.0000001) return __LINE__;
  
  Matrix2dC gtMulTRes = x * z.T();
  Matrix2dC tsMulTRes;
  MulT(x,z,tsMulTRes);
  //cerr << "gtMulTRes=" << gtMulTRes <<"\n";
  //cerr << "tsMulTRes=" << tsMulTRes <<"\n";
  
  if((gtMulTRes - tsMulTRes).SumOfSqr() > 0.0000001) return __LINE__;
  
  return 0;
}

int testMatrix3d() {
  Matrix3dC x(1,7,3,4,5,3,7,8,9);
  Matrix3dC y;
  if(!x.Invert(y)) {
    cerr << "Failed to invert matrix. \n";
    return __LINE__;
  }
  
  if(((y * x) - Matrix3dC(1,0,0,0,1,0,0,0,1)).SumOfSqr() > 0.0000001)
    return __LINE__;  

  // Joel's test.
  
  Matrix3dC E(0, -8.15447e-14, -1.22998e-12,
              -3.47383, 1.35606, 11.4019,
              -1.35606, -11.8774, 0.999454);
  Matrix3dC Eu, Ev;
  Vector3dC Ed;
  
  Ed = SVD(E, Eu, Ev);
  
  if((E - (Eu*Matrix3dC(Ed[0],0,0,0,Ed[1],0,0,0,Ed[2])*Ev.T())).SumOfSqr() > 0.00001)
    return __LINE__;
  
  Vector3dC v3a(1,2,3),v3b(3,2,1);
  Vector3dC v = v3a + v3b;
  if(v[0] != 4 || v[1] != 4 || v[2] != 4) return __LINE__;
  return 0;
}

int testCholeskyDecomposition() {
  //cerr << "testCholeskyDecomposition(), Called. \n";
  for(int i = 0;i < 5;i++) {
    MatrixC rsx = RandomPositiveDefiniteMatrix(3);  
    TFMatrixC<RealT,3,3> rs = rsx;
    
    //cerr << "RS=" << rs << "\n";
    TFMatrixC<RealT,3,3> cd;
    
    if(!CholeskyDecomposition(rs,cd)) return __LINE__;
    //cerr << "l=" << cd <<"\n";
    TFMatrixC<RealT,3,3> nrs = cd * cd.T();
    //cerr << "nrs=" << nrs <<"\n";
    if((rs - nrs).SumOfSqr() > 0.000001) return __LINE__;
  }
  return 0;
}

int testRQDecomposition() {
  
  for(int i = 0;i < 5;i++) {
    MatrixC t = RandomMatrix(3,3,2.0);
    
    Matrix3dC A;
    for(int i = 0;i < 3;i++)
      for(int j = 0;j < 3;j++)
        A[i][j] = t[i][j];
    
    Matrix3dC R,Q;
    A.RQDecomposition(R,Q);
    Matrix3dC Ar = R * Q;
    
    //cerr << "R=" << R << "\nQ=" << Q << "\n";
    //cerr << "Ar=" << Ar << "\nA=" << A << "\n";
    if((A - Ar).SumOfSqr() > 0.000001) return __LINE__;
  }
  
  // Check a corner case:
  {
    Matrix3dC A(288,       0, -1000,
                360, 1066.67,     0,
                1,         0,     0);
    
    Matrix3dC R,Q;
    A.RQDecomposition(R,Q);
    Matrix3dC Ar = R * Q;
    
    //cerr << "R=" << R << "\nQ=" << Q << "\n";
    //cerr << "Ar=" << Ar << "\nA=" << A << "\n";
    if((A - Ar).SumOfSqr() > 0.000001) return __LINE__;
  }

  // Check a corner case:
  {
    Matrix3dC A(1000,       0,   288,
                0   , 1066.67,   360,
                0   ,       0,     1);
    
    Matrix3dC R,Q;
    A.RQDecomposition(R,Q);
    Matrix3dC Ar = R * Q;
    
    //cerr << "R=" << R << "\nQ=" << Q << "\n";
    //cerr << "Ar=" << Ar << "\nA=" << A << "\n";
    if((A - Ar).SumOfSqr() > 0.000001) return __LINE__;
  }
  
  return 0;
}
