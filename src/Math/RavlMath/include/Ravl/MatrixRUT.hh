// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATRIXRUT_HEADER
#define RAVL_MATRIXRUT_HEADER 1
///////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/General/MatrixRUT.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Linear Algebra"

#include "Ravl/Matrix.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Right Upper Triangular matrix.
  // <p>This class contains functions optimised for working with
  // Right Upper Triangular matrix's.  The lower left is assumed
  // to be zero. Most of the operations on this matrix ignore the
  // contents of the lower left part, and it is not guaranteed to contain
  // zero's.  To ensure this use 'ZeroLowerLeft()': this MUST be
  // done before using matrix operators that are not part of this 
  // class. </p>
  // <p>The matrix MUST be square.</p>
  
  class MatrixRUTC
    : public MatrixC
  {
  public:
    MatrixRUTC()
    {}
    //: Default constructor.
    
    explicit MatrixRUTC(SizeT size)
      : MatrixC(size,size)
    {}
    //: Create a new matrix of size * size.
    
    MatrixRUTC(const MatrixC &oth)
      : MatrixC(oth)
    {  RavlAssert(oth.Rows() == oth.Cols()); }
    //: Base constructor.

    MatrixRUTC(const SArray2dC<RealT> &oth)
      : MatrixC(oth)
    {  RavlAssert(oth.Size1() == oth.Size2()); }
    //: Base constructor.
    
    MatrixRUTC Inverse() const;
    //: Calculate the inverse of a upper right triangular matrix.
    // an invalid matrix is returned if this matrix is
    // singular. This can be tested for by IsValid().
    
    bool InverseIP();
    //: Calculate the inverse of a upper right triangular matrix. In place.
    // NOTE: The result is not upper triangular.
    // False is returned if matrix is singular, true otherwise.
    
    void AddOuterProduct(const VectorC &vec);
    //: Add outer product of 'vec' with itself to this matrix.
    
    void AddOuterProduct(const VectorC &vec,RealT a);
    //: Add outer product of 'vec' with itself to this matrix multiplied by a

    void SubtractOuterProduct(const VectorC &vec);
    //: Subtract outer product of 'vec' with itself to this matrix.
    
    void SubtractOuterProduct(const VectorC &vec,RealT a);
    //: Subtract outer product of 'vec' with itself to this matrix multiplied by a
    
    const MatrixRUTC &operator+=(const MatrixRUTC &mat);
    //: Add another right upper matrix to this one.
    
    const MatrixRUTC &operator-=(const MatrixRUTC &mat);
    //: Subtract another right upper matrix from this one.
    
    const MatrixRUTC &operator*=(RealT val);
    //: Multiply by a constant.
    
    const MatrixRUTC &operator/=(RealT val);
    //: Divide by a constant.
    
    void ZeroLowerLeft();
    //: Fill the lower left part of the matrix with zero's.
    // Note: This is an in place operation.
    
    void MakeSymmetric();
    //: Make this matrix symmetric.
    // Copy the upper right triangle to the lower left.
    // Note: this is an in-place operation.
    
    RealT Det() const;
    //: Calculate the determinant of the matrix.
    // Very quick, Just the product of the diagonal.
    
  };

  MatrixRUTC OuterProductRUT(const VectorC &vec);
  //: Return outer product of 'vec' as a right upper triangular matrix.
  
  MatrixRUTC OuterProductRUT(const VectorC &vec,RealT a);
  //: Return outer product of 'vec' as a right upper triangular matrix multiplied by a.
  
  //:---
  //! docentry="Ravl.API.Math.Linear Algebra.Equation Solvers"

  bool SolveIP(MatrixRUTC &A,VectorC &b);
  //: Solve a general linear system  A*x = b in place.
  // The input vector is b, which is replaced by the ouput x. <p>
  // This matrix is altered to L-U factored form by the computation. <p>
  // If the input matrix is singular, false is returned and
  // true if the operation succeeded.
  
  VectorC Solve(const MatrixRUTC &A,const VectorC &b);
  //: Solve a general linear system  A*x = b
  // Where X is the returned vector.
  // If matrix is singular a zero length vector is returned.
}

#endif
