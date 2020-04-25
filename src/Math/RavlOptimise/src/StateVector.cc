// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVector.cc"

#include "Ravl/StateVector.hh"

#define DEFAULT_STEP_SIZE 1.0e-6

namespace RavlN {

  //: Constructor
  StateVectorBodyC::StateVectorBodyC(const VectorC &nx)
  {
    // copy state vector
    x = nx.Copy();

    // create vector of step sizes
    xstep = nx.Copy();
    for ( IntT i=nx.Size()-1; i >= 0; i-- )
      xstep[i] = DEFAULT_STEP_SIZE;
  }

  //: Constructor
  StateVectorBodyC::StateVectorBodyC(const VectorC &nx, const VectorC &nxstep)
  {
    // copy state vector
    x = nx.Copy();

    // copy step sizes
    xstep = nxstep.Copy();
  }
  
  //: Set state vector
  bool StateVectorBodyC::SetX(VectorC &xnew)
  {
    for ( IntT i=x.Size()-1; i >= 0; i-- )
      x[i] = xnew[i];

    Postprocess();
    return true;
  }

  //: Increment state vector
  bool StateVectorBodyC::IncX(VectorC &xinc)
  {
    x += xinc;
    Postprocess();
    return true;
  }

  //: Increment element of state vector
  bool StateVectorBodyC::IncElX(IntT index, RealT val)
  {
    x[index] += val;
    Postprocess();
    return true;
  }

  //: Decrement element of state vector
  bool StateVectorBodyC::DecElX(IntT index, RealT val)
  {
    x[index] -= val;
    Postprocess();
    return true;
  }

  //: Increment the linear system
  //: Default is to do nothing.
  bool StateVectorBodyC::IncrementLS(MatrixRSC &A, VectorC &a)
  {
    return true;
  }

  //: Postprocess state vector after an adjustment to the parameters.
  //: Default is to do nothing.
  bool StateVectorBodyC::Postprocess()
  {
    return true;
  }
}
