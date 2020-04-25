// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RAWMATH_HEADER
#define RAVL_RAWMATH_HEADER 1
//! author="Charles Galambos"
//! userlevel=Advanced
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/RawMatrix.hh"
//! docentry="Ravl.API.Math.Linear Algebra.Matrix Decomposition"

#include "Ravl/Types.hh"

// These routines provide an interface for general matrix operations that don't include all the header files
// that they drag in.

namespace RavlN {

  bool RawSVD(RealT *vec,RealT *mat,UIntT n,UIntT m);
  //: Compute matrix D from SVD.
  
  bool RawSVD(RealT *vec,RealT *mat,RealT *u,UIntT n,RealT *v,UIntT m);
  //: Compute full SVD.
  
  
}

#endif
