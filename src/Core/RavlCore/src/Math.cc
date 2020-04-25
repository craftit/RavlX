// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Math.cc"

#include "Ravl/Math.hh"


namespace RavlN {
  
  // The following is code is based on an implementation 
  // found in the ccmath library.
  
  RealT Pow(RealT y,int n) { 
    RealT s=1.; 
    unsigned m,j; 
    if(!n)
      return s;
    if(n < 0) { 
      m = -n; 
      y = 1. / y;
    } else 
      m = n;
    for(j = 1; j <= m ;j <<= 1) { 
      if(j & m) 
	s *= y; 
      y *= y;
    }
    return s;
  }
  
}
