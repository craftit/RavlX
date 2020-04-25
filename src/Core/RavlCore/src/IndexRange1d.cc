// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/IndexRange1d.cc"

#include "Ravl/IndexRange1d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  IndexRangeC::IndexRangeC(std::istream & s)
  { s >> minI >> maxI; }
  
  std::istream & 
  operator>>(std::istream & sss, IndexRangeC & range) { 
    sss >> range.Min() >> range.Max();
    return  sss;
  }
  
  std::ostream & 
  operator<<(std::ostream & s, const IndexRangeC & range) { 
    s << range.Min() << ' ' << range.Max();
    return s;
  }

  BinOStreamC &operator<<(BinOStreamC &s,const IndexRangeC &range) {
    s << range.Min() << range.Max();
    return  s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,IndexRangeC &range) {
    s >> range.Min() >> range.Max();
    return s;
  }
  

}



