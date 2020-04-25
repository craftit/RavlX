// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Distance/Distance.cc"

#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/Assert.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  DistanceBodyC::DistanceBodyC()
  {}
  //: Default constructor.


  //: XMLFactoryC constructor.
  DistanceBodyC::DistanceBodyC(const XMLFactoryContextC &factory)
    : Function1BodyC(factory)
  {
  }
  
  DistanceBodyC::DistanceBodyC(std::istream &strm)
    : Function1BodyC(strm)
  {}
  
  //: Load from binary stream.
  
  DistanceBodyC::DistanceBodyC(BinIStreamC &strm) 
    : Function1BodyC(strm)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceBodyC::Save (std::ostream &out) const 
  { return Function1BodyC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DistanceBodyC::Save (BinOStreamC &out) const 
  { return Function1BodyC::Save(out); }
  
  //: Measure the distance from d1 to d2.
  
  RealT DistanceBodyC::Measure(const VectorC &d1,const VectorC &d2) const {
    RavlAssertMsg(0,"DistanceBodyC::Measure(), Abstract method called. ");
    return 1;
  }
  
  //: Measure the magnitude of d1.
  
  RealT DistanceBodyC::Magnitude(const VectorC &d1) const {
    VectorC zero(d1.Size());
    zero.Fill(0);
    return Measure(zero,d1);
  }
  
  //: Apply function to 'data'
  // FIXME:- Rename all the Magnitude() methods to Apply1() and
  // change the Magnitude() method of the handle to call Apply1().
  
  RealT DistanceBodyC::Apply1(const VectorC &data) const {
    return Magnitude(data);
  }
  
  ////////////////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  DistanceC::DistanceC(std::istream &strm)
    : Function1C(RAVL_VIRTUALCONSTRUCTOR(strm,DistanceBodyC))
  {}
  
  //: Load from binary stream.
  
  DistanceC::DistanceC(BinIStreamC &strm)
    : Function1C(RAVL_VIRTUALCONSTRUCTOR(strm,DistanceBodyC))
  {}

  RavlN::XMLFactoryRegisterHandleC<DistanceC> g_registerXMLFactoryDistance("RavlN::DistanceC");

  void linkDistance()
  {}
  
}
