// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/LineABC2d.cc"

#include "Ravl/LineABC2d.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  ostream & operator<<(ostream & outS, const LineABC2dC & line) {
    outS << line.Normal() << ' ' << line.C();
    return(outS);
  }
  
  istream & operator>>(istream & inS, LineABC2dC & line) {
    inS >> line.normal >> line.d;
    return(inS);
  }
}

