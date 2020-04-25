// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTORMATRIX4D_HEADER
#define RAVL_VECTORMATRIX4D_HEADER 1
//! docentry="Ravl.API.Math.Geometry.4D"
//! author="Charles Galambos"
//! date="13/04/2002"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/VectorMatrix4d.hh"

#include "Ravl/FVectorMatrix.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 4D Vector matrix.
  
  class VectorMatrix4dC
    : public FVectorMatrixC<4>
  {
  public:
    VectorMatrix4dC()
    {}
    //: Default constructor.
    
    VectorMatrix4dC(const FVectorC<4> &vec,const FMatrixC<4,4> &mat)
      : FVectorMatrixC<4>(vec,mat)
    {}
    //: Construct from a vector and a matrix.
    
  };
}

#endif
