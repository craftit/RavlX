// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/DistanceCityBlock.cc"

#include "Ravl/PatternRec/DistanceCityBlock.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  //: XMLFactoryC constructor.

  DistanceCityBlockBodyC::DistanceCityBlockBodyC(const XMLFactoryContextC &factory)
    : DistanceBodyC(factory)
  {
  }

  //: Load from stream.
  
  DistanceCityBlockBodyC::DistanceCityBlockBodyC(std::istream &strm)
    : DistanceBodyC(strm)
  {}
  
  //: Load from binary stream.

  DistanceCityBlockBodyC::DistanceCityBlockBodyC(BinIStreamC &strm)
    : DistanceBodyC(strm)
  {
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceCityBlockBodyC::Save (std::ostream &out) const  
  { return DistanceBodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceCityBlockBodyC::Save (BinOStreamC &out) const
  { return DistanceBodyC::Save(out); }
 
  
  //: Measure the distance from d1 to d2.
  
  RealT DistanceCityBlockBodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    return VectorC(d1 - d2).SumOfAbs();
  }
  
  //: Measure the magnitude of d1.
  
  RealT DistanceCityBlockBodyC::Magnitude(const VectorC &d1) const {
    return d1.SumOfAbs();
  }
  
  //: Calculates Jacobian matrix at X
  
  MatrixC DistanceCityBlockBodyC::Jacobian (const VectorC &X) const {
    MatrixC dSdX (1,X.Size());
    for(int i = 0;i < (int) X.Size();i++)
      dSdX[0][i] = Sign(X[i]);
    return dSdX;
  }

  /////////////////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DistanceCityBlockBodyC,DistanceCityBlockC,DistanceC);

  RavlN::XMLFactoryRegisterHandleConvertC<DistanceCityBlockC, DistanceC> g_registerXMLFactoryDistanceCityBlock("RavlN::DistanceCityBlockC");

  extern void linkDistanceCityBlock()
  {}
  
  
}
