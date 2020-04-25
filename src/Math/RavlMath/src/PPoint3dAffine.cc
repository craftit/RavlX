// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath

#include "Ravl/PPoint3d.hh"
#include "Ravl/Affine3d.hh"
#include "Ravl/PProjection3d.hh"

namespace RavlN {

  // Construct from an affine transform.
  
  PProjection3dC::PProjection3dC(const Affine3dC &aff)
    : Matrix4dC(aff.SRMatrix()[0][0],aff.SRMatrix()[0][1],aff.SRMatrix()[0][2], aff.Translation()[0],
                aff.SRMatrix()[1][0],aff.SRMatrix()[1][1],aff.SRMatrix()[1][2], aff.Translation()[1],
                aff.SRMatrix()[2][0],aff.SRMatrix()[2][1],aff.SRMatrix()[2][2], aff.Translation()[2],
                0.0, 0.0, 0.0, 1.0)
  {}
  

  // Affine transform of the space.
  
  PPoint3dC Transform(const FAffineC<3> &transform,const PPoint3dC & p) {
    const FMatrixC<3,3> &sr = transform.SRMatrix();
    const FVectorC<3> &t =  transform.Translation();
    return PPoint3dC(sr[0][0] * p[0] + sr[0][1] * p[1] + sr[0][2] * p[2] + t[0] * p[3],
                     sr[1][0] * p[0] + sr[1][1] * p[1] + sr[1][2] * p[2] + t[1] * p[3],
                     sr[2][0] * p[0] + sr[2][1] * p[1] + sr[2][2] * p[2] + t[2] * p[3],
                     p[3]);
    
  }
  
}
