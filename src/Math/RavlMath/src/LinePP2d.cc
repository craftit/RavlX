// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/LinePP2d.cc"

#include "Ravl/LinePP2d.hh"

#define CTOP    0x1
#define CBOTTOM 0x2
#define CRIGHT  0x4
#define CLEFT   0x8

namespace RavlN {
  
  static inline int ContainsCode(const Point2dC &pnt,const RealRange2dC &rng) {
    int ret = 0;
    if(pnt[0] > rng.BRow())
      ret |= CBOTTOM;
    else if(pnt[0] < rng.TRow())
      ret |= CTOP;
    if(pnt[1] > rng.RCol())
      ret |= CRIGHT;
    else if(pnt[1] < rng.LCol())
      ret |= CLEFT;
    return ret;
  }
  
  //: Clip line by given rectangle.
  // Returns false if no part of the line is in the rectangle.
  // Uses the Cohen and Sutherland line clipping algorithm.
  
  bool LinePP2dC::ClipBy(const RealRange2dC &rng) {
    bool accept = false;
    int oc0 = ContainsCode(P1(),rng);
    int oc1 = ContainsCode(P2(),rng);
#if 0
    const RealT vscale = rng.Rows();
    const RealT hscale = rng.Cols();
    RealT diff = ;
    //if(IsSmall(diff,hscale)) // Avoid division by zero. 
    //  np[0] = 0;
    //else
#endif
    
    do {
      if(!(oc0 | oc1)) {
	accept = true;
	break;
      } 
      if(oc0 & oc1)
	break;
      Point2dC np;
      int oc = oc0 ? oc0 : oc1;
      if(oc & CTOP) {
	np[0] = rng.TRow();
	np[1] = P1()[1] + (P2()[1] - P1()[1]) * (rng.TRow() - P1()[0]) / (P2()[0] - P1()[0]);
      } else if(oc & CBOTTOM) {
	np[0] = rng.BRow();
	np[1] = P1()[1] + (P2()[1] - P1()[1]) * (rng.BRow() - P1()[0]) / (P2()[0] - P1()[0]);
      } else if(oc & CRIGHT) {
	np[0] = P1()[0] + (P2()[0] - P1()[0]) * (rng.RCol() - P1()[1]) / (P2()[1] - P1()[1]);
	np[1] = rng.RCol();
      } else { // CLEFT
	np[0] = P1()[0] + (P2()[0] - P1()[0]) * (rng.LCol() - P1()[1]) / (P2()[1] - P1()[1]);
	np[1] = rng.LCol();
      }
      if(oc == oc0) {
	P1() = np;
	oc0 = ContainsCode(P1(),rng);
      } else {
	P2() = np;
	oc1 = ContainsCode(P2(),rng);
      }
    } while(1) ;
    return accept;
  }
 
  bool LinePP2dC::IsPointIn(const Point2dC& point) const {
    if ( ! IsPointOn(point))
      return false;

    // If ab not vertical, check betweenness on x; else on y.
    if ( P1()[0] != P2()[0] ) 
      return   ((P1()[0] <= point[0]) && (point[0] <= P2()[0]))
	||((P1()[0] >= point[0]) && (point[0] >= P2()[0]));
    else
      return   ((P1()[1] <= point[1]) && (point[1] <= P2()[1]))
	||((P1()[1] >= point[1]) && (point[1] >= P2()[1]));
  }

  Point2dC LinePP2dC::Intersection(const LinePP2dC & l) const {
    Vector2dC n1(static_cast<Vector2dC>(Vector()).Perpendicular());
    Vector2dC n2(static_cast<Vector2dC>(l.Vector()).Perpendicular());
    RealT     d1  = - n1.Dot(FirstPoint());
    RealT     d2  = - n2.Dot(l.FirstPoint());
    RealT     det = n1.Cross(n2);
    if (IsAlmostZero(det))
      return Point2dC(0.0, 0.0);
    return Point2dC((n1[1]*d2 - n2[1]*d1)/det,
		    (n2[0]*d1 - n1[0]*d2)/det);
  }
  
  bool LinePP2dC::Intersection(const LinePP2dC & l, Point2dC & here) const {
    Vector2dC n1(static_cast<Vector2dC>(Vector()).Perpendicular());
    Vector2dC n2(static_cast<Vector2dC>(l.Vector()).Perpendicular());
    RealT     det = n1.Cross(n2);
    if (IsAlmostZero(det))
      return false;
    RealT     d1  = - n1.Dot(FirstPoint());
    RealT     d2  = - n2.Dot(l.FirstPoint());
    here[0] = (n1[1]*d2 - n2[1]*d1)/det;
    here[1] = (n2[0]*d1 - n1[0]*d2)/det;
    return true;
  }
  
  //: Find the column position which itersects the given row.
  
  bool LinePP2dC::IntersectRow(RealT row,RealT &col) const {
    Vector2dC dir = P2() - P1();
    row -= P1()[0];
    if(dir[0] == 0)
      return false;
    col = ((row * dir[1]) / dir[0]) + P1()[1];
    return true;
  }
  
  RealT LinePP2dC::ParIntersection(const LinePP2dC & l) const {
    Vector2dC u2P(l.point[0][1] - l.point[1][1],l.point[1][0] - l.point[0][0]); // u2p = l.Vector().Perpendicular();
    return (l.FirstPoint()-FirstPoint()).Dot(u2P)/Vector().Dot(u2P);     
  }
  
  bool LinePP2dC::HasInnerIntersection(const LinePP2dC & l) const {
    RealT t = ParIntersection(l);
    return t >= 0 && t<=1;
  }

  RealT LinePP2dC::DistanceWithin(const Point2dC & pt) const {
    RealT t = ParClosest(pt);
    if (t < 0.0) return P1().EuclideanDistance(pt);
    if (t > 1.0) return P2().EuclideanDistance(pt);
    return Distance(pt);
  }
   
}
