// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/benchmarkSMatrix.cc"

#include "Ravl/SMatrix.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Option.hh"
#include "Ravl/OS/Date.hh"

#include "Ravl/TSMatrixFull.hh"
#include "Ravl/TSMatrixPartial.hh"
#include "Ravl/TSMatrixDiagonal.hh"
#include "Ravl/TSMatrixRightUpper.hh"
#include "Ravl/TSMatrixLeftLower.hh"
#include "Ravl/TSMatrixSparse.hh"
#include "Ravl/TSMatrixSymmetric.hh"
#include "Ravl/TSMatrixTranspose.hh"
#include "Ravl/TSMatrixScaledIdentity.hh"
#include "Ravl/Random.hh"

using namespace RavlN;

int testMatrix(MatrixC mat1,MatrixC mat2,int iters);
int testSMatrix(SMatrixC mat1,SMatrixC mat2,int iters);

const int noMatrixTypes = 7;

SMatrixC createMatrix(UIntT size,IntT type) {
  SMatrixC ret;
  bool filled = false;
  switch(type)
    {
    case 0:  ret = SMatrixC(RandomMatrix(size,size)); filled = true; break;
    case 1:  ret = TSMatrixDiagonalC<RealT>(size); break;
    case 2:  ret = TSMatrixRightUpperC<RealT>(size); break;
    case 3:  ret = TSMatrixLeftLowerC<RealT>(size); break;
    case 4:  ret = TSMatrixScaledIdentityC<RealT>(size,Random1()); filled=true; break;
    case 5:  ret = TSMatrixSymmetricC<RealT>(RandomSymmetricMatrix(size)); filled=true; break;
    case 6:  {
      SMatrixC ret(TSMatrixSparseC<RealT>(size,size));
      int n = (size * size)/5;
      //cerr << "Elements=" << n << "\n";
      for(int i = 0;i < n;i++)
	ret.Element(RandomInt()%size,RandomInt()%size,Random1());
      filled = true;
      return ret;
    }
    default:
      cerr << "Unknown matrix type=" << type << "\n";
      return SMatrixC();
    }
  if(!filled) {
    if(!ret.IsRowDirectAccess()) {
      cerr << "Matrix type " << type << " is not direct row access. \n";
      RavlAssert(0);
    }
    for(UIntT i = 0;i < size;i++) {
      for(Array1dIterC<RealT> it(ret.Row(i));it;it++)
	*it = Random1();
    }
  }
  return ret;
}

int testMatrix(MatrixC mat1,MatrixC mat2,int iters,int method) {
  MatrixC res(mat1.Rows(),mat2.Cols());
  res.Fill(0);
  switch(method) {
  case 0: // All
    for(int i = 0;i < iters;i++)
      res += mat1 * mat2;
    for(int i = 0;i < iters;i++)
      res += mat1.TMul(mat2);
    for(int i = 0;i < iters;i++)
      res += mat1.MulT(mat2);
    break;
  case 1: // Mul.
    for(int i = 0;i < iters;i++)
      res += mat1 * mat2;
    break;
  case 2:
    for(int i = 0;i < iters;i++)
      res += mat1.TMul(mat2);
    break;
  case 3:
    for(int i = 0;i < iters;i++)
      res += mat1.MulT(mat2);
    break;
  case 4:
    for(int i = 0;i < iters;i++)
      res += mat1 + mat2;
    break;
  }
  return 0;
}

int testSMatrix(SMatrixC smat1,SMatrixC smat2,int iters,int method) {
  SMatrixC sres;
  switch(method) {
  case 0: // All
    sres = smat1 * smat2;
    for(int i = 1;i < iters;i++)
      sres += smat1 * smat2;
    sres = smat1.TMul(smat2);
    for(int i = 1;i < iters;i++)
      sres += smat1.TMul(smat2);
    sres = smat1.MulT(smat2);
    for(int i = 1;i < iters;i++)
      sres += smat1.MulT(smat2);
    break;
  case 1: // Mul.
    sres = smat1 * smat2;
    for(int i = 1;i < iters;i++)
      sres += smat1 * smat2;
    break;
  case 2:
    sres = smat1.TMul(smat2);
    for(int i = 1;i < iters;i++)
      sres += smat1.TMul(smat2);
    break;
  case 3:
    sres = smat1.MulT(smat2);
    for(int i = 1;i < iters;i++)
      sres += smat1.MulT(smat2);
    break;
  case 4:
    sres = smat1 + smat2;
    for(int i = 1;i < iters;i++)
      sres += smat1 + smat2;
    break;
  }
  return 0;
}

RealT Evaluate(SMatrixC smat1,SMatrixC smat2,int iters,int method) {
  // Convert matrices.
  TMatrixC<RealT> tmat1 = smat1.TMatrix();
  TMatrixC<RealT> tmat2 = smat2.TMatrix();

  // Time plain matrices.
  DateC start(true);
  testMatrix(tmat1,tmat2,iters,method);
  RealT time1 = (DateC(true) - start).Double();
  //cerr << "Matrix=" << time1 << "\n";
  
  // Time SMatrices.
  start = DateC(true);
  testSMatrix(smat1,smat2,iters,method);
  RealT time2 = (DateC(true) - start).Double();
  //cerr << "SMatrix=" << time2 << "\n";
  return time2/time1;
}

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  IntT iters = opt.Int("i",2000,"Test iterations. ");
  IntT matSize = opt.Int("m",10,"Matrix size. ");
  IntT type1 = opt.Int("m1",0,"Matrix 1 type. 0=Full 1=Diagonal 2=RightUpper 3=LeftLower 4=ScaledIdentity 5=Symmetric 6=Sparse"); 
  IntT type2 = opt.Int("m2",0,"Matrix 2 type. "); 
  IntT method = opt.Int("op",0,"Operation. 0=All 1=Mul 2=TMul 3=MulT 4=Add");
  
  bool testAll = opt.Boolean("a",true,"Test all combinations of matrices.");
  
  opt.Check();
  cout <<"Iterations=" << iters << " Matrix Size=" << matSize << "\n";
  cout << " 0=Full 1=Diagonal 2=RightUpper 3=LeftLower 4=ScaledIdentity 5=Symmetric 6=Sparse. \n";
  
  cout.precision(2);
 

  //cout.setf(ios_base::fixed);
  cout.setf(ios::fixed);
  if(testAll) {
    RealT total = 0;
    int n = 0;
    for(int i = 0;i < noMatrixTypes;i++) {
      for(int j = 0;j < noMatrixTypes;j++) {
	SMatrixC mat1 = createMatrix(matSize,i);
	SMatrixC mat2 = createMatrix(matSize,j);
	
	RealT ratio = Evaluate(mat1,mat2,iters,method);
	n++;
	total += ratio;
	cout  << ratio << " " << flush;
      }
      cout << "\n";
    }
    cerr << "Average=" << (total/n) << "\n";
    return 0;
  }
  SMatrixC mat1 = createMatrix(matSize,type1);
  SMatrixC mat2 = createMatrix(matSize,type2);
  RealT ratio = Evaluate(mat1,mat2,iters,method);
  cout << "Ratio=" << ratio << "\n";
  
  return 0;
}


