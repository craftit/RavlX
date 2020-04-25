// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTORMATRIX2D_HEADER
#define RAVL_VECTORMATRIX2D_HEADER 1
//! docentry="Ravl.API.Math.Geometry.2D"
//! author="Charles Galambos"
//! date="13/04/2002"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/VectorMatrix2d.hh"

#include "Ravl/FVectorMatrix.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 2D Vector matrix.
  
  class VectorMatrix2dC
    : public FVectorMatrixC<2>
  {
  public:
    VectorMatrix2dC()
    {}
    //: Default constructor.
    
    VectorMatrix2dC(const FVectorC<2> &vec,const FMatrixC<2,2> &mat)
      : FVectorMatrixC<2>(vec,mat)
    {}
    //: Construct from a vector and a matrix.
    
  };
}

#endif
