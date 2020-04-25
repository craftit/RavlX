// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Corners/Corner.cc"

#include "Ravl/Image/Corner.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"

namespace RavlImageN {
  
  ostream &operator<<(ostream &out,const CornerC &corn) {
    out << corn.Location() << ' ' << corn.Gradient() << ' ' << corn.Level();
    return out;
  }

  istream &operator>>(istream &in,CornerC &corn) {
    in >> corn.Location() >>  corn.Gradient() >> corn.Level();
    return in;
  }

  //: Write corner to a binary stream.
  
  BinOStreamC &operator<<(BinOStreamC &out,const CornerC &corn) {
    out << corn.Location() << corn.Gradient() << corn.Level();
    return out;
  }
  
  //: Read corner from a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &in,CornerC &corn){
    in >> corn.Location() >>  corn.Gradient() >> corn.Level();
    return in;
  }

}
