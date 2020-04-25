// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// IAPS - Image analysis program system
//! file="Ravl/Math/Geometry/Projective/3D/PPlane3d.cc"

//! date="26.06.1995"
//! author="Radek Marik"
//! rcsid="$Id$"
//! lib=RavlMath

#include "Ravl/BinStream.hh"
#include "Ravl/PPlane3d.hh"

namespace RavlN {
  
 
  ostream & 
  operator<<(ostream & outS, const PPlane3dC & p)
  {
    outS << p.P1() << ' ' << p.P2() << ' ' << p.P3() << ' ' << p.P4();
    return outS;
  }

  istream & 
  operator>>(istream & inS, PPlane3dC & p)
  {
    inS >> p.P1() >> p.P2() >> p.P3() >> p.P4();
    return inS;
  }

  BinOStreamC & operator<<(BinOStreamC & strm, const PPlane3dC & p)
  {
    strm << p.P1() << p.P2() << p.P3() << p.P4();
    return strm;
  }
  // Writes the 'point' into the output stream.
  
  BinIStreamC & operator>>(BinIStreamC & strm, PPlane3dC & p) 
  {
    strm >> p.P1() >> p.P2() >> p.P3() >> p.P4();
    return strm;
  }
  // Reads the point parameters from the input stream.

}
