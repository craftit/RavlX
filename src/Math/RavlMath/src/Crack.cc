// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! date="26.10.1992"
//! author="Radek Marik"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/Boundary/Crack.cc"

// elementary boundary edge based on crack code

#include "Ravl/Crack.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Point2d.hh"

namespace RavlN {
  

  //: Mid point along crack.
  
  Point2dC CrackC::MidPoint() const {
    switch(crackCode) {
    case CR_DOWN : return Point2dC((RealT) Row() +0.5 ,(RealT) Col());

    case CR_RIGHT: return Point2dC((RealT) Row()    ,(RealT) Col() + 0.5);
    case CR_UP   : return Point2dC((RealT) Row() -0.5 ,(RealT) Col());
    case CR_LEFT : return Point2dC((RealT) Row()      ,(RealT) Col() - 0.5);
    case CR_NODIR: return Point2dC((RealT) Row()  +0.5    ,(RealT) Col() + 0.5);
    }
    RavlAssert(0);
    return Point2dC(0,0);
  }
  

}


  
