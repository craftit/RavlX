// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLapack

#include "Ravl/Matrix.hh"
#include "Ravl/MatrixRUT.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Random.hh"
#include "Ravl/Option.hh"
#include "Ravl/SArray2dIter.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Matrix4d.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/MatrixDecomposition.hh"
#include "Ravl/Vector.hh"
#include "Ravl/LeastSquares.hh"

using namespace RavlN;

int testSVD();
int testEigen();
int testFastEigen();
int testMisc();
int testMatrixRUT();
int testDet();
int testLUDecomposition();
int testCholeskyDecomposition();
int testVector();
int testInverse();
int testATAandAAT();
int testSolve();
int testLSQFixedRank();

int main() {
  int ln;
#if 1
  if((ln = testSVD()) != 0) {
    cerr << "testSVD failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testMisc()) != 0) {
    cerr << "testMisc failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testMatrixRUT()) != 0) {
    cerr << "testMatrixRUT failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testEigen()) != 0) {
    cerr << "testEigen() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testCholeskyDecomposition()) != 0) {
    cerr << "testCholeskyDecomposition() failed. Line:" << ln << "\n";
    return 1;
  }  
  if((ln = testLUDecomposition()) != 0) {
    cerr << "testLUDecomposition() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testDet()) != 0) {
    cerr << "testDet() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testVector()) != 0) {
    cerr << "testVector() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testInverse()) != 0) {
    cerr << "testInverse() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testATAandAAT()) != 0) {
    cerr << "testInverse() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testSolve()) != 0) {
    cerr << "testSolve() failed. Line:" << ln << "\n";
    return 1;
  }
  if((ln = testLSQFixedRank()) != 0) {
    cerr << "testLSQFixedRank() failed. Line:" << ln << "\n";
    return 1;
  }
#endif
  cerr << "Test passed. \n";
  return 0;
}


int testSVD() {
  cerr << "tesSVD() Called \n";
  for(int i = 0;i < 10;i++) {
    VectorC D;
    MatrixC U, V;
    MatrixC Test;
    if(i < 5) {
      Test = RandomMatrix(10,10);
    } else {
      Test = RandomMatrix(5,4);
    }
    D=SVD(Test,U,V);
    //cerr << "D=" << D << "\n";
    //cerr << "U=" << U << "\n";
    //cerr << "V=" << V << "\n";
    MatrixC md(U.Cols(),D.Size());
    md.Fill(0);
    md.SetDiagonal(D);
    MatrixC m = U * md * V.T();
    if((m - Test).SumOfSqr() > 0.000001) return __LINE__;
  }
  
  // Joel's test.
  {
    MatrixC E(0, -8.15447e-14, -1.22998e-12,
	      -3.47383, 1.35606, 11.4019,
	      -1.35606, -11.8774, 0.999454);
    MatrixC Eu, Ev;
    VectorC Ed;
    Ed = SVD(E, Eu, Ev);
    if((E - (Eu*MatrixC(Ed[0],0,0,0,Ed[1],0,0,0,Ed[2])*Ev.T())).SumOfSqr() > 0.00001)
      return __LINE__;
  }

  // Another test.
  {
    MatrixC E(4, -3, -4,
             -3, 2, 3,
             -4, 3, 4);
    MatrixC Eu, Ev;
    VectorC Ed;
    Ed = SVD(E, Eu, Ev);
    if((E - (Eu*MatrixC(Ed[0],0,0,0,Ed[1],0,0,0,Ed[2])*Ev.T())).SumOfSqr() > 0.00001)
      return __LINE__;
  }
  
  return 0;
}

int testEigen() {
  cerr << "testEigen() Called \n";
  MatrixC test = RandomSymmetricMatrix(5);
  VectorMatrixC vm = EigenVectors(test);
  VectorC D = vm.Vector();
  MatrixC md(D.Size(),D.Size());
  MatrixC E = vm.Matrix();
  md.Fill(0);
  md.SetDiagonal(D);
  MatrixC m = E * md * E.Inverse();
  if((m - test).SumOfSqr() > 0.0000001) return __LINE__;

  // Check it works on a 1x1 matrix.
  
  test = RandomSymmetricMatrix(1);
  vm = EigenVectors(test);
  D = vm.Vector();
  md =MatrixC(D.Size(),D.Size());
  E = vm.Matrix();
  md.Fill(0);
  md.SetDiagonal(D);
  cerr << "E=" << E << "\n";
  cerr << "md=" << md << "\n";
  m = E * md * E.Inverse();
  if((m - test).SumOfSqr() > 0.0000001) return __LINE__;
  
  return 0;
}

int testFastEigen() {
  cerr << "testFastEigen() Called \n";
  MatrixC test = RandomSymmetricMatrix(5);
  VectorC D = FastEigenVectors(test);
  MatrixC md(D.Size(),D.Size());
  MatrixC E = test;
  md.Fill(0);
  md.SetDiagonal(D);
  MatrixC m = E * md * E.Inverse();
  if((m - test).SumOfSqr() > 0.0000001) return __LINE__;

  // Check it works on a 1x1 matrix.
  
  test = RandomSymmetricMatrix(1);
  D = FastEigenVectors(test);
  md = MatrixC(D.Size(),D.Size());
  E = test;
  md.Fill(0);
  md.SetDiagonal(D);
  cerr << "E=" << E << "\n";
  cerr << "md=" << md << "\n";
  m = E * md * E.Inverse();
  if((m - test).SumOfSqr() > 0.0000001) return __LINE__;
  
  return 0;
}

int testMisc() {
  // Check slicing and SetRow/SetColumn work ok.
  RealT array2[]={11.5,12.8,3,4,100,1000};
  MatrixC mat(3,2,array2);
  MatrixC mat2 = mat.Copy();
  VectorC wer1(mat.SliceRow(2));
  if(wer1.Size() != 2) return __LINE__;
  mat.SetRow(2,wer1); //this causes the following error**
  VectorC wer2(mat.SliceColumn(1));
  if(wer2.Size() != 3) return __LINE__;
  mat.SetColumn(1,wer2); //this causes the following error**
  if((mat - mat2).SumOfSqr() > 0.0000000001) return __LINE__;
  return 0;
}

int testMatrixRUT() {
  // test OuterProductRUT(vec).
  
  VectorC vec(1,2,3);
  MatrixRUTC m = OuterProductRUT(vec);
  //cerr << "m=" << m << "\n";
  m.MakeSymmetric();
  MatrixC om = vec.OuterProduct();
  //cerr << "sm=" << m << "\n";
  //cerr << "om=" << om << "\n";
  if((om - m).SumOfSqr() > 0.00000001) return __LINE__;
  
  // test AddOuterProduct(vec).
  m = MatrixRUTC(3);
  m.Fill(0);
  m.AddOuterProduct(vec);
  m.MakeSymmetric();
  if((om - m).SumOfSqr() > 0.00000001) return __LINE__;
  
  MatrixC t1(2,3);
  t1[0][0] = 1;
  t1[0][1] = 2;
  t1[0][2] = 3;
  t1[1][0] = 4;
  t1[1][1] = 5;
  t1[1][2] = 6;
  
  MatrixC t2(3,2);
  t2[0][0] = 1;
  t2[1][0] = 2;
  t2[2][0] = 3;
  t2[0][1] = 4;
  t2[1][1] = 5;
  t2[2][1] = 6;
  
  MatrixC t3 = t2 * t1;
  m.AddOuterProduct(VectorC(4,5,6));
  m.MakeSymmetric();
  if((m - t3).SumOfSqr() > 0.00000001) return __LINE__;
  //  cerr << "t3=" << t3<< "\n";
  return 0;
}

int testDet() {
  cerr << "testDet(), Called \n";
  RealT nd;
#if 1
  // Check determinant of a 2 by 2 matrix.
  MatrixC tm = RandomMatrix(2,2);
  nd = tm.Det();
  Matrix2dC m22;
  int i,j;
  for(i = 0;i < 2;i++)
    for(j = 0;j < 2;j++) 
      m22[i][j] = tm[i][j];
  cerr << "2 Det=" << nd << " Mat22=" << m22.Det() << "\n";
  if(Abs(m22.Det() - nd) > 0.000000001) return __LINE__;
  
  // Check determinant of a 3 by 3 matrix.
  tm = RandomMatrix(3,3);
  Matrix3dC m33;
  for(i = 0;i < 3;i++)
    for(j = 0;j < 3;j++)
      m33[i][j] = tm[i][j];
  nd = tm.Det();
  cerr << "3 Det=" << nd << " Mat33=" << m33.Det() << "\n";
  if(Abs(m33.Det() - nd) > 0.000000001) return __LINE__;
  
  // Check determinant of a 4 by 4 matrix.
  tm = RandomMatrix(4,4);
  nd = tm.Det();
  Matrix4dC m44;
  for(i = 0;i < 4;i++)
    for(j = 0;j < 4;j++)
      m44[i][j] = tm[i][j];
  cerr << "4 Det=" << nd << " Mat44=" << m44.Det() << "\n";
  if(Abs(m44.Det() - nd) > 0.000000001) return __LINE__;
#endif
  // Check determinant of a 5 by 5 matrix.
  MatrixRSC rs = RandomSymmetricMatrix(5,5);  
  nd = rs.Det();
  RealT od;
  rs.NearSingularInverse(od);
  if(Abs(nd - od) > 0.00000001) return __LINE__;
  cerr << "5 Det=" << nd << " od=" << od << "\n";
  
  return 0;
}

int testCholeskyDecomposition() {
  cerr << "testCholeskyDecomposition(), Called. \n";
  MatrixRSC rs = RandomPositiveDefiniteMatrix(4);  
  //cerr << "RS=" << rs << "\n";
  MatrixC l;
  if(!CholeskyDecomposition(rs,l)) return __LINE__;
  //cerr << "l=" << l <<"\n";
  MatrixC nrs = l * l.T();
  //cerr << "nrs=" << nrs <<"\n";
  if((rs - nrs).SumOfSqr() > 0.000001) return __LINE__;
  return 0;
}

int testLUDecomposition() {
  cerr << "testLUDecomposition(), Called. \n";
  int n = 4;
  MatrixC mat1(n,n);
  MatrixC mat2(n,n);
  MatrixC org,res;
  RealT d;
#if 0
  // Need to change this to unscamble row re-ordering.
  MatrixC mat = RandomMatrix(n,n);
  org = mat.Copy();
  cerr << "Org=" << org << "\n";
  SArray1dC<IntT> order = LUDecomposition(mat,d);
  cerr << "d=" << d << "\n";
  mat1.Fill(0);
  mat2.Fill(0);
  for(int i = 0;i < n;i++) {
    mat1[i][i] = 1;
    for(int j = 0;j < n;j++) {
      if(j < i)
	mat1[i][j] = mat[i][j];
      else 
	mat2[i][j] = mat[i][j];
    }
  }
  //cerr << "mat1=" << mat1 << "\n";
  //cerr << "mat2=" << mat2 << "\n";  
  res = mat1 * mat2;
  cerr << "Res=" << res << "\n";
  cerr << "Org=" << org << "\n";
  cerr << "Order=" << order << " Diff=" << (res - org).SumOfSqr() << "\n";
  if((res - org).SumOfSqr() > 0.00001)
    return __LINE__;
#endif  
#if 1
  MatrixRSC rs = RandomPositiveDefiniteMatrix(n);  
  org = rs.Copy();
  //cerr << "Org=" << org << "\n";
  if(!LUDecompositionPD(rs,d))
    return __LINE__;
  //cerr << "RS=" << rs << "\n";
  //cerr << "Det=" << d << "\n";
  mat1 = rs;
  mat2 = rs.T();
  //cerr << "mat1=" << mat1 << "\n";
  //cerr << "mat2=" << mat2 << "\n";  
  res = mat1 * mat2;
  //cerr << "Res=" << res << "\n";
  if((res - org).SumOfSqr() > 0.00001)
    return __LINE__;  
#endif

  return 0;
}

int testVector() {
  VectorC vec2(1,2);
  if(vec2[0] != 1) return __LINE__;
  if(vec2[1] != 2) return __LINE__;
  VectorC vec3(1,2,3);
  if(vec3[0] != 1) return __LINE__;
  if(vec3[1] != 2) return __LINE__;
  if(vec3[2] != 3) return __LINE__;
  VectorC vec4(1,2,3,4);
  if(vec4[0] != 1) return __LINE__;
  if(vec4[1] != 2) return __LINE__;
  if(vec4[2] != 3) return __LINE__;
  if(vec4[3] != 4) return __LINE__;
  
  // Check creation from a fixed size vector.
  FVectorC<3> fv;
  fv[0] = 1;
  fv[1] = 2;
  fv[2] = 3;
  VectorC vec3a(fv);
  if(vec3a[0] != 1) return __LINE__;
  if(vec3a[1] != 2) return __LINE__;
  if(vec3a[2] != 3) return __LINE__;
 
  VectorC vec3b(3,2,1);
  RealT dist = vec3.CityBlockDistance(vec3b);
  //cerr << "DistCB=" << dist << "\n";
  if(Abs(dist - 4) > 0.000000001) return __LINE__;
  dist = vec3.MaxValueDistance(vec3b);
  //cerr << "DistMax=" << dist << "\n";
  if(Abs(dist - 2) > 0.000000001) return __LINE__;
  dist = vec3.EuclidDistance(vec3b);
  //cerr << "Dist=" << dist << "\n";
  if(Abs(dist - 2.82843) > 0.0001) return __LINE__;
  
  // Check evil conversions between fixed and general matricies and vectors work...
  
  MatrixC wtoi(2,3);
  wtoi.Fill(0);
  int i = 1;
  for(SArray2dIterC<RealT> it(wtoi);it;it++)
    *it = i++;
#if !RAVL_COMPILER_MIPSPRO
  // This MIPS pro compiler doesn't seem to pickup on the conversion
  // between a TVectorC<RealT> and a FVectorC<N>.  Should put a bug report in....
  
  FVectorC<3> worldpoint;  
  worldpoint[0] = 1;
  worldpoint[1] = 2;
  worldpoint[2] = 3;
  FVectorC<2> pixelvalue(wtoi * worldpoint);
  // cerr << "Result=" << pixelvalue << "\n";
  if(pixelvalue[0] != 14) return __LINE__;
  if(pixelvalue[1] != 32) return __LINE__;
#endif
  return 0;
}

int testInverse() {
  MatrixC mat(1,1);
  mat[0][0] = 2;
  MatrixC res = mat.Inverse();
  if(Abs(res[0][0] - 0.5) > 0.000000001)
    return __LINE__;
  return 0;
}

int testATAandAAT() {
  cerr << "testATAandAAT() Called \n";
  MatrixC mat = RandomMatrix(100,100);
  if(MatrixC(mat.ATA() - mat.TMul(mat)).SumOfSqr() > 0.00001)
    return __LINE__;
  if(MatrixC(mat.AAT() - mat.MulT(mat)).SumOfSqr() > 0.00001)
    return __LINE__;
  cerr << "testATAandAAT() Done. \n";
  return 0;
}

int testSolve() {
  cerr << "testSolve() Called. \n";
  // Check normal.
  MatrixC a = RandomMatrix(10,10);
  MatrixC b = RandomMatrix(10,2);
  MatrixC x = Solve(a,b);
  if(x.Rows() == 0) return __LINE__;
  //cerr << "x=" << x << "\n";
  MatrixC c = a * x;
  cerr << "c=" << c << "\n";
  cerr << "b=" << b << "\n";
  if(MatrixC(c - b).SumOfSqr() > 0.0001)
    return __LINE__;
  
  // Check positive definite.
  a = RandomPositiveDefiniteMatrix(10);
  b = RandomMatrix(10,2);
  x = SolvePD(a,b);
  if(x.Rows() == 0) return __LINE__;
  //cerr << "x=" << x << "\n";
  c = a * x;
  //cerr << "c=" << c << "\n";
  //cerr << "b=" << b << "\n";
  if(MatrixC(c - b).SumOfSqr() > 0.0001)
    return __LINE__;
  
  return 0;
}

int testLSQFixedRank() {
  cerr << "testLSQFixedRank(), Called. \n";
  VectorC b(3);
  b[0] = 1;
  b[1] = 2;
  b[2] = 3;
  MatrixC A(3,2);
  A[0][0] = 0.1; A[0][1] = 0.2;
  A[1][0] = 0.3; A[1][1] = 0.4;
  A[2][0] = 0.5; A[2][1] = 0.6;
  
  VectorC res = LeastSquaresFixedRank(A,b,1);
  if(res.Size() != 2) return __LINE__;
  if(Abs(res[0] - 2.43172) > 0.0001) return __LINE__;
  if(Abs(res[1] - 3.0803) > 0.0001) return __LINE__;
  res = LeastSquaresFixedRank(A,b,2);
  //cerr << "Result=" << res << "\n";
  if(res.Size() != 2) return __LINE__;
  if(Abs(res[0]) > 0.000001) return __LINE__;
  if(Abs(res[1] - 5) > 0.0001) return __LINE__;
  
  return 0;
}
