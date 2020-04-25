// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/RealHist2dBinaryIO.cc"

#include "Ravl/RealHistogram2d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  BinOStreamC &operator<<(BinOStreamC & strm,const RealHistogram2dC &hist)  {
    strm << hist.Offset() << hist.Scale() << (const SArray2dC<UIntT> &)(hist);
    return strm;
  }
  
  BinIStreamC &operator>>(BinIStreamC & strm,RealHistogram2dC &hist) {
    Point2dC offset;
    Vector2dC scale;
    SArray2dC<UIntT> xhist;
    strm >> offset >> scale >> xhist;
    hist = RealHistogram2dC(scale,offset,xhist,false);
    return strm;
  }
  
}
