// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/StateVectorQuadratic.cc"

#include "Ravl/StateVectorQuadratic.hh"

namespace RavlN {

  //: static method to build state vector from a,b,c parameters of quadratic
  VectorC StateVectorQuadraticBodyC::BuildX(RealT a, RealT b, RealT c)
  {
    VectorC x(3);

    x[0] = a;
    x[1] = b;
    x[2] = c;
    return x;
  }

  //: Constructor
  StateVectorQuadraticBodyC::StateVectorQuadraticBodyC(RealT a, RealT b,
						       RealT c)
    : StateVectorBodyC(BuildX(a,b,c))
  {
  }

  //: Constructor
  StateVectorQuadraticBodyC::StateVectorQuadraticBodyC(RealT a, RealT b,
						       RealT c,
						       const VectorC &nxstep)
    : StateVectorBodyC(BuildX(a,b,c),nxstep)
  {
  }

  //: Get parameter a of quadratic curve y = a*x^2 + b*x + c
  RealT StateVectorQuadraticBodyC::GetA() const
  {
    return x[0];
  }
  
  //: Get parameter b of quadratic curve y = a*x^2 + b*x + c
  RealT StateVectorQuadraticBodyC::GetB() const
  {
    return x[1];
  }
  
  //: Get parameter a of quadratic curve y = a*x^2 + b*x + c
  RealT StateVectorQuadraticBodyC::GetC() const
  {
    return x[2];
  }
}
