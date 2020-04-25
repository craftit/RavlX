// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MATRIXDECOMPOSITION_HEADER
#define RAVL_MATRIXDECOMPOSITION_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixDecomposition.hh"
//! docentry="Ravl.API.Math.Linear Algebra.Matrix Decomposition"
//! author="Charles Galambos"
//! userlevel=Normal

#include "Ravl/Matrix.hh"

namespace RavlN {
  class MatrixRSC;
  
  SArray1dC<IntT> LUDecomposition(MatrixC &mat,RealT &d);
  //: LU Decomposition with partial pivoting.
  // This transforms 'mat' into (L + U - I). 'd' is set to
  // the determinant of the matrix. <br>
  // The order of the rows in the matrix is changed. the mapping
  // is returned if the decomposition was succesfull, if not
  // an invalid array is returned.
  
  inline SArray1dC<IntT> LUDecomposition(MatrixC &mat) 
  { RealT d; return LUDecomposition(mat,d); }
  //: LU Decomposition with partial pivoting.
  // This transforms mat such that mat' = mat * mat.T() <br>
  // As LUDecomposition(MatrixC &,RealT &) but the determinant is discarded.
  
  bool LUDecompositionPD(MatrixC &mat,RealT &det);
  //: LUDecomposition for positive definite matrices.
  // This transforms 'mat' into (L + U - I). 'd' is set to
  // the determinant of the matrix. <br>
  // 'mat' must be symmetric and positive definite.

  inline bool LUDecompositionPD(MatrixC &mat)
  { RealT d; return LUDecompositionPD(mat,d); }
  //: LUDecomposition for positive definite matrices.
  // This transforms 'mat' into (L + U - I). <br>
  // 'mat' must be symmetric and positive definite.
  
  void LUBackSubstitute(const MatrixC &lu,const SArray1dC<IntT> &index,SArray1dC<RealT> &b);
  //: Back substitute with permutation.
  // LU Matrix must be in (L + U - I) form. Intended to work with the output of LUDecomposition(..)
  
  void LUBackSubstitute(const MatrixC &lu,SArray1dC<RealT> &b);  
  //: Back substitute.
  // LU Matrix must be in LU form. Intended to work with the output of LUDecompositionPD(..)
  
  bool CholeskyDecomposition(const MatrixRSC &m,MatrixC &l);
  //: Perform Cholesky decomposition of rs.  
  // Such that m = l * l.T() where the matrix 'm' is positive definite
  //!param: m - Matrix to decompose
  //!param: l - Variable to hold result
  //!return: True if decomposition is successfull.
  
}


#endif
