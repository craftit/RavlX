// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPRIMITIVEBINARYPOLYNOMAIL_HEADER
#define RAVLPRIMITIVEBINARYPOLYNOMAIL_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Sequence/PrimitiveBinaryPolynomial.hh"
//! lib=RavlMath
//! author="Charles Galambos"
//! date="23/03/99"
//! docentry="Ravl.API.Math.Sequences"
//! example=exPrimiteBinaryPolynomial.cc

#include "Ravl/SArray1d.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Evaluate a binary polynomial in a recurance sequence.
  // See Numerical Recipies in 'C' pg 312. (this is a full re-implementation) <p>
  // These are used in generating Sobol sequences.
  // Small object.
  
  class PrimitiveBinaryPolynomialC {
  public:
    PrimitiveBinaryPolynomialC()
      {}
    //: Default constructor.
    
    PrimitiveBinaryPolynomialC(int arg,int degree,const SArray1dC<UIntT> &ninit)
      : deg(degree),
      args(arg),
      init(ninit)
      {}
    //: Constructor.
    
    SArray1dC<UIntT> Evaluate(IntT len);
    //: Evalutate the sequence of 'len' values.  
    
  protected:
    IntT deg;     // Degree of polynomial
    IntT args;    // Args for polynomial
    SArray1dC<UIntT> init; // Inital sequence.
  };
}


#endif
