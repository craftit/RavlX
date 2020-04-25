// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/VertexColourByteRGB.cc"

#include "Ravl/BinStream.hh"
#include "Ravl/3D/VertexColourByteRGB.hh"

namespace Ravl3DN 
{  
  ostream &operator<<(ostream &s, const VertexColourByteRGBC& v) { 
    s << v.Position() << ' ' << v.Normal() << ' ' << v.Colour();
    return s;
  }

  istream &operator>>(istream &s,VertexColourByteRGBC &v) { 
    s >> v.Position() >> v.Normal() >> v.Colour();
    return s;
  }
  
  BinOStreamC &operator<<(BinOStreamC &s,const VertexColourByteRGBC &v) {
    s << static_cast<const VertexC &>(v) << v.Colour();
    return s;
  }
  
  BinIStreamC &operator>>(BinIStreamC &s,VertexColourByteRGBC &v) {
    s >> static_cast<VertexC &>(v) >> v.Colour();
    return s;
  }

}
