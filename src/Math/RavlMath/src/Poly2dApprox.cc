// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath

#include "Ravl/Polygon2d.hh"
#include "Ravl/PolyLine2d.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  
  static DListC<Point2dC>  SortSegment(DLIterC<Point2dC> &p1,DLIterC<Point2dC> &p2,RealT maxDist) {
    DListC<Point2dC> ret;
    DLIterC<Point2dC> at = p1;
    LineABC2dC line(*p1,*p2);
    DLIterC<Point2dC> fp;
    RealT largestDist = maxDist;
    for(; at != p2;at.NextCrc()) {
      RealT d = line.Distance(*at);
      if(d > largestDist) {
        fp = at;
        largestDist = d;
      }
    }
    // No mid point, return empty list.
    if(!fp) return ret;
    
    // Do two sub sections either side.
    DListC<Point2dC> tmp = SortSegment(p1,fp,maxDist);
    ret.MoveLast(tmp);
    ret.InsLast(*fp); // Insert mid point.
    tmp = SortSegment(fp,p2,maxDist);
    ret.MoveLast(tmp);    
    return ret;
  }
  
  //: Generate an approximation to the given polyline within the given distance limit.
  
  PolyLine2dC PolyLine2dC::Approx(RealT distLimit) const {
    DLIterC<Point2dC> first = *this;
    if(!first) return PolyLine2dC();
    DLIterC<Point2dC> last = *this; last.Last();
    PolyLine2dC ret = SortSegment(first,last,distLimit);
    ret.InsFirst(*first);
    ret.InsLast(*last);
    return ret;
  }
  
  Polygon2dC Polygon2dC::Approx(RealT maxDist) const {
    Polygon2dC ret;
    DLIterC<Point2dC> it(*this);
    if(!it) return ret;
    
    // Find furthest point from start.
    
    RealT maxPointDist = 0;
    DLIterC<Point2dC> maxAt = it;
    Point2dC at = *it;
    for(;it;it++) {
      RealT x = at.SqrEuclidDistance(*it);
      if(x <= maxPointDist) 
        continue;    
      maxPointDist = x;
      maxAt = it;
    }
    
    // Find furthest point from other furthest point.
    it.First();
    maxPointDist = 0;
    DLIterC<Point2dC> max2At = it;
    at = *maxAt;
    for(;it;it++) {
      RealT x = at.SqrEuclidDistance(*it);
      if(x <= maxPointDist) 
        continue;    
      maxPointDist = x;
      max2At = it;
    }
    
    
    // Put in first point.
    ret.InsLast(*max2At);
    
    // Sort out first segment
    DListC<Point2dC> tmp = SortSegment(max2At,maxAt,maxDist);
    ret.MoveLast(tmp);
    
    // Insert mid point.
    ret.InsLast(*maxAt);

    // And the last.
    tmp = SortSegment(maxAt,max2At,maxDist);
    ret.MoveLast(tmp);    
    
    return ret;
  }
  
}
