// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/PLinePP2d.cc"

#include "Ravl/LinePP2d.hh"
#include "Ravl/PPoint2d.hh"
#include "Ravl/PLine2d.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/IndexRange2d.hh"

namespace RavlN {
  
  PLine2dC::PLine2dC(const LinePP2dC &l, RealT s)
    : PPointLine2dC(PPoint2dC(l.FirstPoint(), s),PPoint2dC(l.SecondPoint(), s))
  {}
  

  bool PLine2dC::ClipBy(LinePP2dC& PPLine, const RealRange2dC& Rectangle)
  {
    IntT j = 0;
    LinePP2dC lineSeg;
    PPoint2dC x;
    
    x = this->Intersection(PLine2dC(1,0,-Rectangle.TRow()));
    if (!x.IsIdeal()) {
      Point2dC p(x.Point2d());
      if (Rectangle.ColRange().Contains(p[1])) PPLine[j++] = p;
      if (j >= 2) {
        if (PPLine[0] != PPLine[1])  return true;
        else --j;
      }
    }
    x = this->Intersection(PLine2dC(1,0,-Rectangle.BRow()));
    if (!x.IsIdeal()) {
      Point2dC p(x.Point2d());
      if (Rectangle.ColRange().Contains(p[1])) PPLine[j++] = p;
      if (j >= 2) {
        if (PPLine[0] != PPLine[1])  return true;
        else --j;
      }
    }
    x = this->Intersection(PLine2dC(0,1,-Rectangle.LCol()));
    if (!x.IsIdeal()) {
      Point2dC p(x.Point2d());
      if (Rectangle.RowRange().Contains(p[0])) PPLine[j++] = p;
      if (j >= 2) {
        if (PPLine[0] != PPLine[1])  return true;
        else --j;
      }
    }
    x = this->Intersection(PLine2dC(0,1,-Rectangle.RCol()));
    if (!x.IsIdeal()) {
      Point2dC p(x.Point2d());
      if (Rectangle.RowRange().Contains(p[0])) PPLine[j++] = p;
      if (j >= 2) {
        if (PPLine[0] != PPLine[1])  return true;
        else --j;
      }
    }

    using namespace RavlConstN;
    PPLine = LinePP2dC(Point2dC(nanReal,nanReal),Point2dC(nanReal,nanReal));
    return false;
  }

  bool PLine2dC::ClipBy(LinePP2dC& PPLine, const IndexRange2dC& Rect)
  {
    return this->ClipBy(PPLine, RealRange2dC(Rect.TRow(),Rect.BRow(),Rect.LCol(),Rect.RCol()));
  }

  
}
