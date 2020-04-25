// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/SMatrix.cc"

#include "Ravl/SMatrix.hh"
#include "Ravl/TSMatrixFull.hh"
#include "Ravl/TSMatrixRightUpper.hh"
#include "Ravl/TSMatrixLeftLower.hh"
#include "Ravl/TSMatrixSymmetric.hh"
#include "Ravl/TSMatrixTranspose.hh"

namespace RavlN {

  //: Create a full matrix of the given size.
  
  SMatrixC::SMatrixC(UIntT rows,UIntT cols)
    : TSMatrixC<RealT>(rows,cols)
  {}
  
  //: Create a handle to full matrix mat.
  
  SMatrixC::SMatrixC(const TMatrixC<RealT> &mat) 
    : TSMatrixC<RealT>(mat)    
  {}
  

  //: Create an right upper  triangular matrix from this.
  // This will discard any values in the lower left  below
  // the diagonal of the matrix.
  
  SMatrixC SMatrixC::TriangularRightUpper() const {
    if(MatrixType() == typeid(TSMatrixRightUpperBodyC<RealT>))
      return Copy();
    return TSMatrixRightUpperC<RealT>(TMatrix());
  }
  
  //: Create an right upper  triangular matrix from this.
  // This will discard any values in the upper right above
  // the diagonal of the matrix.
  
  SMatrixC SMatrixC::TriangularLeftLower() const {
    if(MatrixType() == typeid(TSMatrixLeftLowerBodyC<RealT>))
      return Copy();
    return TSMatrixLeftLowerC<RealT>(TMatrix());
  }

}
