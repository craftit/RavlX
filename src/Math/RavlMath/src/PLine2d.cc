// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/PLine2d.cc"

#include "Ravl/PPoint2d.hh"
#include "Ravl/PLine2d.hh"

namespace RavlN {

  Vector2dC  PLine2dC::Normal() const
  { return Vector2dC(P1(), P2()); }

  PPoint2dC PLine2dC::Intersection(const PLine2dC & l) const
  { return PPoint2dC(*this, l); }
  
  RealT PLine2dC::Distance(const PPoint2dC &p) const { 
    return RavlN::Abs(Dot(p)) / 
      Sqrt(Sqr((*this)[0]) + Sqr((*this)[1])); 
  }
  
  ostream & operator<<(ostream & outS, const PLine2dC & p) {
    outS << p.P1() << ' ' << p.P2() << ' ' << p.P3();
    return outS;
  }

  istream & operator>>(istream & inS, PLine2dC & p) {
    inS >> p.P1() >> p.P2() >> p.P3();
    return inS;
  }


}
