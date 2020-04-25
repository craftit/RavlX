// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIX_HEADER
#define RAVL_SMATRIX_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! date="20/8/2002"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrix.hh"

#include "Ravl/TSMatrix.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Smart matrix handle.
  
  class SMatrixC
    : public TSMatrixC<RealT>
  {
  public:
    SMatrixC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    SMatrixC(const TSMatrixC<RealT> &mat)
      : TSMatrixC<RealT>(mat)
    {}
    //: Base class constructor.
    
    SMatrixC(UIntT rows,UIntT cols);
    //: Create a full matrix of the given size.
    
    SMatrixC(const TMatrixC<RealT> &mat);
    //: Create a handle to full matrix mat.
    // Note: This does not copy the matrix.
    
    SMatrixC TriangularRightUpper() const;
    //: Create an right upper  triangular matrix from this.
    // This will discard any values in the lower left  below
    // the diagonal of the matrix.
    
    SMatrixC TriangularLeftLower() const;
    //: Create an right upper  triangular matrix from this.
    // This will discard any values in the upper right above
    // the diagonal of the matrix.
    
    SMatrixC Copy() const
    { return TSMatrixC<RealT>::Copy(); }
    //: Create a copy.
  };
}

#endif
