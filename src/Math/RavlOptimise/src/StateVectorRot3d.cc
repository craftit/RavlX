// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorRot3d.cc"

#include "Ravl/StateVectorRot3d.hh"

namespace RavlN {

  //: Constructor
  StateVectorRot3dBodyC::StateVectorRot3dBodyC(const VectorC &nx)
    : StateVectorBodyC(nx)
  {
    Postprocess();
  }

  //: Constructor
  StateVectorRot3dBodyC::StateVectorRot3dBodyC(const VectorC &x, const VectorC &xstep)
    : StateVectorBodyC(x,xstep)
  {
    Postprocess();
  }

  //: Constructor.
  StateVectorRot3dBodyC::StateVectorRot3dBodyC(const VectorC &x, const VectorC &xstep, const Matrix3dC &Rnew) 
    : StateVectorBodyC(x,xstep),
      R(Rnew)
  {}
  
  //: Get 3D rotation matrix represented by state vector
  const Matrix3dC StateVectorRot3dBodyC::GetRotMatrix() const
  {
    return R;
  }
}
