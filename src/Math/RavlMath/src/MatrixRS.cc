// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixRS.cc"

#include "Ravl/MatrixRS.hh"
#include "Ravl/CCMath.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  //: Calculate the inverse of a upper right triangular matrix.
  // an invalid matrix is returned if this matrix is
  // singular. This can be tested for by IsValid().
  
  MatrixRSC MatrixRSC::Inverse() const {
    MatrixRSC ret = Copy();
    if(!ret.InverseIP()) {
      // Could throw an exception ?
      return MatrixRSC(); 
    }
    return ret;
  }
  
  bool MatrixRSC::InverseIP() {
    RavlAlwaysAssert(IsContinuous()); // What should we do now ?
    RavlAlwaysAssertMsg(Cols() == Rows(),"MatrixRSC::InverseIP(), Matrix must be square to invert ");    
    bool ret = psinv(&(*this)[0][0],Rows()) == 0; // ccmath routine.
    MakeSymmetric();
    return ret;
  }

  //: Make this matrix symmetric.
  // Copy the upper right triangle to the lower left.
  // Note: this is an in-place operation.

  void MatrixRSC::MakeSymmetric() {
    for(unsigned int i = 0;i < Rows();i++) {
      for(unsigned int j = i + 1;j < Cols();j++)
	(*this)[j][i] = (*this)[i][j];
    }    
  }
  
  //: Solve a general linear system  A*x = b
  // The input matix A is this one.  The input
  // vector is b, which is replaced by the ouput x.
  // This matrix is altered to L-U factored form by the computation.
  
  bool SolveIP(MatrixRSC &mat,VectorC &b) {
    RavlAlwaysAssertMsg(mat.Cols() == mat.Rows(),"MatrixC::SolveIP(), Matrix must be square. ");
    RavlAlwaysAssertMsg(mat.Cols() == b.Size(),"MatrixC::SolveIP(), Matrix must be the same size as the vector to solve. ");
    return solvps(&mat[0][0],&b[0],mat.Rows()) == 0;
  }
  
  
  //: Solve a general linear system  A*x = b
  // Where a is this matrix, and X is the returned.
  // If matrix is singular a zero length vector is returned.
  
  VectorC Solve(const MatrixRSC &mat,const VectorC &b) {
    VectorC ret(b.Copy());
    MatrixRSC tmp(mat.Copy());
    if(!SolveIP(tmp,ret))
      return VectorC();
    return ret;
  }
  
  
}
