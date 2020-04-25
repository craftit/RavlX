// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////
//! rcsid="$Id: Option.cc 5431 2006-03-28 17:28:31Z ees1wc $"
//! lib=RavlCore
//! file="Ravl/Core/System/STL.cc"

#include "Ravl/STL.hh"
#include "Ravl/Math.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  /// Write stl string to binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const std::string &str) {
    UIntT strSize = str.size();
    strm << strSize;
    strm.OBuff(str.data(),strSize);
    return strm; 
  }
  
  /// Read stl string from binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,std::string &str) {
    UIntT strSize;
    strm >> strSize;
    if(strSize > strm.ArraySizeLimit())
      throw ExceptionOutOfRangeC("Incoming string size exceeds limit for stream.");
    str.clear();
    if(strSize > 0) {
      str.resize(strSize);
      strm.IBuff(&(str[0]),strSize);
    }
    return strm; 
  }
  
}
