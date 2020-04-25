// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixLUDecomposition.cc"

#include "Ravl/MatrixDecomposition.hh"

#include "Ravl/Matrix.hh"
#include "Ravl/MatrixRS.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  static const RealT smallValue = 1e-8;
  
  //: LUDecomposition for positive definite matrices.
  
  bool LUDecompositionPD(MatrixC &mat,RealT &det) {
    det = 1;
    IntT n =  mat.Rows();
    if(n != (IntT) mat.Cols()) {
      cerr << "LUDecompositionPD(), ERROR: Matrix must be square. \n";
      return false;
    }
    SArray1dC<RealT> diag(n);
    int i;
    for(i=0;i < n;i++) {
      int k,j;
      RealT sum = mat[i][i];
      for(k = i-1;k >= 0;k--)
	sum -= Sqr(mat[i][k]);
      if(sum < 0.0) {
	cerr << "LUDecompositionPD(), WARNING: Matrix not positive definite. \n";
	return false;
      }
      diag[i] = Sqrt(sum);
      for(j = i+1;j < n;j++) {
	sum = mat[i][j];
	for(k = i-1;k >= 0;k--)
	  sum -= mat[i][k] * mat[j][k];
	mat[j][i] = sum/diag[i];
      }
    }
    for(i = 0;i < n;i++) {
      RealT v = diag[i];
      mat[i][i] = v;
      det *= v;
      for(int j = i+1;j < n;j++)
	mat[i][j] = 0;
    }
    return true;
  }
  
  //: LU Decomposition with partial pivoting.
  
  SArray1dC<IntT> LUDecomposition(MatrixC &mat,RealT &d) {
    UIntT n = mat.Rows();
    if(n != mat.Cols()) {
      cerr << "LUDecomposition(), ERROR: Matrix must be square. \n";
      return SArray1dC<IntT>();
    }
    SArray1dC<IntT> perm(n);
    RealT xmax;
    d = 1;
    IntT piv;
    UIntT i,j,k = 0;
    VectorC sv(n);
    for(SArray1dIterC<RealT> it(sv);it;it++,k++) {
      RealT t;
      xmax = 0.0;
      for(BufferAccessIterC<RealT> rit(mat[k],n);rit;rit++)
	if((t = Abs(*rit)) > xmax)
	  xmax = t;
      if(Abs(xmax) < smallValue) {
	cerr << "LUDecomposition(), WARNING: Singular matrix detected on row " << k << " \n";
	return SArray1dC<IntT>();
      }
      *it = 1/xmax;
    }
    for(j = 0;j < n;j++) {
      for(i = 1;i < j;i++) {
	RealT &z = mat[i][j];
	RealT x = z;
	BufferAccessIterC<RealT> row = mat[i];
	for(k = 0;k < i;k++,row++)
	  x -= *row * mat[k][j];
	z = x;
      }
      i = j;
      {
	RealT &z = mat[i][j];
	RealT x = z;
	BufferAccessIterC<RealT> row = mat[i];
	for(k = 0;k < j;k++,row++)
	  x -= *row * mat[k][j];
	z = x;
	xmax = Abs(x) * sv[i];
	piv = i;
      }
      for(i++;i < n;i++) {
	RealT &z = mat[i][j];
	RealT x = z;
	BufferAccessIterC<RealT> row = mat[i];
	for(k = 0;k < j;k++,row++)
	  x -= *row * mat[k][j];
	z = x;
	if((x = (Abs(x) * sv[i])) >= xmax) {
	  xmax = x;
	  piv = i;
	}
      }
      perm[j] = piv;
      if(piv != (IntT) j) {
	mat.SwapRows(j,piv); // Swap rows.
	d = -d;
	sv[piv] = sv[j];
      }
      RealT &jj = mat[j][j];
      if(Abs(jj) < smallValue)
	jj = smallValue;
      if(j != n) {
	RealT x = 1 / jj;
	for(i = j+1;i < n;i++)
	  mat[i][j] *= x;
      }
      d *= jj; // Compute the determinant.
    }
    return perm;
  }

  
  void LUBackSubstitute(const MatrixC &lu,SArray1dC<RealT> &b) {
    RealT sum;
    IntT i,n = lu.Rows();
    for(i = 0;i < n;i++) {
      sum = b[i];
      for(int k = i-1;k >= 0;k--)
	sum -= lu[i][k] * b[k];
      b[i] = sum / lu[i][i];
    }
    for(i = n-1;i >= 0;i--) {
      sum = b[i];
      for(int k = i+1;k < n;k++)
	sum -= lu[k][i] * b[k];
      b[i] = sum / lu[i][i];
    }
  }
  
  void LUBackSubstitute(const MatrixC &lu,const SArray1dC<IntT> &index,SArray1dC<RealT> &b) {
    IntT i,n = lu.Rows();
    for(i = 0;i < n;i++) {
      IntT in = index[i];
      RealT sum = b[in];
      b[in] = b[i];
      for(BufferAccessIter2C<RealT,RealT> it(lu[i],b,IndexRangeC(0,i-1));it;it++)
	sum -= it.Data1() * it.Data2();
      b[i] = sum;
    }
    for(i = n-1;i >= 0;i--) {
      RealT sum = b[i];
      for(BufferAccessIter2C<RealT,RealT> it(lu[i],b,IndexRangeC(i+1,n-1));it;it++)
	sum -= it.Data1() * it.Data2();
      b[i] = sum/lu[i][i];
    }
  }
  
  bool CholeskyDecomposition(const MatrixRSC &rs,MatrixC &ret) {
    const int n = rs.Rows();
    ret = MatrixC(n,rs.Cols());
    ret.Fill(0);
    for(int i = 0;i < n;i++) {
      SizeBufferAccessC<RealT> rowi = ret[i];
      for(int j = i;j < n;j++) {
	RealT sum = rs[i][j];
	SizeBufferAccessC<RealT> rowj = ret[j];
	for(BufferAccessIter2C<RealT,RealT> it(rowi,rowj,IndexRangeC(0,i-1));it;it++)
	  sum -= it.Data1() * it.Data2();
	if(i == j) {
	  //RavlAssertMsg(sum > 0.0,"Matrix is not positive definite. ");
	  if(sum <= 0.0)
	    return false;
	  ret[i][i] = Sqrt(sum);
	} else
	  ret[j][i] = sum/ret[i][i];
      }
    }
    return true;
  }
 
  
}
