// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Math.Linear Algebra.Lapack"
//! lib=RavlLapackWraps
//! file="Ravl/Contrib/Lapack/lapack.hh"
//! author="Kieron Messer"


#ifndef RAVL_LAPACK_HH
#define RAVL_LAPACK_HH

#include "Ravl/VectorMatrix.hh"

namespace RavlN
{
  //! userlevel=Normal

  bool InverseIP_Lapack(MatrixC & matrix);
  //: Invert a matrix in-place using Lapack routines.


  bool SVD_IP_Lapack(MatrixC & m, MatrixC & u, VectorC & s, MatrixC & v);
  //: Compute singular value decomposition of matrix using Lapack routines.
  // M = U * S * V.T().  Note the contents of m are destroyed


  bool EigenVectors_Lapack(const MatrixC & mat, VectorMatrixC & vm);
  //: Compute eigen-values and eigen-vectors of a real symmetric matrix using Lapack routines

}//end of namespace

#endif
