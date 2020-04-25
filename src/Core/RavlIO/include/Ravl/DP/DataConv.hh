// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPDATACONV_HEADER
#define RAVL_DPDATACONV_HEADER 1
/////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Type Converter"
//! lib=RavlIO
//! rcsid="$Id$"
//! file="Ravl/Core/IO/DataConv.hh"
//! userlevel=Normal
//! author="Charles Galambos"
//! date="17/08/1998"

#include "Ravl/RCWrap.hh"
#include "Ravl/RCAbstract.hh"

namespace RavlN {
  extern bool DPCanConvert(const std::type_info &from,const std::type_info &to);
  //! userlevel=Normal
  //: Test if conversion is possible.
  
  extern RCAbstractC DPDoConversion(const RCAbstractC &dat,const std::type_info &from,const std::type_info &to);
  //! userlevel=Normal
  //: Do conversion through abstract handles.
  
  template<class InT,class OutT>
  bool DPTypeConvert(const InT &inraw,OutT &outraw) {
    RCAbstractC result = DPDoConversion(RCWrapC<InT>(inraw).Abstract(),typeid(InT),
					typeid(OutT));
    RCWrapC<OutT> out(result,true);
    if(!out.IsValid())
      return false; // Did conversion succeed ?
    outraw = out.Data();
    return true;
  }
  //! userlevel=Normal
  //: Convert between two types using the type conversion graph.
  // This is an example. Its not very useful since if both types are known at
  // compile time the necessary methods can be invoked directly which is much faster.
  
  template<class OutT>
  bool DPTypeConvert(const RCWrapAbstractC &in,OutT &outraw) {
    RCAbstractC result = DPDoConversion(const_cast<RCWrapAbstractC &>(in).Abstract(),in.DataType(),
					typeid(OutT));
    RCWrapC<OutT> out(result,true);
    if(!out.IsValid())
      return false; // Did conversion succeed ?
    outraw = out.Data();
    return true;
  }
  //! userlevel=Normal
  //: Convert between a abstract handle to an object and a known type.
}



#endif
