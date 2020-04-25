// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/ScanPolygon2d.cc"

#include "Ravl/ScanPolygon2d.hh"
#include "Ravl/SArray1dIter2.hh"
#include <stdlib.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  // Segments:
  // The area between the two segments scanned
  //
  //      P2      P1
  //       | #### |
  //    D1 | scan | D2
  //       | #### |
  //      P1      P2

  //: Constructor.
  
  ScanPolygon2dC::LineSegmentC::LineSegmentC(const Point2dC *_p1,const Point2dC *_p2) 
    : m_p1(_p1),
      m_p2(_p2)
  {
  }
  
  //: Get column of intersection with row.
  
  bool ScanPolygon2dC::LineSegmentC::IntersectRow(RealT row,RealT &col) const {
    //std::cerr << "ScanPolygon2dC::LineSegmentC::IntersectRow() " << *p1 << " -> " << *p2 << " @ " << row << "\n";
    return LinePP2dC(*m_p1,*m_p2).IntersectRow(row,col);
  }
  
  //: Get row intersection with no checking.
  
  RealT ScanPolygon2dC::LineSegmentC::IntersectRow(RealT row) const {
    RealT col = 0;
    if(!LinePP2dC(*m_p1,*m_p2).IntersectRow(row,col)) {
      std::cerr << "ScanPolygon2dC::LineSegmentC::IntersectRow(), Failed. \n";
      RavlAssert(0);
    }
    return col;
  }

  ostream &operator<<(ostream &os,const ScanPolygon2dC::LineSegmentC &ls) {
    os << "(" << *ls.P1() << " " << *ls.P2() << ") ";
    return os;
  }
  //:-----------------------------------------------------
  
  static int CompareRows(const void *e1,const void *e2) {
    Point2dC &p1 =**((Point2dC **) e1); 
    Point2dC &p2 =**((Point2dC **) e2);
    RealT val = p1.Row() - p2.Row();
    //std::cerr << "val=" << val << "\n";
    // Make sure rounding doesn't screw things up.
    if(val > 0) return 1;
    if(val < 0) return -1;
    // Sort on column next
    val = p1.Col() - p2.Col();
    if(val > 0) return 1;
    if(val < 0) return -1;
    return 0;
  }
  
  //: Get following point on polygon
  
  const Point2dC *ScanPolygon2dC::NextPnt(const Point2dC *pnt) {
    if(pnt == m_lastPnt) return m_firstPnt;
    return (pnt + 1);
  }
  
  //: Get previous point on polygon
  
  const Point2dC *ScanPolygon2dC::PrevPnt(const Point2dC *pnt) {
    if(pnt == m_firstPnt) return m_lastPnt;
    return (pnt - 1);
  }
  
  //: Quantise row position
  
  RealT ScanPolygon2dC::RowQuant(RealT row) {
    RealT qRow = row / m_rowStep;
    return Ceil(qRow) * m_rowStep;
  }
  
  //: Goto first polygon in scan.
  
  bool ScanPolygon2dC::First(const Polygon2dC &poly,RealT _rowStep) {
    m_rowStep = _rowStep;
    // Make an array of lines and points
    
    size_t size = poly.Size();
    RavlAssert(poly.Area() < 0);
    
    m_points = SArray1dC<Point2dC>(size);
    m_ptrs = SArray1dC<Point2dC *>(size);
    if(size < 1) {
      ONDEBUG(std::cerr << "Polygon empty \n");
      return false;
    }
    
    SArray1dIter2C<Point2dC,Point2dC *> lit(m_points,m_ptrs);    
    for(DLIterC<Point2dC> it(poly);it;it++,lit++) {
      lit.Data1() = *it;
      lit.Data2() = &lit.Data1();
    }
    m_firstPnt = &(m_points[0]);
    m_lastPnt = &(m_points[size-1]);
    qsort((void *) &(m_ptrs[0]),size,sizeof(Point2dC *),&CompareRows);
    m_rowLast = m_ptrs[size-1]->Row();
#if DODEBUG
    std::cerr << " " << size << " points:";
    for(SArray1dIterC<Point2dC *> it(m_ptrs);it;it++)
      std::cerr << " " << *(*it) << ",";
    std::cerr << "\n";
#endif
    m_pat = 0;
    m_row = RowQuant(m_ptrs[m_pat]->Row());
    while(m_row < m_ptrs[m_pat]->Row())
      m_row += m_rowStep;
    ONDEBUG(std::cerr << "First row=" << m_row << " \n");
    ONDEBUG(std::cerr << "Starting at " << *PrevPnt(m_ptrs[m_pat]) << " : " << *m_ptrs[m_pat] << " : " << *NextPnt(m_ptrs[m_pat]) << "\n");
    CheckSpans();
    m_spans.First();
    if(!m_spans.IsElm()) {
      ONDEBUG(std::cerr << "Degenerate polygon. \n");
      return false;
    }
    SetupSpan();
    return true;
  }
  
  //: Setup current 'span'
  
  bool ScanPolygon2dC::SetupSpan() {
    RealT v1 = 0,v2 = 0;
    RavlAssert(m_spans.IsElm());
//    RavlAssert(spans->Data1().Height() < 0);
//    RavlAssert(spans->Data2().Height() > 0);
    m_spans->Data1().IntersectRow(m_row,v1);
    m_spans->Data2().IntersectRow(m_row,v2);
    m_span = RealRangeC(v1,v2);
    return true;
  }
  
  bool ScanPolygon2dC::CheckSpans() {
    bool changed = false;
    //ONDEBUG(std::cerr << "CheckSpans at " << m_pat << " of " << m_ptrs.Size() << " \n");

    // Go through all new points we've passed
    while(m_pat < m_ptrs.Size()) {
      RealT nrow = m_ptrs[m_pat]->Row();
      if(m_row < nrow)
	break; // Done all of them.

      changed = true;
      ONDEBUG(std::cerr << "================================================================\n");
      ONDEBUG(std::cerr << "ScanPolygon2dC::Next(), pit=" << *(m_ptrs[m_pat]) << " pat=" << m_pat <<" Row=" << m_row << "\n");
      ONDEBUG(std::cerr << "ScanPolygon2dC::Next(), Spans=" << m_spans.List() << "\n");
      
      // Is this end or begining of an existing line ?
      const Point2dC *at = m_ptrs[m_pat];
      bool done = false;
      for(m_spans.First();m_spans;m_spans++) {
	ONDEBUG(std::cerr <<
                "At=" << *at << " (" << at << ") =>"
		" D1.P1()=" << *m_spans->Data1().P1() << " (" << m_spans->Data1().P1() << ") " <<
		" D1.P2()=" << *m_spans->Data1().P2() << " (" << m_spans->Data1().P2() << ") " <<
		" D2.P1()=" << *m_spans->Data2().P1() << " (" << m_spans->Data2().P1() << ") " <<
		" D2.P2()=" << *m_spans->Data2().P2() << " (" << m_spans->Data2().P2() << ") " <<
                "\n");
	ONDEBUG(std::cerr << "T1: " << at << " " << m_spans->Data1().P1() << "\n");
	if(m_spans->Data1().P1() == at) {
	  const Point2dC *prev = PrevPnt(m_ptrs[m_pat]);
	  ONDEBUG(std::cerr << "Prev=" << *prev << "\n");
	  if(prev->Row() >= nrow && m_spans->Data1().P1() != m_spans->Data2().P2()) {
  	    ONDEBUG(std::cerr << "Add segment 1 " << *prev << " " << *m_spans->Data1().P1() <<  "\n");
	    m_spans->Data1() = LineSegmentC(prev,m_spans->Data1().P1());
	  } else {
	    ONDEBUG(std::cerr << "Del segment 1 " << *prev << " " << *m_spans->Data1().P1() <<  "\n");
            m_spans.Del();
	  }
  	  m_pat++;
	  done = true;
	  break;
	}
	ONDEBUG(std::cerr << "T1: " << at << " " << m_spans->Data2().P2() << "\n");
	if(m_spans->Data2().P2() == at) {
	  const Point2dC *next = NextPnt(m_ptrs[m_pat]);
	  ONDEBUG(std::cerr << "Next=" << *next << " spans.IsLast()=" << m_spans.IsLast() << " RowGT=" << (next->Row() >= nrow) << " Eq=" << (m_spans->Data1().P2() == m_spans->Data2().P1()) << "\n");
          
	  if(next->Row() >= nrow) {
	    ONDEBUG(std::cerr << "Add segment 2 " << *m_spans->Data2().P2() << " " << *next <<  "\n");
	    m_spans->Data2() = LineSegmentC(m_spans->Data2().P2(),next);
	  } else {
	    ONDEBUG(std::cerr << "Merge with next segment. \n");
            RavlAssert(!m_spans.IsLast());
            m_spans->Data2() = m_spans.NextData().Data2();
            m_spans.Next();
            m_spans.Del();
	  }
	  m_pat++;
	  done = true;
	  break;
	}
      }
      if(done) {
	ONDEBUG(std::cerr << "Done. " << m_spans.List().Size() << "\n");
	continue;
      }
      ONDEBUG(std::cerr << "Found a spike. \n");
      //: Must be a spike
      // Either split existing span, or create a new one.

      RealT prow = (*m_ptrs[m_pat])[0];
      RealT pcol = (*m_ptrs[m_pat])[1];

      // Does point itersect existing span ?
      bool newSpike = true;
      for(m_spans.First();m_spans;m_spans++) {
        RealT v1 = m_spans->Data1().IntersectRow(prow);
        RealT v2 = m_spans->Data2().IntersectRow(prow);
        if(Min(v1,v2) < pcol && pcol < Max(v1,v2)) {
          newSpike = false;
          break;
        }
      }

      const Point2dC *next = NextPnt(m_ptrs[m_pat]);
      const Point2dC *prev = PrevPnt(m_ptrs[m_pat]);

      LineSegmentC seg1(prev,m_ptrs[m_pat]);
      LineSegmentC seg2(m_ptrs[m_pat],next);

      ONDEBUG(std::cerr << "Found a spike. New=" << newSpike << " @" << *m_ptrs[m_pat] << " N:" << *m_ptrs[Min(m_pat+1,(size_t) m_ptrs.Size()-1)] << " \n");
      if(newSpike) {

        // If points lie along a line, just skip them.
        while(seg2.P1()->Row() == seg2.P2()->Row()) {
          RavlAssert(m_pat < (m_ptrs.Size()-1));
          if(m_ptrs[m_pat+1] != seg2.P2())
            break;
          m_pat++;
          ONDEBUG(std::cerr<< "AT " << *m_ptrs[m_pat] << " : Seg " << *seg2.P1() << " " << *seg2.P2() << " \n");
          ONDEBUG(std::cerr<< "Moving spike " << *seg2.P2() << " to " << *NextPnt(m_ptrs[m_pat])<< " \n");
          RavlAssert(m_ptrs[m_pat] == seg2.P2());
          seg2.SetP1(m_ptrs[m_pat]);
          seg2.SetP2(NextPnt(m_ptrs[m_pat]));
          RavlAssert(seg2.P1() != seg2.P2());
        }

        // Check if this is really a merge with another segment.
        if(seg2.P1()->Row() > seg2.P2()->Row()) {
          // Its going up, must be a merge.
          ONDEBUG(std::cerr << "NOT A SPIKE. Seg2= " << *seg2.P1() << " to " << *seg2.P2() << " spans:" << m_spans.List().Size() <<"\n");
          if(seg1.P2()->Row() == m_rowLast)
            break; // 
          for(m_spans.First();m_spans;m_spans++) {
            //std::cerr << "Check= " << *m_spans->Data1().P1() << " to " << *m_spans->Data1().P2() << "\n";
            if(m_spans->Data1().P1() == seg2.P1())
              break;
          }
          RavlAssert(m_spans.IsElm());

          m_spans->Data1() = seg1;
        } else {
          ONDEBUG(std::cerr << "A SPIKE. Seg2= " << *seg2.P1() << " to " << *seg2.P2() << " spans:" << m_spans.List().Size() <<"\n");

          // New segment, work out where to put it.
          for(m_spans.First();m_spans;m_spans++)
            if(pcol < m_spans->Data1().IntersectRow(m_row))
              break;

          m_spans.InsertBef(PairC<LineSegmentC>(seg1,seg2));
        }
      } else {
	// Split segment
	RavlAssert(m_spans); // Should be in a valid span!

        // If points lie along a line, just skip them.
        while(seg1.P1()->Row() == seg1.P2()->Row()) {
          RavlAssert(m_pat < (m_ptrs.Size()-1));
          if(m_ptrs[m_pat+1] != seg1.P1())
            break;
          m_pat++;
          ONDEBUG(std::cerr<< "Moving split " << *seg1.P1() << " to " << *PrevPnt(m_ptrs[m_pat])<< " \n");
          seg1.SetP2(seg1.P1());
          seg1.SetP1(PrevPnt(m_ptrs[m_pat]));
          RavlAssert(seg1.P1() != seg1.P2());
        }

        // Check if this is really a merge with another segment.
          
        LineSegmentC tmp = m_spans->Data2();
        m_spans->Data2() = seg2;
        
        if(seg1.P1()->Row() > seg1.P2()->Row()) {
          m_spans.InsertAft(PairC<LineSegmentC>(seg1,tmp));
        }
      }
      m_pat++;
    }
    if(changed) {
      ONDEBUG(std::cerr << "Final spans=" << m_spans.List() << "\n");
    }
    
    return true;
  }

  //: Goto next scan element.
  
  bool ScanPolygon2dC::Next() {
    //ONDEBUG(std::cerr << "ScanPolygon2dC::Next(), Spans=" << m_spans.List().Size() << " Row=" << m_row << "\n");
    m_spans.Next();
    if(m_spans) {
      SetupSpan();
      return true;
    }
    if(m_row > m_rowLast) {
      ONDEBUG(std::cerr << "Passed last row. \n");
      return false;
    }
    m_row += m_rowStep;
    //ONDEBUG(std::cerr << "ScanPolygon2dC::Next(), Row=" << m_row << "\n");
    CheckSpans();
    
    m_spans.First();
    if(!m_spans.IsElm()) {
      return false;
    }
    SetupSpan();
    return true;
  }
  
}
