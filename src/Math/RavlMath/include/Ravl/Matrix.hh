// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATRIXC_HEADER
#define RAVL_MATRIXC_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/General/Matrix.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Linear Algebra"

#include "Ravl/TMatrix.hh"
#include "Ravl/FMatrix.hh"

namespace RavlN {
  
  class VectorC;
  class VectorMatrixC;
  

  //! userlevel=Normal
  //: Matrix of real values.
  
  class MatrixC
    : public TMatrixC<RealT>
  {
  public:
    MatrixC()
    {}
    //: Default constructor.

    bool IsReal() const;
    //: Test if matrix only contains real values.
    // This will return false if either nan's (Not an number) 
    // or infinite values are found.

    MatrixC(const VectorC &vec)
      : TMatrixC<RealT>((const TVectorC<RealT> &)vec)
    {}
    //: Treat vector as column matrix.
    
    MatrixC(UIntT rows,UIntT cols)
      : TMatrixC<RealT>(rows,cols)
    {}
    //: Construct a new matrix of rows x cols.
    
    MatrixC(UIntT rows,UIntT cols,const RealT *data)
      : TMatrixC<RealT>(rows,cols,data)
    {}
    //: Construct a new matrix of rows x cols with row wise array of data.

    MatrixC(UIntT rows,UIntT cols,const RealT &data)
      : TMatrixC<RealT>(rows,cols,data)
    {}
    //: Construct a new matrix, fill with copy of 'data'.
    
    MatrixC(UIntT rows,UIntT cols,SArray1dC<RealT> &data)
      : TMatrixC<RealT>(rows,cols,data)
    {}      
    //: Convert an array into a rows by cols matrix.

    MatrixC(SizeT rows,SizeT cols,const SArray1dC<RealT> &data)
      : TMatrixC<RealT>(rows,cols,data)
    {}
    //: Convert an array into a rows by cols matrix.

    MatrixC(const TMatrixC<RealT> &oth)
      : TMatrixC<RealT>(oth)
    {}
    //: Base class constructor.
    
    MatrixC(const SArray2dC<RealT> &oth)
      : TMatrixC<RealT>(oth)
    {}
    //: Base class constructor.
    
    MatrixC(RealT v1,RealT v2,
	    RealT v3,RealT v4)
      : TMatrixC<RealT>(v1,v2,v3,v4)
    {}
    //: Construct a 2 x 2 matrix from given values.
    
    MatrixC(RealT v1,RealT v2,RealT v3,
	    RealT v4,RealT v5,RealT v6,
	    RealT v7,RealT v8,RealT v9)
      : TMatrixC<RealT>(v1,v2,v3,v4,v5,v6,v7,v8,v9)
    {}
    //: Construct a 3 x 3 matrix from given values.
    
    MatrixC(const SArray2dC<FloatT> &oth)
      : TMatrixC<RealT>(oth.Size1(),oth.Size2())
    {
      for(BufferAccess2dIter2C<RealT,FloatT> it(*this,Size2(),oth,Size2());it;it++)
        it.Data1() = static_cast<RealT>(it.Data2());
    }
    //: Convert from a float matrix.
    
    operator TMatrixC<FloatT> () const {
      TMatrixC<FloatT> ret(Size1(),Size2());
      for(BufferAccess2dIter2C<RealT,FloatT> it(*this,Size2(),ret,Size2());it;it++)
        it.Data2() = static_cast<FloatT>(it.Data1());
      return ret;
    }
    //: Convert to a float matrix.
    
    template<unsigned int N,unsigned int M>
    MatrixC(const TFMatrixC<RealT,N,M> &fmat)
      : TMatrixC<RealT>(fmat)
    {}
    //: Construct from fixed size matrix.
    
    MatrixC Inverse() const;
    //: Calculate the inverse of this matrix.
    // an invalid matrix (with 0 elements) is returned if this matrix is
    // singular. 
    
    bool InverseIP(RealT &det);
    //: Calculate the inverse of this matrix and its determinant in place.
    // Returns false if matrix is singular. <p>
    // Notes:
    // The matrix must be square <p>
    // If the matrix is not stored in a continuous area of memory a slightly
    // different routine is used to do the inversion.      
    
    bool InverseIP()
    { RealT det; return InverseIP(det); }
    //: Calculate the inverse of this matrix in place.
    // Returns false if matrix is singular. <p>
    // Notes:
    // The matrix must be square <p>
    // If the matrix is not stored in a continuous area of memory a slightly
    // different routine is used to do the inversion.  
    
    MatrixC PseudoInverse(RealT thresh = 1e-5) const;
    //: Calculate the pseudo inverse 
    // <p>Uses <a href="RavlN.SVDC.html">singular value decomposition</a> to
    // decompose the matrix, and sets
    // the singular values smaller than 'thresh' to zero.</p>
    // <p>If the m x n matrix is not square, ensure m &gt; n.</p>
    // <p>If the rank of the matrix M is r, and the pseudo-inverse is denoted
    // M~, then M~ * M is a unit matrix of size r.  On the other hand, M * M~
    // is not a unit matrix.</p>

    RealT Det() const;
    //: Calculate the determinant of the matrix.
    
    void Dump() const;
    //: Dump to cout in a human readable format.
    
    void NormaliseRows();
    //: Normalise rows so they have a magnitude of 1.
    // Zero rows are ignored.
    
    void NormaliseColumns();
    //: Normalise columns so they have a magnitude of 1.
    // Zero rows are ignored.
    
    const TMatrixC<RealT> &AddOuterProduct(const TVectorC<float> &vec1,const TVectorC<float> &vec2);
    //: Add outer product of vec1 and vec2 to this matrix.

    const TMatrixC<RealT> &AddOuterProduct(const TVectorC<float> &vec1,const TVectorC<float> &vec2,const float &a);
    //: Add outer product of vec1 and vec2 multiplied by a to this matrix .

    using TMatrixC<RealT>::AddOuterProduct;
  };
  
  
  //:---
  //! docentry="Ravl.API.Math.Linear Algebra.Equation Solvers"

  VectorC Solve(const MatrixC &A, const VectorC &b);
  //: Solve a general linear system  A*<b>x</b> = <b>b</b>
  // The solution vector <b>x</b> is the return value.<br>
  // If A is singular a zero length vector is returned.
  
  bool SolveIP(MatrixC &A, VectorC &b);
  //: Solve a general linear system  A*<b>x</b> = <b>b</b> in place.
  // The vector <b>b</b> is replaced by the solution vector <b>x</b>. <br> 
  // The matrix A is replaced by the L-U factored form. <br> 
  // If A is singular, the return value is false; otherwise the return value is true.<br> 
  
  MatrixC Solve(const MatrixC &A, const MatrixC &B);
  //: Solve a general linear system  A*X = B
  // The solution matrix is the return value.<br>
  // If A is singular a zero-sized matrix is returned.
  
  bool SolveIP(MatrixC &A, MatrixC &B);
  //: Solve a general linear system  A*X = B in place. 
  // The matrix of vectors B is replaced by the solution matrix X. <br> 
  // The matrix A is replaced by the L-U factored form. <br> 
  // If A is singular, the return value is false; otherwise the return value is true.<br> 
  
  //:---
  //! docentry="Ravl.API.Math.Linear Algebra.Matrix Decomposition"
  VectorC SVD(const MatrixC &M);
  //: Singular value decomposition, eg. M = U * S * V.T(). 
  // The diagonal matrix S is returned as a vector. Values for the
  // other matrices are not computed.
  // If the operation fails the returned vector is invalid.
  
  VectorC SVD_IP(MatrixC &M);
  //: Singular value decomposition, eg. M = U * S * V.T(). 
  // The diagonal matrix S is returned as a vector. Values for the
  // other matrixes are not computed.
  // If the operation fails the returned vector is invalid. <p>
  // NB. This function destroys the contents of this matrix.
  
  VectorC SVD(const MatrixC &M,MatrixC & u, MatrixC & v);
  //: Singular value decomposition, eg. M = U * S * V.T(). 
  // The diagonal matrix S is returned as a vector.<br>
  // This also returns the U and V matrices.  The passed
  // matrices will be used to store the results if they are
  // of the correct size; otherwise new matrices will be allocated.<br>
  // If the operation fails the returned vector is invalid.
  
  VectorC SVD_IP(MatrixC &M,MatrixC & u, MatrixC & v);
  //: Singular value decomposition, eg. M = U * D * V.T(). 
  // The diagonal matrix D is returned as a vector.
  // This also returns the U and V matrices.  The passed
  // matrixes will be used to store the results if they are
  // of the correct size; otherwise new matrices will be allocated.<br>
  // If the operation fails the returned vector is invalid.<br>
  // NB. This function destroys the contents of this matrix.
  
  VectorC EigenValues(const MatrixC &M);
  //: Calculate the eigenvalues of this matrix, for real symmetric matrices
  // This matrix remains unchanged.  <br>
  // A vector of the eigen values is returned.
  // If any errors occurred a zero length vector is generated.
  
  VectorC EigenValuesIP(MatrixC &M);
  //: Calculate the eigenvalues of this matrix, for real symmetric matrices
  // The contents of this matrix is destroyed, but less memory is needed than for <a href="RavlN.EigenValuesObconst_MatrixC_AmpCb.html">EigenValues(const MatrixC &M)</a>.  <br>
  // A vector of the eigen values is returned. 
  // If any errors occurred a zero length vector is generated.
  
  VectorC FastEigenValues(MatrixC &M);
  //: Calculate the eigenvalues of this matrix, for real symmetric matrices
  // As <a href="RavlN.EigenValuesIPObMatrixC_AmpCb.html">EigenValuesIP(MatrixC &M)</a>, but uses a faster algorithm (from CCMath) that occasionally fails for ill-conditioned matrices.<br>
  // The contents of this matrix is destroyed.  <br>
  // A vector of the eigen values is returned.  
  // If any errors occur a zero length vector is generated.<br>
  
  VectorC EigenVectors(const MatrixC &M,MatrixC &E);
  //: Calculate the eigenvalues and vectors of this matrix, for real symmetric matrices.
  // M = E*L*E~ where L is the diagonal matrix of eigenvalues.<br>
  // The matrix M remains unchanged. <br>
  // L is returned as a VectorC.
  
  VectorMatrixC EigenVectors(const MatrixC &M);
  //: Calculate the eigenvalues and vectors of this matrix, for real symmetric matrices.
  // M = E*L*E~ where L is the diagonal matrix of eigenvalues.<br>
  // The matrix M remains unchanged. <br>
  // L and E are returned as the vector and matrix components respectively of the returned VectorMatrixC.
  
  VectorC EigenVectorsIP(MatrixC &M);
  //: Calculate the eigenvalues and vectors of this matrix, for real symmetric matrices
  // As <a href="RavlN.EigenVectorsObconst_MatrixC_AmpCb.html">EigenVectors(MatrixC &M)</a>, except that eigenvalue matrix E is returned through the argument A.<br>
  // The eigenvalues L are returned as VectorC.

  VectorC FastEigenVectors(MatrixC &M);
  //: Calculate the eigen values and vectors of this matrix, for real symmetric matrices
  // As <a href="RavlN.EigenVectorsIPObMatrixC_AmpCb.html">EigenVectorsIP(MatrixC &M)</a>, but uses a faster algorithm (from CCMath) that occasionally fails for ill-conditioned matrices.<br>
  // The eigenvalue matrix E is returned through the argument M.<br>
  // L is returned as a VectorC.
  // If any errors occur a zero length vector is generated.<br>

  RealT MaxEigenValue(const MatrixC &M,VectorC &maxv);
  //: Get the maximum eigen value and its vector, for real symmetric matrices
  
  //:---
  //! docentry="Ravl.API.Math.Linear Algebra"
  MatrixC RandomMatrix(int n,int m,RealT scale = 1);
  //: Create a random matrix of values between -scale and scale with the given size.
  
  MatrixC RandomSymmetricMatrix(int n,RealT scale = 1);
  //: Create a random symmetric matrix of values between -scale and scale with the given size.
  
  MatrixC RandomPositiveDefiniteMatrix(int n);
  //: Create a random positive definite matrix.
  // The matrix is also symmetric in the current implementation, this may be changed at some point.
}

namespace RavlN {
  //! userlevel=Develop
  bool InverseIP_GaussJordan(MatrixC & m, RealT & det);
  //: The default Matrix inverse routine.
}

#endif
