// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIXSPARSE_HEADER
#define RAVL_SMATRIXSPARSE_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrixSparse.hh"

#include "Ravl/TSMatrixSparse.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Sparse matrix of real values.
  
  class SMatrixSparseC 
    : public TSMatrixSparseC<RealT>
  {
  public:
    SMatrixSparseC()
    {}
    //: Default constructor.
    // Create an invalid handle.
    
    SMatrixSparseC(UIntT rows,UIntT cols)
      : TSMatrixSparseC<RealT>(rows,cols)
    {}
    //: Create a matrix of the given size.
    // The matrix is effectively empty and behaves as if it was filled
    // with zero's.
    
    SMatrixSparseC(const TMatrixC<RealT> &mat,const RealT &zeroValue = 1e-6)
      : TSMatrixSparseC<RealT>(mat,zeroValue)
    {}
    //: Construct a sparse matrix for a full matrix.
    // Any elements with an absolute value smaller than 'zeroValue' are taken as zero.
    
    SMatrixSparseC(TSMatrixC<RealT> &xyz)
      : TSMatrixSparseC<RealT>(xyz)
    {}
    //: Base constructor.
    // If argument is not a sparse matrix an invalid handle will be created.
    

  };
}


#endif
