// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Sequence/testPolygon2dIter.cc"
//! author="Robert Crida"
//! docentry="Ravl.API.Math.Sequences"
//! userlevel=Develop

#include "Ravl/Polygon2dIter.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int main(int nargs,char **argv) 
{
  Polygon2dC polygon;
  polygon.InsLast(Point2dC(0,0));
  polygon.InsLast(Point2dC(5,7));
  polygon.InsLast(Point2dC(0,10));
  polygon.InsLast(Point2dC(10,10));
  polygon.InsLast(Point2dC(10,0));
  for(Polygon2dIterC it(polygon); it; it++) {
    std::cerr << it.Row() << ' ' << it.IndexRange() << std::endl;
  }
  return 0;
}
