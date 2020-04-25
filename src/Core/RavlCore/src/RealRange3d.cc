// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/RealRange3d.cc"

#include "Ravl/RealRange3d.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/IndexRange3d.hh"

namespace RavlN {
  
  RealRange3dC::RealRange3dC(const RealRangeC & iRange,
			     const RealRangeC & jRange,
			     const RealRangeC & kRange)
    : is(iRange), 
      js(jRange),
      ks(kRange)
  {}
  
  //: Multiply a 2d index range by a real 2d range.
  // Multiplying by a real range of 0-1,0-1 is a unit transform.
  
  IndexRange3dC operator*(const RealRange3dC &realRange,const IndexRange3dC &indexRange) {
    return IndexRange3dC(realRange.Range1() * indexRange.Range1(),
			 realRange.Range2() * indexRange.Range2(),
			 realRange.Range3() * indexRange.Range3());
  }

  std::ostream &operator<<(std::ostream &s,const RealRange3dC &ir) {
    s << ir.Range1() << ' ' << ir.Range2() << ' ' << ir.Range3();
    return s;
  }
  
  std::istream &operator>>(std::istream &s,RealRange3dC &ir) {
    s >> ir.Range1() >> ir.Range2() >> ir.Range3();
    return s;
  }
  
  BinOStreamC &operator<<(BinOStreamC &s,const RealRange3dC &ir) {
    s << ir.Range1() << ir.Range2() << ir.Range3();
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,RealRange3dC &ir) {
    s >> ir.Range1() >> ir.Range2() >> ir.Range3();
    return s;
  }
  
}
