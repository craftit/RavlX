// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////
//! docentry="Algorithms.Data Processing"
//! lib=RavlIO
//! rcsid="$Id$"
//! file="Ravl/Core/IO/Process.cc"

#include "Ravl/DP/Process.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {

  //: Default constructor.

  DPProcessBaseBodyC::DPProcessBaseBodyC()
  {}

  //: Stream constructor.

  DPProcessBaseBodyC::DPProcessBaseBodyC(std::istream &in)
    : DPEntityBodyC(in)
  {}

  //: Binary stream constructor.

  DPProcessBaseBodyC::DPProcessBaseBodyC(BinIStreamC &in)
    : DPEntityBodyC(in)
  {}

  //: Copy constructor.

  DPProcessBaseBodyC::DPProcessBaseBodyC(const DPProcessBaseBodyC &oth)
    : DPEntityBodyC(oth)
  {}

  //: Save to std::ostream.
  
  bool DPProcessBaseBodyC::Save(std::ostream &out) const 
  { return DPEntityBodyC::Save(out); }
  
  //: Save to binary stream.  
  
  bool DPProcessBaseBodyC::Save(BinOStreamC &out) const
  { return DPEntityBodyC::Save(out); }
  
  
  //: Get number of inputs..
  
  UIntT DPProcessBaseBodyC::NoInputs() const 
  { return 1; }
  
  //: Get number of outputs.
  
  UIntT DPProcessBaseBodyC::NoOutputs() const 
  { return 1; }
  
  //: Get input type.
  
  const std::type_info &DPProcessBaseBodyC::InputType(int n) const  { 
    return typeid(void); 
  }
  
  //: Get Output type.
  
  const std::type_info &DPProcessBaseBodyC::OutputType(int n) const  { 
    return typeid(void); 
  }
  
  //////////////////////////////////////////////////////////
  
  //: Constructor from an abstract handle.
  
  DPProcessBaseC::DPProcessBaseC(const RCAbstractC &abst) 
    : DPEntityC(abst) 
  { CheckHandleType(Body()); }
  
  //: Stream constructor.
  
  DPProcessBaseC::DPProcessBaseC(std::istream &strm) 
    : DPEntityC(RAVL_VIRTUALCONSTRUCTOR(strm,DPProcessBaseBodyC))
  {}
  
  
  DPProcessBaseC::DPProcessBaseC(BinIStreamC &strm)
    : DPEntityC(RAVL_VIRTUALCONSTRUCTOR(strm,DPProcessBaseBodyC))
  {}

  //////////////////////////////////////////////////////////

  static DPEntityBodyC::RefT Conv2Base(const DPProcessBaseBodyC::RefT &aProc) {
    return aProc.BodyPtr();
  }

  static DPProcessBaseBodyC::RefT Conv2BaseArrReal(const DPProcessBodyC<SArray1dC<RealT>,SArray1dC<RealT> >::RefT &aProc) {
    return aProc.BodyPtr();
  }
  static DPProcessBaseBodyC::RefT Conv2BaseArrFloat(const DPProcessBodyC<SArray1dC<float>,SArray1dC<float> >::RefT &aProc) {
    return aProc.BodyPtr();
  }

  static DPProcessC<SArray1dC<float>,SArray1dC<float> > Conv2HandleArrFloat(const DPProcessBodyC<SArray1dC<float>,SArray1dC<float> >::RefT &aProc) {
    return DPProcessC<SArray1dC<float>,SArray1dC<float> >(*aProc);
  }

  static DPProcessC<SArray1dC<RealT>,SArray1dC<RealT> > Conv2HandleArrReal(const DPProcessBodyC<SArray1dC<RealT>,SArray1dC<RealT> >::RefT &aProc)
  {
    return DPProcessC<SArray1dC<RealT>,SArray1dC<RealT> >(*aProc);
  }

  DP_REGISTER_CONVERSION(Conv2Base,1);
  DP_REGISTER_CONVERSION(Conv2BaseArrReal,1);
  DP_REGISTER_CONVERSION(Conv2BaseArrFloat,1);
  DP_REGISTER_CONVERSION(Conv2HandleArrFloat,1);
  DP_REGISTER_CONVERSION(Conv2HandleArrReal,1);
}

