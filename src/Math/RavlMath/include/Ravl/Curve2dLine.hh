// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CURVE2DLINE_HEADER
#define RAVL_CURVE2DLINE_HEADER 1
//////////////////////////////////////////////////////
//! docentry="Ravl.API.Math.Geometry.2D"
//! userlevel=Normal
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! date="26/08/1998"
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2dLine.hh"

#include "Ravl/LineABC2d.hh"
#include "Ravl/StdConst.hh"
//#include "Ravl/Angle.hh"

namespace RavlN {
  class IndexRange2dC;
  template<class DataT> class Array1dC;
  
  //: Parametric Line.
  // This class modifies the line equation so it always
  // has a unit normal.
  
  class Curve2dLineC : public LineABC2dC {
  public:
    inline Curve2dLineC()
    {}
    //: Default constructor.

    inline Curve2dLineC(const Array1dC<Point2dC> &points) { 
      RealT res;
      FitLSQ(points,res);  
    }
    //: Make a line that best fits the given points.
    
    inline Curve2dLineC(Point2dC p1,Point2dC p2)
      : LineABC2dC(p1,p2)
    { MakeUnitNormal(); }
    //: Make a line passing through two pixels.
    
    inline Curve2dLineC(const LineABC2dC &Line)
      : LineABC2dC(Line)
    { MakeUnitNormal(); }
    //: From a line.
    
    inline Curve2dLineC(RealT row,RealT theta) {
      theta += RavlConstN::pi/2;
      Vector2dC ang(Cos(theta),Sin(theta)); // Direction of line.
      ((LineABC2dC &) (*this)) = LineABC2dC(Point2dC(row,0),ang);
      MakeUnitNormal();
    }
    //: From Y axis crossing point and angle.

    inline Curve2dLineC(Point2dC mid,Vector2dC dir)
      : LineABC2dC(mid,dir)
    { MakeUnitNormal(); }
    //: From a point and a direction.
    
    inline Curve2dLineC(RealT a,RealT b,RealT c)
      : LineABC2dC(a,b,c)
    {}
    //: From paramiters.
    
    bool FitLSQ(const Array1dC<Point2dC> &points,RealT &residual) {
      bool ret = LineABC2dC::FitLSQ(points,residual);
      MakeUnitNormal();
      return ret;
    }
    //: Fit least squares.
    
    inline RealT Fit(const Array1dC<Point2dC> &pnts) { 
      RealT tmp;
      if(!FitLSQ(pnts,tmp))
	return -1;
      return tmp;
    }
    //: Default fitting method.
    // Returns the residual from the fit.
    
    RealT AssessFit(const Array1dC<Point2dC> &points);
    //: Measure error for fit.
    // Returns the average distance of the points from the line.
    
    RealT Angle() const
    { return Normal().Perpendicular().Angle(); }
    //: Get angle of line.
    
    inline const Curve2dLineC &operator=(const LineABC2dC &It) {
      ((LineABC2dC &) (*this)) = It;
      MakeUnitNormal();
      return *this;
    }
    //: Assign from a plain LineABC.
    
    inline Vector2dC Tangent(RealT /*p*/) const 
    { return Normal().Perpendicular(); }
    //: Tangent to curve at p
    
    inline Vector2dC UnitTangent(RealT /*p*/) const 
    { return Normal().Perpendicular(); }
    //: Tangent to curve at p
    
    inline Vector2dC UnitNormal() const
    { return Normal(); }
    //: Get the unit normal.
    
    inline Point2dC Point(RealT p) const 
    { return (Normal().Perpendicular()*p)-(Normal()*C()); }
    //: Localion of curve at p
    
    inline RealT Closest(Point2dC Pnt) const;
    //: Parametric value of closest point on curve to Pnt.
    
    void GetLimits(RealT &Low,RealT &High,const IndexRange2dC &Rect) const;
    //: Get limits of paramiter if shape is within Rect.
    
    inline const char *GetName() const { return "Curve2dLine"; }
    //: Name of primitive.
    
    inline IntT CodeSize() const { return 30; }
    //: Estimated bits to code primitive.
    
    bool WriteGF(ostream &Out) const;
    //: Write GF file.
    
    static bool WriteGFHeader(ostream &Out);
    //: Write GF file.  
    
    void Print(ostream &out) const;
    //: Print to ostream.
  };
  
  ostream & operator<<(ostream & outS, const Curve2dLineC & index);
  // Sends the Curve2dLineC into the output stream.
  
  inline RealT Curve2dLineC::Closest(Point2dC Pnt) const { 
    // FIXME :- Could this be simplified ??
    Point2dC Where = Projection(Pnt); // Get a point actualy on the line.
    if(Abs(B()) > Abs(A()))
      return ((Where[0] + (A() * C())) / -B());
    return ((Where[1] + (B() * C())) / A());  
  }
  
}


#endif
