// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StoredState.cc"

#include "Ravl/StoredState.hh"

namespace RavlN {

  //: Constructor for optimisation base class
  StoredStateC::StoredStateC()
  {
    stateVec = StateVectorC();
  }

  //: Constructor for optimisation base class
  StoredStateC::StoredStateC(const StateVectorC &stateVecInit)
  {
    stateVec = stateVecInit.Copy();
  }
}
