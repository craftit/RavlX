// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! date="26.04.1995"
//! author="Radek Marik"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/1D/PPoint1d.cc"

#include "Ravl/PPoint1d.hh"
#include "Ravl/Stream.hh"

namespace RavlN {
  
  ostream &operator<<(ostream & outS, const PPoint1dC & p) {
    outS << p.P1() << ' ' << p.P2();
    return outS;
  }
  
  istream &operator>>(istream & inS, PPoint1dC & p) {
    inS >> p.P1() >> p.P2();
    return inS;
  }
  

}
