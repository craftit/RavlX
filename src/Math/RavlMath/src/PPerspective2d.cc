// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/PPerspective2d.cc"

#include "Ravl/PPerspective2d.hh"

namespace RavlN {
  ostream & operator<<(ostream & outS, const PPerspective2dC & p)
  {
    for (UIntT i = 0; i < 3; ++i)
      for (UIntT j = 0; j < 3; ++j)
	outS << p[i][j] << ' ';
    return outS;
  }
  
  istream & operator>>(istream & inS, PPerspective2dC & p)
  {
    for (UIntT i = 0; i < 3; ++i)
      for (UIntT j = 0; j < 3; ++j)
	inS >> p[i][j];
    return inS;
  }
  
}
