// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: FunctionAckley.cc 4106 2004-03-18 16:30:45Z kier $"
//! lib=RavlPatternRec
//! author="Kieron Messer"
//! file="Ravl/PatternRec/Modeling/Basic/FunctionAckley.cc"

#include "Ravl/PatternRec/TestFunctions.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/config.h"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  
  /*
   * Ackley function
   */

  FunctionAckleyBodyC::FunctionAckleyBodyC(UIntT numberOfDimensions) :
      FunctionBodyC(numberOfDimensions, 1)
  {
  }
  
  VectorC FunctionAckleyBodyC::Apply(const VectorC &data) const
  {

    VectorC out(1);
    RealT n = (RealT) InputSize();
    RealT a = 20;
    RealT b = 0.2;
    RealT c = 2.0 * RavlConstN::pi;
    RealT s1 = 0.0, s2 = 0.0;

    for (SArray1dIterC<RealT> it(data); it; it++) {
      s1 = s1 + Pow(*it, 2.0);
      s2 = s2 + Cos(c * *it);
    }

    out[0] = -a * Exp(-b * Sqrt(1.0 / n * s1)) - Exp(1.0 / n * s2) + a + Exp(1.0);
    //std::cerr << data << ' ' << out[0] << '\n';
    return out;
  }

  /*
   * Simple Sphere Function
   */

  FunctionSphereBodyC::FunctionSphereBodyC(UIntT numberOfDimensions) :
      FunctionBodyC(numberOfDimensions, 1)
  {
  }

  VectorC FunctionSphereBodyC::Apply(const VectorC &data) const
  {

    RealT s = 0.0;
    for (SArray1dIterC<RealT> it(data); it; it++) {
      s += Pow(*it, 2.0);
    }
    VectorC out(1);
    out[0] = s;
    //std::cerr << data << ' ' << out[0] << '\n';
    return out;
  }

  /*
   * Simple Sphere Function
   */

  FunctionGriewankBodyC::FunctionGriewankBodyC(UIntT numberOfDimensions) :
      FunctionBodyC(numberOfDimensions, 1)
  {
  }

  VectorC FunctionGriewankBodyC::Apply(const VectorC &data) const
  {
    VectorC out(1);
    RealT fr = 4000.0;
    RealT p = 1.0;
    RealT s = 0.0;

    for (SArray1dIterC < RealT > it(data); it; it++) {
      s += Pow(*it, 2.0);
      p *= Cos(*it/Sqrt((RealT)(it.Index().V()+1)));
    }
    out[0] = 1.0 + (s/fr)- p;
    return out;
  }

}
