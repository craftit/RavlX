// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/RealRange2d.cc"

#include "Ravl/RealRange2d.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Multiply a 2d index range by a real 2d range.
  // Multiplying by a real range of 0-1,0-1 is a unit transform.
  
  IndexRange2dC operator*(const RealRange2dC &realRange,const IndexRange2dC &indexRange) {
    return IndexRange2dC(realRange.Range1() * indexRange.Range1(),
			 realRange.Range2() * indexRange.Range2());
  }
  
  RealRange2dC::RealRange2dC(const RealRangeC & rowRange,
			     const RealRangeC & colRange)
    : rows(rowRange), cols(colRange)
  {}
  
  RealRange2dC::RealRange2dC(const RealRange2dC & range)
    : rows(range.RowRange()), cols(range.ColRange())
  {}

  RealRange2dC::RealRange2dC(const TFVectorC<RealT,2> &center,RealT nrows,RealT ncols) {
    if(nrows > 0)
      rows = RealRangeC(center[0] - nrows/2,center[0] + nrows/2);
    else
      rows = RealRangeC(center[0],center[0]);
    if(ncols > 0)
      cols = RealRangeC(center[1] - ncols/2,center[1] + ncols/2);
    else
      cols = RealRangeC(center[1],center[1]);
  }
  

  //: Rotate rectangle 180 degree's around the given center.
  
  RealRange2dC RealRange2dC::Rotate180(TFVectorC<RealT,2> centre) {
    return RealRange2dC((End() * -1) + (centre*2),(Origin() * -1) + (centre * 2));
  }
  
  std::ostream &operator<<(std::ostream &s,const RealRange2dC &ir) {
    s << ir.Range1() << ' ' << ir.Range2();
    return s;
  }
  
  std::istream &operator>>(std::istream &s,RealRange2dC &ir) {
    s >> ir.Range1() >> ir.Range2();
    return s;
  }
  
  BinOStreamC &operator<<(BinOStreamC &s,const RealRange2dC &ir) {
    s << ir.Range1() << ir.Range2();
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,RealRange2dC &ir) {
    s >> ir.Range1() >> ir.Range2();
    return s;
  }
  
}
