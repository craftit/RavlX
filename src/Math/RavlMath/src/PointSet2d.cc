// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/PointSet2d.cc"

#include "Ravl/PointSet2d.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/Projection2d.hh"

namespace RavlN {

  Point2dC PointSet2dC::Centroid() const {
    // Create return value
    Point2dC centroid(0,0);
    // Sum
    IntT size = 0;
    for (DLIterC<Point2dC> point(*this); point; point++) {
      centroid += point.Data();
      size++;
    }
    // Divide
    centroid /= (RealT) size;
    // Done
    return centroid;
  }
  
  static RealT PCot(const Point2dC& oPointA, const Point2dC& oPointB, const Point2dC& oPointC) {
    Vector2dC oBA = oPointA - oPointB;
    Vector2dC oBC = oPointC - oPointB;
    RealT cross = Abs(oBC.Cross(oBA));
    if (cross < 1e-6)
      cross = 1e-6;
    return (oBC.Dot(oBA) / cross);
  }
   
  SArray1dC<RealT> PointSet2dC::BarycentricCoordinate(Point2dC& point) const {
    // Create return array
    SArray1dC<RealT> oWeights(Size());
    // Keep track of total
    RealT fTotalWeight = 0;
    // For each polygon vertex
    SArray1dIterC<RealT> res(oWeights);
    for (DLIterC<Point2dC> it(*this); it && res; it++) {
      RealT sqDist = Vector2dC(point - it.Data()).SumOfSqr();
      if (sqDist != 0) {
	RealT fWeight = (PCot(point,it.Data(),it.PrevCrcData()) + 
			 PCot(point,it.Data(),it.NextCrcData())) / sqDist;
	res.Data() = fWeight;
	fTotalWeight += fWeight;
      }
      else {
	res.Data() = 1;
	fTotalWeight += 1;
      }
      res++;
    }
    // Normalise weights
    oWeights /= fTotalWeight;
    // Done
    return oWeights;
  }
  
  //: Compute the bounding rectangle for the point set.
  
  RealRange2dC PointSet2dC::BoundingRectangle() const {
    RealRange2dC ret(0,0);
    DLIterC<Point2dC> point(*this);
    if(!point) 
      return ret; // No points in set.
    ret = RealRange2dC(*point,0);
    for (; point; point++)
      ret.Involve(*point);
    return ret;
  }
  
  //: Translate point set by vector.
  
  const PointSet2dC &PointSet2dC::operator+=(const TFVectorC<RealT,2> &offset) {
    for(DLIterC<Point2dC> it(*this);it;it++)
      *it += offset;
    return *this;
  }
  
  //: Translate point set by subracting a vector.
  
  const PointSet2dC &PointSet2dC::operator-=(const TFVectorC<RealT,2> &offset) {
    for(DLIterC<Point2dC> it(*this);it;it++)
      *it -= offset;
    return *this;
  }
  
  //: Scale the point set by multiplying the points by 'scale'.
  
  const PointSet2dC &PointSet2dC::operator*=(RealT scale) {
    for(DLIterC<Point2dC> it(*this);it;it++)
      *it *= scale;
    return *this;
  }
  
  //: Scale the point set by multiplying the points by trans.
  
  const PointSet2dC &PointSet2dC::operator*=(const Affine2dC &trans) {
    for(DLIterC<Point2dC> it(*this);it;it++)
      *it = trans * (*it);
    return *this;
  }
  
  //: Scale the point set by multiplying the points by trans.
  
  const PointSet2dC &PointSet2dC::operator*=(const Projection2dC &trans) {
    for(DLIterC<Point2dC> it(*this);it;it++)
      *it = trans * (*it);
    return *this;
  }
  
}
