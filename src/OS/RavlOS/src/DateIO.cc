// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Time/DateIO.cc"

#include "Ravl/OS/Date.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Stream operator.
  
  BinOStreamC &operator <<(BinOStreamC &strm,const DateC &date) {
    strm << ((Int32T) date.TotalSeconds()) << ((Int32T) date.USeconds());
    return strm;
  }
  
  //: Stream operator.
  
  BinIStreamC &operator >>(BinIStreamC &strm,DateC &date) {
    Int32T v1,v2;
    strm >> v1 >> v2;
    date = DateC((long int) v1,(long int)v2);
    return strm;
  }
}
