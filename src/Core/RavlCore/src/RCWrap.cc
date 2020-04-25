// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/Base/RCWrap.cc"

#include "Ravl/Assert.hh"
#include "Ravl/RCWrap.hh"

//: Ravl library namespace.

namespace RavlN {  

  //: Save to text stream.
  
  bool RCWrapBaseBodyC::Save(std::ostream &strm) const 
  { return RCBodyVC::Save(strm); }
    
  //: Save to binary stream.
  
  bool RCWrapBaseBodyC::Save(BinOStreamC &strm) const 
  { return RCBodyVC::Save(strm); }
  
  const std::type_info &RCWrapBaseBodyC::DataType() const {
    AssertFailed(__FILE__,__LINE__,"Abstract method called: WrapBaseBodyC::DataType() ");
    return typeid(void); 
  }
  //: Get type of wrapped object.
  
}
