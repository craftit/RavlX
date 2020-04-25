// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Base/RCBodyV.cc"

#include "Ravl/RCBodyV.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif

namespace RavlN {
  /// RCBodyVC //////////////////////////////////////////
  
  //: Destructor.
  
  RCBodyVC::~RCBodyVC()
  {}
  
  const char *(*TypeNameMap)(const std::type_info &type) = 0;
  
  //: Save to stream out.
  
  bool RCBodyVC::Save(std::ostream &out) const {
    RavlAssert(TypeNameMap != 0);
    out << TypeNameMap(typeid(*this)) << " ";
    return true;
  }
  
  //: Save to stream out.
  
  bool RCBodyVC::Save(BinOStreamC &out) const {
    RavlAssert(TypeNameMap != 0);
    out << TypeNameMap(typeid(*this));
    return true; 
  }
  
  //: Save to binary stream.
  
  RCBodyVC &RCBodyVC::Copy() const {
    RavlAssert(0);
    return *new RCBodyVC();
  }
  //: Make copy of body.
  
  RCBodyC &RCBodyVC::DeepCopy(UIntT levels) const {
    RavlAssert(0);
    return *new RCBodyVC();
  }
  //: Make copy of body.
  
  std::istream &operator>>(std::istream &strm,RCBodyVC &obj) {
    return strm;
  }
  //: Input virtual body.
  // No-op.
  
  std::ostream &operator<<(std::ostream &strm,const RCBodyVC &obj) {
    return strm;
  }
  //: Output virtual body.
  // No-op.
  
  BinIStreamC &operator>>(BinIStreamC &strm,RCBodyVC &obj) {
    return strm;
  }
  //: Input virtual body.
  // No-op.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RCBodyVC &obj) {
    return strm;
  }
  //: Output virtual body.
  // No-op.

};
