// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=Ravl3D
//! file="Ravl/3D/Mesh/Vertex.cc"

#include "Ravl/3D/Vertex.hh"

namespace Ravl3DN {
  
  ostream &operator<<(ostream &s,const VertexC &v) { 
    s << v.Position() << ' ' << v.Normal();
    return s;
  }

  istream &operator>>(istream &s,VertexC &v) { 
    s >> v.Position() >> v.Normal();
    return s;
  }

}
