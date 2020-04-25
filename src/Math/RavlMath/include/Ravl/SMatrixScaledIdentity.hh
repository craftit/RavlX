// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SMATRIXSCALEDIDENTITY_HEADER
#define RAVL_SMATRIXSCALEDIDENTITY_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Linear Algebra.Smart Matrix"
//! file="Ravl/Math/LinearAlgebra/General/SMatrixScaledIdentity.hh"

#include "Ravl/TSMatrixScaledIdentity.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: ScaledIdentity matrix of real values.
  
  class SMatrixScaledIdentityC
    : public TSMatrixScaledIdentityC<RealT>
  {
  public:
    SMatrixScaledIdentityC(int i,RealT scale)
      : TSMatrixScaledIdentityC<RealT>(i,scale)
    {}
    //: Create a ScaledIdentity matrix of size i.
    
    SMatrixScaledIdentityC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    SMatrixScaledIdentityC(const TSMatrixC<RealT> &base)
      : TSMatrixScaledIdentityC<RealT>(base)
    {}
    //: Base constructor.
    // If matrix 'mat' is not a TSMatrixScaledIdentityC an invalid
    // handle will be created.

  };
  
}


#endif
