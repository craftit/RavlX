// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/exFAffine.cc"
//! docentry="Ravl.API.Math.Geometry"
//! author="Charles Galambos"
//! userlevel=Normal

#include "Ravl/FAffine.hh"
#include "Ravl/Stream.hh"

//: Maybe not such a great example

using namespace RavlN;

int main (int argc, char** argv)
{
  FAffineC<2> motion_vector;
  
  cout << motion_vector << "\n";

  return 0;
}



