// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIXLEFTLOWER_HEADER
#define RAVL_SMATRIXLEFTLOWER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrixLeftLower.hh"

#include "Ravl/TSMatrixLeftLower.hh"

namespace RavlN {

  class SMatrixLeftLowerC 
    : public TSMatrixLeftLowerC<RealT>
  {
  public:
    SMatrixLeftLowerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    SMatrixLeftLowerC(UIntT n)
      : TSMatrixLeftLowerC<RealT>(n)
    {}
    //: Create a new LeftLower matrix of size n.
    // Note: The contents of the matrix are undefined.
    
    SMatrixLeftLowerC(const TMatrixC<RealT> &mat)
      : TSMatrixLeftLowerC<RealT>(mat)
    {}
    //: Construct from normal matrix.
    
  };
}


#endif
