// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIXRIGHTUPPER_HEADER
#define RAVL_SMATRIXRIGHTUPPER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrixRightUpper.hh"

#include "Ravl/TSMatrixRightUpper.hh"

namespace RavlN {

  class SMatrixRightUpperC 
    : public TSMatrixRightUpperC<RealT>
  {
  public:
    SMatrixRightUpperC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    SMatrixRightUpperC(UIntT n)
      : TSMatrixRightUpperC<RealT>(n)
    {}
    //: Create a new RightUpper matrix of size n.
    // Note: The contents of the matrix are undefined.
    
    SMatrixRightUpperC(const TMatrixC<RealT> &mat)
      : TSMatrixRightUpperC<RealT>(mat)
    {}
    //: Construct from a plain matrix.
    
  };
}


#endif
