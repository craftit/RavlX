// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: RealHist2dBinaryIO.cc 4354 2004-06-08 16:07:51Z craftit $"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/RealHist2dBinaryIO.cc"

#include "Ravl/RealHistogram3d.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  BinOStreamC &operator<<(BinOStreamC & strm,const RealHistogram3dC &hist)  {
    strm << hist.Offset() << hist.Scale() << (const SArray3dC<UIntT> &)(hist);
    return strm;
  }
  
  BinIStreamC &operator>>(BinIStreamC & strm,RealHistogram3dC &hist) {
    Point3dC offset;
    Vector3dC scale;
    SArray3dC<UIntT> xhist;
    strm >> offset >> scale >> xhist;
    hist = RealHistogram3dC(scale,offset,xhist);
    return strm;
  }
  
}
