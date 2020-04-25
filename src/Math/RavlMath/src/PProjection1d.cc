// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! date="26.06.1995"
//! author="Radek Marik"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/1D/PProjection1d.cc"

#include "Ravl/PProjection1d.hh"

namespace RavlN {
  
  PProjection1dC::PProjection1dC(const PPoint1dC & i,
				 const PPoint1dC & o,
				 const PPoint1dC & u)
    //=================================================
    // Ref.: pp. 120-121
    // Ref.: Radek Marik, Maths Notes, 1995
  {
    // Linear solution of multiplicators
    RealT c0 = o.P2()*u.P1() - o.P1()*u.P2();
    RealT c1 = i.P1()*u.P2() - i.P2()*u.P1();
    // Construct the matrix of the projection.
    (*this) = Matrix2dC( c0 * i.P1(), c1 * o.P1(),
			 c0 * i.P2(), c1 * o.P2());
  }
  
  PProjection1dC::PProjection1dC(istream & inS)
  { inS >> *this; }
  
  ostream & 
  operator<<(ostream & outS, const PProjection1dC & p) {
    for (UIntT i = 0; i< 2; ++i)
      for (UIntT j = 0; j < 2; ++j)
	outS << p[i][j] << ' ';
    return outS;
  }
  
  istream & 
  operator>>(istream & inS, PProjection1dC & p) {
    for (UIntT i = 0; i< 2; ++i)
      for (UIntT j = 0; j < 2; ++j)
	inS >> p[i][j];
    return inS;
  }
}
