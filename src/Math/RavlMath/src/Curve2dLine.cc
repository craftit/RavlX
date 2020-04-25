// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2dLine.cc"

#include "Ravl/Curve2dLine.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Stream.hh"

//#include "Ravl/LinearS2.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  void Curve2dLineC::GetLimits(RealT &Low,RealT &High,const IndexRange2dC &Rect) const {
    Vector2dC unit = Tangent(0);
    RealT ux = fabs(unit[0]);
    RealT uy = fabs(unit[1]);
    RealT x1,y1,x2,y2;
    if(ux > uy) {
      x1 = Rect.Origin().Row();
      y1 = ValueY(x1);
      x2 = Rect.End().Row();
      y2 = ValueY(x2);
    } else {
      y1 = Rect.Origin().Col();
      x1 = ValueX(y1);
      y2 = Rect.End().Col();
      x2 = ValueX(y2);    
    }
    Low = Closest(Point2dC(x1,y1));
    High = Closest(Point2dC(x2,y2));
    if(Low > High) {
      RealT Tmp = Low;
      Low = High;
      High = Tmp;
    }
  }
  
  ///////////////////////
  // Write GF file.
  
  bool Curve2dLineC::WriteGF(ostream &Out) const {
    // Write end points of line
    Out << (*this);
    return true;
  }
  
  ///////////////////////
  // Write GF file header.  
  
  bool Curve2dLineC::WriteGFHeader(ostream &Out) {
    Out << "lineABC ";
    return true;
  }
  
  ///////////////////////
  // Print to ostream.
  
  void Curve2dLineC::Print(ostream &out) const {
    out << "LineABC:" << ((const LineABC2dC &) *this);
  }
  
  ///////////////////////
  // Write line out to stream
  
  ostream & operator<<(ostream & outS, const Curve2dLineC & line) {
    outS << ((const LineABC2dC &) line);
    return outS;
  }
  
}
