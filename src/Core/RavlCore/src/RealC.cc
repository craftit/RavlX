// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/RealC.cc"

#include "Ravl/RealC.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  std::ostream &operator<<(std::ostream &out,const RealC &x) {
    out << x.v;
    return out;
  }
  
  std::istream &operator>>(std::istream &in,RealC &x) {
    in >> x.v;
    return in;
  }

  BinOStreamC &operator<<(BinOStreamC &strm,const RealC &x) {
    strm << x.v;
    return strm;
  }
  
  BinIStreamC &operator>>(BinIStreamC &strm,RealC &x) {
    strm >> x.v;
    return strm;
  }
  
}
