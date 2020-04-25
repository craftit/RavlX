// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/Designer.cc"

#include "Ravl/PatternRec/Designer.hh"

namespace RavlN {

  //: Default constructor.

  DesignerBodyC::DesignerBodyC()
   : m_verbose(false)
  {}

  DesignerBodyC::DesignerBodyC(const XMLFactoryContextC & factory)
    : m_verbose(factory.AttributeBool("verbose",false))
  {}

  //: Load from stream.
  
  DesignerBodyC::DesignerBodyC(std::istream &strm)
    : RCBodyVC(strm),
      m_verbose(false)
  {}
  
  //: Load from binary stream.
  
  DesignerBodyC::DesignerBodyC(BinIStreamC &strm)
    : RCBodyVC(strm),
      m_verbose(false)
  {}
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignerBodyC::Save (std::ostream &out) const 
  { return RCBodyVC::Save(out); }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignerBodyC::Save (BinOStreamC &out) const 
  { return RCBodyVC::Save(out); }
  
  //: Get the current parameters.
  VectorC DesignerBodyC::Parameters() const {
    RavlAssertMsg(0,"DesignerBodyC::Parameters(), Abstract method called. \n");
    return VectorC();
  }
  
  //: Get the current parameters.
  void DesignerBodyC::ParameterLimits(VectorC &defaultValues,VectorC &min,VectorC &max,SArray1dC<StringC> &names) const
  {
    names = SArray1dC<StringC>();
    defaultValues = VectorC();
    min = VectorC();
    max = VectorC();
  }

  VectorC DesignerBodyC::SetParameters(const VectorC &params) {
    RavlAssertMsg(0,"DesignerBodyC::Parameters(const VectorC &), Abstract method called. \n");
    return params;
  }
  //: Set the current parameters.
  // Returns the current parameters, which may not be exactly those
  // set in 'params', but will be the closest legal values.

  // Reset designer to initial state
  bool DesignerBodyC::Reset() {
    RavlAssertMsg(0,"DesignerBodyC::Reset(), Abstract method called. \n");
    return false;
  }


  ////////////////////////////////////////////////////////////////////
  
  //: Load from stream.
  
  DesignerC::DesignerC(std::istream &strm)
    : RCHandleVC<DesignerBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,DesignerBodyC))
  {}
  
  //: Load from binary stream.
  
  DesignerC::DesignerC(BinIStreamC &strm)
    : RCHandleVC<DesignerBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,DesignerBodyC))
  {}

  
}
