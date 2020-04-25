// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POLYGON2DITER_HEADER
#define RAVL_POLYGON2DITER_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Sequence/Polygon2dIter.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Robert Crida"
//! docentry="Ravl.API.Math.Sequences"
//! date="08/02/1999"

#include "Ravl/Polygon2d.hh"

namespace RavlN {
  //: Iterate across the interior of a polygon on an index grid
  // NB. This performs a scanline conversion of the polygon
  
  class Polygon2dIterC {
    class EdgeC {
    public:
      EdgeC() {}
      EdgeC(const Point2dC &p1, const Point2dC &p2);
      RealT xof(RealT row) const { return m_a * row + m_b; }
      IndexC MinRow() const { return m_minRow; }
      IndexC MaxRow() const { return m_maxRow; }
    private:
      IndexC m_minRow,m_maxRow; //end point with smallest y coord
      RealT m_a,m_b; //line is x = a*y + b;
    };

    class IELC: public DListC<EdgeC> {
    public:
      IELC() :m_minRow(RavlConstN::maxInt), m_maxRow(RavlConstN::minInt) {}
      void Add(const EdgeC &e);
      bool Next(const IndexC &row, EdgeC &e);
      IndexC MinRow() { return m_minRow; }
      IndexC MaxRow() { return m_maxRow; }
    private:
      IndexC m_minRow;
      IndexC m_maxRow;
    };

    class AELC: public DListC<EdgeC> {
    public:
      void Add(const EdgeC &e, const IndexC &row);
      void DeleteEdges(const IndexC &row);
      bool First(IndexRangeC &indexRange, const IndexC &row);
      bool Next(IndexRangeC &indexRange, const IndexC &row);
    private:
      DListC<RealT> m_sortedEdges;
    };
  public:
    inline Polygon2dIterC() :m_valid(false) {}
    //: Default constructor does not create a valid iterator!

    inline Polygon2dIterC(const Polygon2dC &polygon)
      : m_polygon(polygon)
    { First(); }
    //: Constructor.
    
    void First();
    //: Goto first scan line in the polygon.
    
    inline bool IsElm() const { return m_valid; }
    //: At valid position ?

    operator bool() const
    { return IsElm(); }
    //: At a valid position ?

    inline const IndexC &Row() const { return m_row; }
    //: Get the current row for the scanline
    
    inline const IndexRangeC &IndexRange() const { return m_indexRange; }
    //: Get point.
    // Largest error from radius should be less than 0.5
    
    bool Next();
    //: Goto next scan line.
    // Returns true if we're now at a valid scanline.
    
    bool operator++(int)
    { return Next(); }
    //: Goto next point.
    // Returns true if we're now at a valid scanline.
  private:
    Polygon2dC m_polygon;
    IndexC m_row;
    IndexRangeC m_indexRange;
    bool m_valid;
    IELC m_iel;
    AELC m_ael;

  };

}  

#endif
