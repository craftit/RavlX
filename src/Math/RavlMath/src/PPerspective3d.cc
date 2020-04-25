// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! file="Ravl/Math/Geometry/Projective/3D/PPerspective3d.cc"

//! author="Radek Marik"
//! date="26.06.1995"
//! rcsid="$Id$"
//! lib=RavlMath

#include "Ravl/PPerspective3d.hh"

namespace RavlN {
  
  PPerspective3dC::PPerspective3dC(const PPoint3dC & e, const PPlane3dC & i)
    : PProjection3dC(  i.P2()*e.P2() + i.P3()*e.P3() + i.P4()*e.P4(),
		       -i.P2()*e.P1(),
		       -i.P3()*e.P1(),
		       -i.P4()*e.P1(),

		       -i.P1()*e.P2(),
		       i.P1()*e.P1() + i.P3()*e.P3() + i.P4()*e.P4(),
		       -i.P3()*e.P2(),
		       -i.P4()*e.P2(),

		       -i.P1()*e.P3(),
		       -i.P2()*e.P3(),
		       i.P1()*e.P1() + i.P2()*e.P2() + i.P4()*e.P4(),
		       -i.P4()*e.P3(),

		       -i.P1()*e.P4(),
		       -i.P2()*e.P4(),
		       -i.P3()*e.P4(),
		       i.P1()*e.P1() + i.P2()*e.P2() + i.P3()*e.P3())
  {}
  
  ostream & operator<<(ostream & outS, const PPerspective3dC & p)
  {
    for (UIntT i = 0; i < 4; ++i) {
      for (IndexC j = 0; j < 4; ++j)
	outS << ' ' << p[i][j];
      outS << '\n';
    }
    return outS;
  }

  istream & operator>>(istream & inS, PPerspective3dC & p) {
    for (UIntT i = 0; i < 4; ++i)
      for (UIntT j = 0; j < 4; ++j)
	inS >> p[i][j];
    return inS;
  }
}
