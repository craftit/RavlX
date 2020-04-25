// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/BinStream.cc"

#include "Ravl/BinStream.hh"

namespace RavlN {

  // Default to old RAVL behaviour. 32 bit on 32 bit machines. 64 on 64.
#if RAVL_CPUTYPE_32
  static bool g_compatibilityMode32bit = true;
#else
  static bool g_compatibilityMode32bit = false;
#endif

  bool DefaultToCompatibilityMode32Bit() {
    return g_compatibilityMode32bit;
  }

  void SetCompatibilityMode32Bit(bool activate) {
    g_compatibilityMode32bit = activate;
  }

  //: Destructor.

  BinOStreamC::~BinOStreamC()
  {}

  //: Destructor.

  BinIStreamC::~BinIStreamC()
  {}

  //: Character stream.

  BinOStreamC &BinOStreamC::operator << (const char *text) {
    UInt32T len = 0;
    if(text == 0) {// Check for null string just in case.
      (*this) << len;
      return (*this);
    }
    const char *place;
    for(place = text;*place != 0;place++,len++) ;
    (*this) << len;
    OBuff(text,len);
    return (*this);
  }

  BinOStreamC &operator<<(BinOStreamC &strm,const IndexC &ind)  {
    if(strm.CompatibilityMode32Bit()) {
      Int32T value = ind.V();
      strm << value;
    } else {
      Int64T value = ind.V();
      strm << value;
    }
    return strm;
  }

  static const UIntT g_maxUInt = (UIntT) -1;

  BinIStreamC &operator>>(BinIStreamC &strm,IndexC &ind)  {
    if(strm.CompatibilityMode32Bit()) {
      Int32T value;
      strm >> value;
      ind = value;
    } else {
      Int64T value;
      strm >> value;
      if(sizeof(ind.V()) == 4) {
        if(value > ((Int64T)g_maxUInt)) {
          throw ExceptionOutOfRangeC("Attempted to load object too large for this machine. ");
        }
      }
      ind = IndexC(value);
    }
    return strm;
  }


}
