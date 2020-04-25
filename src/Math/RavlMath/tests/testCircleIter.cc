// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Sequence/testCircleIter.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Sequences"
//! userlevel=Develop

#include "Ravl/CircleIter.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int main(int nargs,char **argv) 
{
  int i = 0;
  int rad = 20;
  RealT maxDist = 0;
  Point2dC origin(0,0);
  for(CircleIterC it(rad);it.IsElm();it.Next(),i++) {
    RealT dst = Point2dC(it.Data()).EuclidDistance(origin);
    RealT diff = Abs((RealT) rad - dst);
    //cerr << "Dst:" << Abs((RealT) rad - dst) << " ";
    if(diff > 0.5) { // Should never be greater than 0.5 !
      std::cerr << "ERROR: Excessive distance from expected position. " << dst << "\n";
      return 1;
    }
    if(diff > maxDist)
      maxDist = diff;
    //cout << it.Data() << std::endl;
  }
  std::cout << "Points:" << i << " Largest error:" << maxDist << std::endl;
  if(i != 112)
    return 2;
  return 0;
}
