// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id: LogValueC.cc 1294 2002-06-25 15:17:27Z craftit $"
//! lib=RavlCore
//! file="Ravl/Core/Math/LogValue.cc"

#include "Ravl/LogValue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  std::ostream &operator<<(std::ostream &out,const LogValueC &x) {
    out << x.Log();
    return out;
  }
  
  std::istream &operator>>(std::istream &in,LogValueC &x) {
    RealT v;
    in >> v;
    x = LogValueC(v,true);
    return in;
  }
  
  BinOStreamC &operator<<(BinOStreamC &strm,const LogValueC &x) {
    strm << x.Log();
    return strm;
  }
  
  BinIStreamC &operator>>(BinIStreamC &strm,LogValueC &x) {
    RealT v;
    strm >> v;
    x = LogValueC(v,true);
    return strm;
  }

}
