// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PCLINESEG_HEADER
#define RAVL_PCLINESEG_HEADER 1
//////////////////////////////////////////////////////
//! docentry="Ravl.API.Math.Geometry.2D"
//! userlevel=Normal
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! date="26/08/1998"
//! file="Ravl/Math/Geometry/Euclidean/2D/Curve2dLineSegment.hh"

#include "Ravl/Curve2dLine.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  //: Parametric line segement.
  
  class Curve2dLineSegmentC 
    : public Curve2dLineC
  {
  public:
    inline Curve2dLineSegmentC()
    {}
    //: Default constructor.
    
    inline Curve2dLineSegmentC(const Array1dC<Point2dC> &pnts)
    { Fit(pnts); }
    //: Make a line that best fits the given points.
    
    inline Curve2dLineSegmentC(const Curve2dLineSegmentC &Oth);
    //: Copy constructor.
    
    inline Curve2dLineSegmentC(const LineABC2dC &Line,Point2dC Start,Point2dC End);
    //: From a line.
    
    inline Curve2dLineSegmentC(Point2dC Start,Point2dC End);
    //: From a pair of points.
    
    RealT FitLSQ(const Array1dC<Point2dC> &pnts,RealT &residual);
    //: Make a line that fits points.
    
    inline RealT Fit(const Array1dC<Point2dC> &pnts) { 
      RealT tmp;
      return FitLSQ(pnts,tmp); 
    }
    //: Default fitting method.
    
    inline RealT Start() const { return Ends[0]; }
    //: Get paramiter for start of line.
    
    inline RealT End() const { return Ends[1]; }
    //: Get paramiter for end of line.
    
    inline RealT &Start() { return Ends[0]; }
    //: Get paramiter for start of line.
    
    inline RealT &End() { return Ends[1]; }
    //: Get paramiter for end of line.
    
    inline Point2dC StartPnt() const { return Point(Start()); }
    //: Get start point of line.
    
    inline Point2dC EndPnt() const { return Point(End()); }
    //: Get end point of line.
    
    inline Point2dC MidPnt() const { return (StartPnt() + EndPnt())/2; }
    //: Mid point of segment.
  
    inline RealT Length() const { return StartPnt().EuclidDistance(EndPnt()); }
    //: Length of segment.
    
    bool WriteGF(ostream &Out) const;
    //: Write GF file.
    
    RealT Compair(Curve2dLineSegmentC &oth);
    //: This is designed to be used to match lines.
    // It returns the sum of the distances between the
    // closest end points.
    
    static bool WriteGFHeader(ostream &Out);
    //: Write GF file.  
    
    Point2dC PointOnLine( RealT offset );
    //: Get a point on the list a distance 'offset' from 'StartPnt.
    // End is in the positive direction and the distance is in the 
    // same units as the co-oridinate system (unlike the Point() method.)
    
  private:
    RealT Ends[2];
  };
  
  ostream &operator<<(ostream &strm,const Curve2dLineSegmentC &dat);
  //: Stream output.
  
  istream &operator>>(istream &strm,Curve2dLineSegmentC &dat);
  //: Stream input.

  ////////////////////////////////////////////////
  
  inline Curve2dLineSegmentC::Curve2dLineSegmentC(const Curve2dLineSegmentC &Oth) 
    : Curve2dLineC(Oth)
  {
    Ends[0] = Oth.Ends[0];
    Ends[1] = Oth.Ends[1];
  }
  
  inline Curve2dLineSegmentC::Curve2dLineSegmentC(const LineABC2dC &Line,Point2dC Start,Point2dC End) 
    : Curve2dLineC(Line)
  {
    Ends[0] = Closest(Start);
    Ends[1] = Closest(End);
  }
  
  inline 
  Curve2dLineSegmentC::Curve2dLineSegmentC(Point2dC Start,Point2dC End)
    : Curve2dLineC(Start,End)
  {
    Ends[0] = Closest(Start);
    Ends[1] = Closest(End);
  }

}


#endif
