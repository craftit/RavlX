// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2d.cc"

#include "Ravl/Curve2d.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Stream.hh"

#ifndef __sgi__
#include <typeinfo>
#else
#include <typeinfo.h>
#endif

namespace RavlN {
  
  
  Point2dC Curve2dBodyC::Point(RealT /*p*/) const  {
    cerr << "Curve2dC::Point(), ERROR:ABSTRACT METHOD CALLED in " << typeid(*this).name() << "\n";
    RavlAssertMsg(0,"Curve2dBodyC::Point(RealT /*p*/), Abstract method called. \n");  
    return Point2dC(0,0);
  }
  
  ////////////////////////
  // Tangent to curve at p
  // Default attempts to estimate it.
  
  Vector2dC Curve2dBodyC::Tangent(RealT p) const  {
    const RealT SmallNo = 0.000001;
    return Vector2dC(Point(p * (SmallNo + 1)) - Point(p+(SmallNo - 1)));
  }
  
  Vector2dC Curve2dBodyC::UnitTangent(RealT p) const { 
    return Tangent(p).Unit();
  }
  
  ////////////////////////////
  // Parametric value of closest point on curve to Pnt.
  // Attemp to estimate it ??
  
  RealT Curve2dBodyC::Closest(Point2dC /*Pnt*/) const 
  { return 0; }
  
  ////////////////////////////
  // Get limits of paramiter if shape is within Rect.
  
  void Curve2dBodyC::GetLimits(RealT &Low,RealT &High,const IndexRange2dC &) const {
    Low = 0;
    High = 1;
  }
  
  ////////////////////////////
  // Fit the current curve to the pnts.
  
  RealT Curve2dBodyC::Fit(const SArray1dC<Point2dC> &/*Pnts*/)  {
    cerr << "Curve2dBodyC::Fit(),  ERROR: ABSTRACT METHOD CALLED. \n";
    RavlAssert(0);  
    return 0;
  }
  
  ////////////////////////////
  // Write GF file.
  
  bool Curve2dBodyC::WriteGF(ostream &Out) const {
    Out << "Curve2dBodyC::WriteGF(), ABSTRACT METHOD. \n";
    RavlAssert(0);
    return true;
  }
  
  ////////////////////////////
  // Write GF file.
  
  bool Curve2dBodyC::WriteGFHeader(ostream &Out) const  {
    Out << "Curve2dBodyC::WriteGFHeader(), ABSTRACT METHOD. \n";
    RavlAssert(0);
    return true;
  }
  
  ///////////////////////////
  // Print to ostream.
  
  void Curve2dBodyC::Print(ostream &out) const {
    out << "{ Curve2dBodyC::Print(), ABSTRACT METHOD! }";
  }
  
  ///////////////////////////
  // Sends the Curve to output stream.
  
  ostream & operator<<(ostream & outS, const Curve2dC & cv)  {
    cv.Print(outS);
    return outS;
  }
  
}
