// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIXDIAGONAL_HEADER
#define RAVL_SMATRIXDIAGONAL_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrixDiagonal.hh"

#include "Ravl/TSMatrixDiagonal.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Diagonal matrix of real values.
  
  class SMatrixDiagonalC
    : public TSMatrixDiagonalC<RealT>
  {
  public:
    SMatrixDiagonalC(int i)
      : TSMatrixDiagonalC<RealT>(i)
    {}
    //: Create a diagonal matrix of size i.
    
    SMatrixDiagonalC(const SArray1dC<RealT> &dat)
      : TSMatrixDiagonalC<RealT>(dat)
    {}
    //: Construct diagonal from an array
    // Construct from an array of values, can be used to
    // construct matrix from a VectorC.
    
    SMatrixDiagonalC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
  };
  
}


#endif
