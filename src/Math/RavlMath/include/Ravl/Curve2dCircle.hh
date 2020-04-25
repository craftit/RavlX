// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CURVE2DARC_HEADER
#define RAVL_CURVE2DARC_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! userlevel=Normal
//! docentry="Ravl.API.Math.Geometry.2D"
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2dCircle.hh"

#include "Ravl/Circle2d.hh"

namespace RavlN {
  class IndexRange2dC;
  
  //: Arc.
  
  class Curve2dCircleC : public Circle2dC {
  public:

    Point2dC Point(RealT p) const
    { return Value(p); }
    //: Localion of curve at p
    
    Vector2dC Tangent(RealT p) const 
    { return Vector2dC(-Sin(p),Cos(p)); }
    //: Tangent to curve at p
    
    Vector2dC UnitTangent(RealT p) const
    { return Vector2dC(-Sin(p),Cos(p)); }
    //: Tangent to curve at p
    
    RealT Closest(const Point2dC &pnt) const
    { return Angle(pnt); }
    //: Parametric value of closest point on curve to Pnt.
    
    void GetLimits(RealT &Low,RealT &High,const IndexRange2dC &Rect) const;
    //: Get limits of paramiter if shape is within Rectangle 'Rect'.
    // NOT IMPLEMENTED.
    
  };
}


#endif
