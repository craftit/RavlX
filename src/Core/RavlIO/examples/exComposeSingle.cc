// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/exComposeSingle.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Core.Data Processing.Single Items"
//! author="Charles Galambos"

//: Example of how to use single item processing pipelines.

#include "Ravl/DP/ComposeSingle.hh"
#include "Ravl/IntC.hh"

namespace RavlN {
  IntC Multiply2(const IntC &x) {
    return x * 2;
  }
}

using namespace RavlN;
using namespace RavlComposeSingleN;

int main() {
  
  IntC x = 1;
  IntC y = 0;
  
  x >> Multiply2 >> Multiply2 >> y;
  
  cerr << "X=" << x<< " Y=" << y << "\n";
  return 0;
}
