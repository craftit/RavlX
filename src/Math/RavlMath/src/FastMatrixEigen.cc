// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id: MatrixEigen.cc 3241 2003-07-25 15:31:39Z craftit $"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/FastMatrixEigen.cc"

#include "Ravl/StdConst.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/CCMath.hh"

// matrix functions.

namespace RavlN {
  // n*m row*col
  
  
  VectorC FastEigenValues(MatrixC &mat) {
    RavlAlwaysAssert(mat.IsContinuous()); // Should we cope with this silently ?
    RavlAlwaysAssertMsg(mat.Rows() == mat.Cols(),"MatrixC::EigenValuesIP() Matrix must be square. ");
    VectorC ret(mat.Rows());
    if(mat.Rows() == 0)
      return ret;
    if(mat.Rows() == 1) {
      ret[0] = mat[0][0];
      return ret;
    }
    eigval(&(mat[0][0]),&(ret[0]),mat.Rows());
    return ret;
  }
  
  VectorC FastEigenVectors(MatrixC &mat) {
    RavlAlwaysAssert(mat.IsContinuous()); // Should we cope with this silently ?
    RavlAlwaysAssertMsg(mat.Rows() == mat.Cols(),"MatrixC::EigenVectorsIP() Matrix must be square. ");
    VectorC ret(mat.Rows());
    if(mat.Rows() == 0)
      return ret;
    if(mat.Rows() == 1) {
      ret[0] = mat[0][0];
      mat[0][0] = 1;
      return ret;
    }
    if (!eigen(&(mat[0][0]),&(ret[0]),mat.Rows())) return VectorC();
    return ret;
  }
  

  //: Get the maximum eigen value and its vector.
  
  RealT MaxEigenValue(const MatrixC &mat,VectorC &maxv) {
    RavlAlwaysAssert(mat.IsContinuous()); // Should we cope with this silently ?
    RavlAlwaysAssertMsg(mat.Rows() == mat.Cols(),"MatrixC::MaxEigenValue() Matrix must be square. ");
    maxv = VectorC(mat.Rows());
    RealT ret = evmax(const_cast<RealT *>(&mat[0][0]),&maxv[0],mat.Rows());
#if 0
    if(ret == HUGE) {
      return RavlConstN::nanReal;
    }
#endif
    return ret;
  }

}
