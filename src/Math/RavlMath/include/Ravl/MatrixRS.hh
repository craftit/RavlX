// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATRIXRS_HEADER
#define RAVL_MATRIXRS_HEADER 1
///////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/General/MatrixRS.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Linear Algebra"

#include "Ravl/Matrix.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Positive Symetric matrix
  // This class contains functions optimised for working with
  // positive definite symetric matrix's. <p>
  // This matrix MUST be square.
  
  class MatrixRSC
    : public MatrixC
  {
  public:
    MatrixRSC()
    {}
    //: Default constructor.
    
    explicit MatrixRSC(UIntT size)
      : MatrixC(size,size)
    {}
    //: Create a new matrix of size * size.
    
    MatrixRSC(const MatrixC &oth)
      : MatrixC(oth)
    { RavlAssert(oth.Rows() == oth.Cols()); }
    //: Base constructor.

    MatrixRSC(const SArray2dC<RealT> &oth)
      : MatrixC(oth)
    { RavlAssert(oth.Size1() == oth.Size2()); }
    //: Base constructor.
    
    MatrixRSC Inverse() const;
    //: Calculate the inverse of a symmetric matrix.
    // an invalid matrix is returned if this matrix is
    // singular. This can be tested for by IsValid().

    bool InverseIP();
    //: Calculate the inverse of a symmetric matrix in place.
    // "false" is returned if this matrix is singular. 
    
    MatrixC NearSingularInverse(RealT &det) const;
    //: Inverts this matrix and returns determinant of original matrix.
    // Note: This currently only works for positive definite matrices.
    // This routine is particularly useful when the matrix is near singular
    // as it uses PCA to first rotate co-ordinate axis, so no nasty divisions.
    // See Fukunaga -Introduction to Statistical Pat Rec, page 40.
    
    void MakeSymmetric();
    //: Make this matrix symmetric.
    // Copy the upper right triangle to the lower left.<br>
    // Note: this is an in-place operation.
    
  };

//! docentry="Ravl.API.Math.Linear Algebra.Equation Solvers"

  bool SolveIP(MatrixRSC &A,VectorC &b);
  //: Solve a general linear system  A*x = b
  // The input vector is b, which is replaced by the ouput x. <p>
  // This matrix is altered to L-U factored form by the computation. <p>
  // If the input matrix is singular, false is returned and
  // true if the operation succeeded.
  
  VectorC Solve(const MatrixRSC &A,const VectorC &b);
  //: Solve a general linear system  A*x = b
  // Where X is the returned vector.
  // If matrix is singular a zero length vector is returned.
  
  MatrixC SolvePD(const MatrixC &A,const MatrixC &B);
  //: Solve a general linear system  A*X = B where A is postive definite.
  // This returns the solution X, or a 0 by 0 matrix if it fails.  This can be tested for by IsValid().
  
  bool SolvePD_IP(MatrixC &A,MatrixC &B);
  //: Solve a general linear system  A*X = B in place, where A is postive definite.
  // A is modified to its LU form and B is replace by X.
  
  
}

#endif
