// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/PPointLine2d.cc"

#include "Ravl/PPointLine2d.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {
  ostream & operator<<(ostream & outS, const PPointLine2dC & p) {
    outS << p.P1() << ' ' << p.P2() << ' ' << p.P3();
    return outS;
  }
  
  istream & operator>>(istream & inS, PPointLine2dC & p) {
    inS >> p.P1() >> p.P2() >> p.P3();
    return inS;
  }
}
