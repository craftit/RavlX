// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/DistanceMax.cc"

#include "Ravl/PatternRec/DistanceMax.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {


  //: XMLFactoryC constructor.

  DistanceMaxBodyC::DistanceMaxBodyC(const XMLFactoryContextC &factory)
    : DistanceBodyC(factory)
  {
    
  }
  
  DistanceMaxBodyC::DistanceMaxBodyC(std::istream &strm)
    : DistanceBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  DistanceMaxBodyC::DistanceMaxBodyC(BinIStreamC &strm)
    : DistanceBodyC(strm)
  {}

  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceMaxBodyC::Save (std::ostream &out) const 
  { return DistanceBodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceMaxBodyC::Save (BinOStreamC &out) const 
  { return DistanceBodyC::Save(out); }
  
  //: Measure the distance from d1 to d2.
  
  RealT DistanceMaxBodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    return VectorC(d1 - d2).MaxMagnitude();
  }
  
  //: Measure the magnitude of d1.
  
  RealT DistanceMaxBodyC::Magnitude(const VectorC &d1) const {
    return d1.MaxMagnitude();
  }

  //: Calculates Jacobian matrix at X
  
  MatrixC DistanceMaxBodyC::Jacobian (const VectorC &X) const {
    MatrixC dSdX (1,X.Size()); 
    dSdX.Fill(0);
    dSdX[0][X.MaxAbsIndex()] = Sign (X[X.MaxAbsIndex()]);
    return dSdX;
  }
  
  ////////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DistanceMaxBodyC,DistanceMaxC,DistanceC);

  RavlN::XMLFactoryRegisterHandleConvertC<DistanceMaxC, DistanceC> g_registerXMLFactoryDistanceMax("RavlN::DistanceMaxC");

  extern void linkDistanceMax()
  {}
  
}
