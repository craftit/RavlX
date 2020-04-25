// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTORMATRIX3D_HEADER
#define RAVL_VECTORMATRIX3D_HEADER 1
//! docentry="Ravl.API.Math.Geometry.3D"
//! author="Charles Galambos"
//! date="13/04/2002"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/FixedSize/VectorMatrix3d.hh"

#include "Ravl/FVectorMatrix.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 3D Vector matrix.
  
  class VectorMatrix3dC
    : public FVectorMatrixC<3>
  {
  public:
    VectorMatrix3dC()
    {}
    //: Default constructor.
    
    VectorMatrix3dC(const FVectorC<3> &vec,const FMatrixC<3,3> &mat)
      : FVectorMatrixC<3>(vec,mat)
    {}
    //: Construct from a vector and a matrix.
    
  };
}

#endif
