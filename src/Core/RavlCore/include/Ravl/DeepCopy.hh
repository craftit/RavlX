// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DEEPCOPY_HEADER
#define RAVL_DEEPCOPY_HEADER 1
//////////////////////////////////////////
//! lib=RavlCore
//! author="Charles Galambos"
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Base/DeepCopy.hh"
//! docentry="Ravl.API.Core.Misc"

#include "Ravl/Types.hh"

namespace RavlN {
  
  inline
  IntT StdDeepCopy(const IntT &val,UIntT levels = ((UIntT) -1)) {
    return val;
  } 
  //: Deep copy for built in type.
  
  inline  
  UIntT StdDeepCopy(const UIntT &val,UIntT levels = ((UIntT) -1)) {
    return val;
  } 
  //: Deep copy for built in type.

  inline
  RealT StdDeepCopy(const RealT &val,UIntT levels = ((UIntT) -1)) {
    return val;
  } 
  //: Deep copy for built in type.

  inline
  FloatT StdDeepCopy(const FloatT &val,UIntT levels = ((UIntT) -1)) {
    return val;
  } 
  //: Deep copy for built in type.

  inline
  ByteT StdDeepCopy(const ByteT &val,UIntT levels = ((UIntT) -1)) {
    return val;
  } 
  //: Deep copy for built in type.
  
  inline
  UInt16T StdDeepCopy(const UInt16T &val,UIntT levels = ((UIntT) -1)) 
  { return val; }
  //: Deep copy for built in type.
  
  inline
  Int16T StdDeepCopy(const Int16T &val,UIntT levels = ((UIntT) -1)) 
  { return val; }
  //: Deep copy for built in type.
  
  template<class DataT>
  inline
  DataT StdDeepCopy(const DataT &val,UIntT levels = ((UIntT) -1)) {
    return val.DeepCopy(levels);
  }
  //: Call deep copy on a class.
  // Copy to a depth of 'levels' 
}

#endif
