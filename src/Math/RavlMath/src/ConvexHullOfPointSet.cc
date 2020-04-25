// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Euclidean/2D/ConvexHullOfPointSet.cc"

#include "Ravl/Polygon2d.hh"
#include "Ravl/LineABC2d.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  
  static DListC<Point2dC> SubHull(DListC<Point2dC> &points,const Point2dC &start,const Point2dC &end) {
    
    LineABC2dC line(start,end);
    
    // Partition points to ones left and right of the line.
    
    DListC<Point2dC> left;
    RealT maxLeft = -1;
    DLIterC<Point2dC> mostLeft;
    
    for(DLIterC<Point2dC> it(points);it;it++) {
      RealT dist = line.SignedDistance(*it);
      if(dist > 0) {
	left.MoveFirst(it);
	if(dist > maxLeft) {
	  mostLeft = left;
	  maxLeft = dist;
	}
      }
    }
    DListC<Point2dC> hull;
    if(left.IsEmpty())
      return hull; // Nothing left!
    
    Point2dC pntLeft = *mostLeft;
    hull.MoveLast(mostLeft); // Take points we know are on the hull out.
    
    DListC<Point2dC> shull = SubHull(left,start,pntLeft);
    hull.MoveFirst(shull);
    
    if(!left.IsEmpty()) {
      shull = SubHull(left,pntLeft,end);
      hull.MoveLast(shull);
    }
    // Drop the rest of the points in 'left'
    
    return hull;
  }

  //: Generate a convex hull from a set of points
  // The list 'points' is destroyed.
  
  Polygon2dC ConvexHull(DListC<Point2dC>& points) {
    Polygon2dC ret;
    if(points.IsEmpty())
      return ret;
    
    // Find left and right most points.
    
    DLIterC<Point2dC> it(points);
    DLIterC<Point2dC> left = it;
    DLIterC<Point2dC> right = it;
    RealT min = (*it)[0];
    RealT max = min;
    it++;
    IntT npoints = 1;
    for(;it;it++) {
      if((*it)[0] < min) {
	min = (*it)[0];
	left = it;
      } else {
	if((*it)[0] > max) {
	  max = (*it)[0];
	  right = it;
	}
      }
      npoints++;
    }
    if(npoints <= 3) { // Trivial solution ?
      ret = Polygon2dC(points);
      return ret;
    }
    DListC<Point2dC> hull,tmp;
    Point2dC pntLeft = *left;
    Point2dC pntRight = *right;
    hull.MoveFirst(left);
    tmp.MoveFirst(right); // Move right most point onto a temporary list.
    if(!points.IsEmpty()) {
      DListC<Point2dC> shull = SubHull(points,pntLeft,pntRight);
      hull.MoveLast(shull);
    }
    hull.MoveLast(tmp); // Put right most point in.
    if(!points.IsEmpty()) {
      DListC<Point2dC> shull = SubHull(points,pntRight,pntLeft);
      hull.MoveLast(shull);
    }
    ret = Polygon2dC(hull);
    return ret;
  }
  
  
  //: Generate a convex hull from a set of point.
  
  Polygon2dC ConvexHull(const SArray1dC<Point2dC>& points) {
    DListC<Point2dC> pnts;      
    for(SArray1dIterC<Point2dC> it(points);it;it++)
      pnts.InsLast(*it);
    if(points.Size() <= 3) // Is solution trivial ?
      return Polygon2dC(pnts);
    return ConvexHull(pnts);
  }
  
}
