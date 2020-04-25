// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorAffine2d.cc"

#include "Ravl/StateVectorAffine2d.hh"

namespace RavlN {
  
  //: Constructor.
  
  StateVectorAffine2dBodyC::StateVectorAffine2dBodyC(const Affine2dC &nt)
    : StateVectorBodyC(StateVecFromAffine2d(nt)),
      t(nt)
  {}
  
  //: Construct from a state vector.
  
  StateVectorAffine2dBodyC::StateVectorAffine2dBodyC(const VectorC &sv) 
    : StateVectorBodyC(sv)
  {
    Postprocess();    
  }
  
  //: Generate the state vector from the affine parameters.
  
  VectorC StateVectorAffine2dBodyC::StateVecFromAffine2d(const Affine2dC &aff) {
    VectorC ret(6);
    ret[0] = aff.SRMatrix()[0][0];
    ret[1] = aff.SRMatrix()[0][1];
    ret[2] = aff.Translation()[0];
    ret[3] = aff.SRMatrix()[1][0];
    ret[4] = aff.SRMatrix()[1][1];
    ret[5] = aff.Translation()[1];
    return ret;
  }
  
  //: Postprocess state vector after an adjustment to the parameters
  
  bool StateVectorAffine2dBodyC::Postprocess() {
    t.SRMatrix()[0][0] = x[0];
    t.SRMatrix()[0][1] = x[1];
    t.Translation()[0] = x[2];
    t.SRMatrix()[1][0] = x[3];
    t.SRMatrix()[1][1] = x[4];
    t.Translation()[1] = x[5];
    return true;
  }

}
