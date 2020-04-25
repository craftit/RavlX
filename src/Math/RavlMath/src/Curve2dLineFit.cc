// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2dLineFit.cc"

#include "Ravl/Math.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Curve2dLine.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Array1d.hh"

namespace RavlN {
  
  // Measure error for fit.
  
  RealT Curve2dLineC::AssessFit(const Array1dC<Point2dC> &pnts) {
    RealT Err = 0;
    for(BufferAccessIterC<Point2dC> it(pnts);it;it++)
      Err += Distance(*it);
    return Err/pnts.Size();
  }
}
