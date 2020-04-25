// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INTELMKL_HEADER
#define RAVL_INTELMKL_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Math.Linear Algebra.IntelMKL"
//! lib=RavlIntelMKL
//! file="Ravl/Contrib/IntelMKL/IntelMKL.hh"
//! author="Kieron Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/MatrixRS.hh"

namespace RavlN {

  //! userlevel=Develop
  bool InverseIP_IntelMKL_hook(MatrixC & matrix, RealT & det);
  //: The hook used to replace Ravl inverse function.

  bool EigenVectors_IntelMKL_hook(VectorC & evalues, MatrixC & evector, const MatrixC & m);
  //: The hook used to replace the Ravl EigenVectors function
}

namespace RavlN {

  //! userlevel=Normal
  bool InverseIP_IntelMKL(MatrixC & matrix);
  //: Invert a matrix in-place using Intel MKL functions

  bool EigenVectors_IntelMKL(const MatrixC &mat, VectorMatrixC & vm);
  //: Compute eigen-values and eigen-vectors of a real symmetric matrix using Intel MKL functions

  bool SVD_IP_IntelMKL(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v);
//: Compute SVD of matrix M = U * S * V.T() using IntelMKL functions.
// Note the contents of m are destroyed

}

#endif
