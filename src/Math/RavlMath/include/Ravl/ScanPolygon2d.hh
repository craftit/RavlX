// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SCANPOLYGON2D_HEADER
#define RAVL_SCANPOLYGON2D_HEADER 1
//! lib=RavlMath
//! date="16/8/2004"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Geometry.2D"
//! file="Ravl/Math/Geometry/Euclidean/2D/ScanPolygon2d.hh"

#include "Ravl/Polygon2d.hh"
#include "Ravl/RealRange1d.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  
  
  //! userlevel=Develop
  //: Scans a polygon in the manner of a set of raster scan lines.
  // This assumes the boundary does not cross itself, is
  // oriented clockwise (in the Ravl coordinate system) and
  // contains no repeated points.
  // <br>
  // Note: This class is still in development, use with care.
  
  class ScanPolygon2dC {
  public:
    ScanPolygon2dC()
    {}
    //: Default constructor.
    
    ScanPolygon2dC(const Polygon2dC &poly,RealT rowStep = 1.0)
    { First(poly,rowStep); }
    //: Default constructor.
    //!param: rowStep - spacing between scan rows
    
    bool First(const Polygon2dC &poly,RealT rowStep = 1.0);
    //: Goto first polygon in scan.
    
    bool Next();
    //: Goto next scan element.
    
    const RealRangeC &Data() 
    { return m_span; }
    //: Current range 
    // The end points of the RealRangeC are the column coords where the current raster scan line cuts the polygon
    
    bool operator++(int)
    { return Next(); }
    //: Goto next scan row.
    
    bool IsElm() const
    { return m_spans.IsElm(); }
    //: Are we on a valid element ?
    
    operator bool() const
    { return IsElm(); }
    //: Are we on a valid element ?
    
    RealT Row() const
    { return m_row; }
    //: Access current scan row coordinate.
    
    class LineSegmentC {
    public:
      LineSegmentC(const Point2dC *_p1,const Point2dC *_p2);
      //: Constructor.
      
      LineSegmentC()
      {}
      //: Default constructor.
      
      const Point2dC *P1() const
      { return m_p1; }
      
      const Point2dC *P2() const
      { return m_p2; }

      void SetP2(const Point2dC *p2)
      { m_p2 = p2; }

      void SetP1(const Point2dC *p1)
      { m_p1 = p1; }

      bool IntersectRow(RealT row,RealT &col) const;
      //: Get column of intersection with row.
      
      RealT IntersectRow(RealT row) const;
      //: Get row intersection with no checking.

      RealT Height() const
      { return m_p2->Row() - m_p1->Row(); }
      //: Get height of span.
    protected:
      const Point2dC *m_p1;
      const Point2dC *m_p2;
    };
    
  protected:
    const Point2dC *NextPnt(const Point2dC *pnt);
    //: Get following point on polygon
    
    const Point2dC *PrevPnt(const Point2dC *pnt);
    //: Get previous point on polygon
    
    RealT RowQuant(RealT row);
    //: Quantise row position
    
    bool SetupSpan();
    //: Setup current 'span'
    
    bool CheckSpans();
    
    
    RealT m_row;
    RealT m_rowStep;
    RealT m_rowLast;
    RealRangeC m_span;
    DLIterC<PairC<LineSegmentC> > m_spans; // Current set of spans.
    const Point2dC *m_firstPnt,*m_lastPnt;
    SArray1dC<Point2dC *> m_ptrs;
    size_t m_pat;
    SArray1dC<Point2dC> m_points;
  };
  
}


#endif
