// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIXSYMMETRIC_HEADER
#define RAVL_SMATRIXSYMMETRIC_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrixSymmetric.hh"

#include "Ravl/TSMatrixSymmetric.hh"

namespace RavlN {

  class SMatrixSymmetricC 
    : public TSMatrixSymmetricC<RealT>
  {
    SMatrixSymmetricC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    SMatrixSymmetricC(UIntT n)
      : TSMatrixSymmetricC<RealT>(n)
    {}
    //: Create a new symmetric matrix of size n.
    // Note: The contents of the matrix are undefined.
    
    SMatrixSymmetricC(const TMatrixC<RealT> &mat)
      : TSMatrixSymmetricC<RealT>(mat)
    {}
    //: Constructor.
    
    SMatrixSymmetricC(const TSMatrixC<RealT> &mat)
      : TSMatrixSymmetricC<RealT>(mat)
    {}
    //: Create from a base handle.
    // If matrix 'mat' is not a TSMatrixSymmetricC an invalid
    // handle will be created.
    
  };
}


#endif
