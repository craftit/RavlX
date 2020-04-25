// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Sequence/exPrimiteBinaryPolynomial.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Sequences"
//! userlevel=Normal

#include "Ravl/PrimitiveBinaryPolynomial.hh"

using namespace RavlN;

int main()
{
  SArray1dC<UIntT> init(3);
  
  {
    init[0] = 1;
    PrimitiveBinaryPolynomialC bp(0,1,init);
    SArray1dC<UIntT> seq = bp.Evaluate(4);
    for(UIntT i = 0;i < seq.Size();i++) {
      std::cerr << seq[i] << ' ' ;
    }
    std::cerr << "\n";
  }
  {
    init[0] = 1;
    init[1] = 1;
    PrimitiveBinaryPolynomialC bp(1,2,init);
    SArray1dC<UIntT> seq = bp.Evaluate(4);
    for(UIntT i = 0;i < seq.Size();i++) {
      std::cerr << seq[i] << ' ' ;
    }
    std::cerr << "\n";
  }
  {
    init[0] = 1;
    init[1] = 3;
    init[2] = 7;
    PrimitiveBinaryPolynomialC bp(1,3,init);
    SArray1dC<UIntT> seq = bp.Evaluate(4);
    for(UIntT i = 0;i <  seq.Size();i++) {
      std::cerr << seq[i] << ' ' ;
    }
    std::cerr << "\n";
  }
  {
    init[0] = 1;
    init[1] = 3;
    init[2] = 3;
    PrimitiveBinaryPolynomialC bp(2,3,init);
    SArray1dC<UIntT> seq = bp.Evaluate(4);
    for(UIntT i = 0;i <  seq.Size();i++) {
      std::cerr << seq[i] << ' ' ;
    }
    std::cerr << "\n";
  }
  return 0;
}
