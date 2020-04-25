// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_STOREDSTATE_HEADER
#define RAVLMATH_STOREDSTATE_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StoredState.hh"

#include "Ravl/StateVector.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //! autoLink=on
  //: This class is the base class for optimisation algorithm classes
  // It stores a state vector x of parameters.
  class StoredStateC {

  public:
    StoredStateC();
    //: Default constructor for optimisation base class

    StoredStateC(const StateVectorC &stateVecInit);
    //: Constructor for optimisation base class

  protected:
    StateVectorC stateVec; // state parameter vector

  public:
    inline const StateVectorC &GetSolution() const
    { return stateVec; }
    //: Solution parameters x for sample with lowest residual
  };
}


#endif
