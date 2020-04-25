// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/Size.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {


  //! Write to a binary stream
  BinOStreamC &operator<<(BinOStreamC &strm,const SizeC &size) {
    if(strm.CompatibilityMode32Bit()) {
      UInt32T value = size;
      strm << value;
    } else {
      UInt64T value = size;
      strm << value;
    }
    return strm;
  }

  //! Read from a binary stream
  BinIStreamC &operator>>(BinIStreamC &strm,SizeC &size) {
    if(strm.CompatibilityMode32Bit()) {
      UInt32T value;
      strm >> value;
      size = value;
    } else {
      UInt64T value;
      strm >> value;
      if(sizeof(size.V()) == 4) {
        if(value > ((UInt64T)((UInt32T) -1))) {
          throw ExceptionOutOfRangeC("Attempted to load object too large for this machine. ");
        }
      }
      size = static_cast<size_t>(value);
    }
    return strm;
  }

  std::ostream &operator<<(std::ostream &strm,const SizeC &size) {
    strm << size.V();
    return strm;
  }
  //: Write to text stream

  std::istream &operator>>(std::istream &strm,SizeC &size){
    strm >> size.V();
    return strm;
  }
  //: Write to text stream

 }
