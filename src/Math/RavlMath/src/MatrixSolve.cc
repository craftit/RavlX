// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixSolve.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Vector.hh"
#include "Ravl/CCMath.hh"
#include "Ravl/MatrixDecomposition.hh"

namespace RavlN {
  
  //: Solve a general linear system  A*x = b
  // The input matix A is this one.  The input
  // vector is b, which is replaced by the ouput x.
  // This matrix is altered to L-U factored form by the computation.
  
  bool SolveIP(MatrixC &mat,VectorC &b) {
    RavlAlwaysAssertMsg(mat.Cols() == mat.Rows(),"MatrixC::SolveIP(), Matrix must be square. ");
    RavlAlwaysAssertMsg(mat.Cols() == b.Size(),"MatrixC::SolveIP(), Matrix must be the same size as the vector to solve. ");
    return solv(&mat[0][0],&b[0],mat.Rows()) == 0;
  }
  
  
  //: Solve a general linear system  A*x = b
  // Where a is this matrix, and X is the returned.
  // If matrix is singular a zero length vector is returned.
  
  VectorC Solve(const MatrixC &mat,const VectorC &b)  {
    VectorC ret(b.Copy());
    MatrixC tmp(mat.Copy());
    if(!SolveIP(tmp,ret))
      return VectorC();
    return ret;
  }

  //: Solve a general linear system  A*X = B in place. 
  // A is modified to LU form and B is overwritten with the result X.
  
  bool SolveIP(MatrixC &lu,MatrixC &B) {
    RavlAlwaysAssertMsg(lu.Cols() == lu.Rows(),"MatrixC::Solve(), Matrix A must be square. ");
    RavlAlwaysAssertMsg(lu.Cols() == B.Rows(),"MatrixC::Solve(), A.Cols() must equal B.Rows(). ");
    MatrixC ret(B.Rows(),B.Cols());
    SArray1dC<IntT> index = LUDecomposition(lu);
    if(index.Size() == 0)
      return false;
    IntT n = (IntT) B.Cols();
    VectorC tmp(B.Rows());
    for(int i = 0;i < n;i++) {
      tmp.CopyFrom(const_cast<MatrixC &>(B).SliceColumn(i));
      LUBackSubstitute(lu,index,tmp);
      B.SliceColumn(i).CopyFrom(tmp);
    }
    return true;
  }
  
  
  //: Solve a general linear system  A*X = B
  // This return's X, or a 0 by 0 matrix if it fails.
  
  MatrixC Solve(const MatrixC &A,const MatrixC &B) {
    RavlAlwaysAssertMsg(A.Cols() == A.Rows(),"MatrixC::Solve(), Matrix A must be square. ");
    RavlAlwaysAssertMsg(A.Cols() == B.Rows(),"MatrixC::Solve(), A.Cols() must equal B.Rows(). ");
    MatrixC ret(B.Rows(),B.Cols());
    MatrixC lu = A.Copy();
    SArray1dC<IntT> index = LUDecomposition(lu);
    if(index.Size() == 0)
      return MatrixC(); // Failed.
    IntT n = (IntT) B.Cols();
    VectorC tmp(B.Rows());
    for(int i = 0;i < n;i++) {
      tmp.CopyFrom(const_cast<MatrixC &>(B).SliceColumn(i));
      LUBackSubstitute(lu,index,tmp);
      ret.SliceColumn(i).CopyFrom(tmp);
    }
    return ret;
  }

  //: Solve a general linear system  A*X = B
  // This return's X, or a 0 by 0 matrix if it fails.
  
  MatrixC SolvePD(const MatrixC &A,const MatrixC &B) {
    RavlAlwaysAssertMsg(A.Cols() == A.Rows(),"MatrixC::Solve(), Matrix A must be square. ");
    RavlAlwaysAssertMsg(A.Cols() == B.Rows(),"MatrixC::Solve(), A.Cols() must equal B.Rows(). ");
    MatrixC ret(B.Rows(),B.Cols());
    MatrixC lu = A.Copy();
    if(!LUDecompositionPD(lu))
      return MatrixC(); // Failed.
    IntT n = (IntT) B.Cols();
    VectorC tmp(B.Rows());
    for(int i = 0;i < n;i++) {
      tmp.CopyFrom(const_cast<MatrixC &>(B).SliceColumn(i));
      LUBackSubstitute(lu,tmp);
      ret.SliceColumn(i).CopyFrom(tmp);
    }
    return ret;
  }

  //: Solve a general linear system  A*X = B
  
  bool SolvePD_IP(MatrixC &lu,MatrixC &B) {
    RavlAlwaysAssertMsg(lu.Cols() == lu.Rows(),"MatrixC::Solve(), Matrix A must be square. ");
    RavlAlwaysAssertMsg(lu.Cols() == B.Rows(),"MatrixC::Solve(), A.Cols() must equal B.Rows(). ");
    if(!LUDecompositionPD(lu))
      return false; // Failed.
    IntT n = (IntT) B.Cols();
    VectorC tmp(B.Rows());
    for(int i = 0;i < n;i++) {
      tmp.CopyFrom(const_cast<MatrixC &>(B).SliceColumn(i));
      LUBackSubstitute(lu,tmp);
      B.SliceColumn(i).CopyFrom(tmp);
    }
    return true;
  }

}
