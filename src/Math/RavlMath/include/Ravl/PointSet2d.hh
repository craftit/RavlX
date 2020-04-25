// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POINTSET2D_HEADER
#define RAVL_POINTSET2D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/2D/PointSet2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="James Smith"
//! date="26/9/2002"
//! docentry="Ravl.API.Math.Geometry.2D"

#include "Ravl/Point2d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DList.hh"
#include "Ravl/RealRange2d.hh"

namespace RavlN {  

  class Affine2dC;
  class Projection2dC;

  //! userlevel=Normal
  //: A set of points in 2d space
  
  class PointSet2dC: public DListC<Point2dC>
  {
  public:
    PointSet2dC() {}
    //: Empty list of points.
      
    PointSet2dC(const DListC<Point2dC> & points)
      : DListC<Point2dC>(points)
    {}
    //: Construct from list of points
      
    Point2dC Centroid() const;
    //: Returns the centroid (mean) of the points

    SArray1dC<RealT> BarycentricCoordinate(Point2dC& point) const;
    //: Calculates the barycentric coordinate of point
    
    RealRange2dC BoundingRectangle() const;
    //: Compute the bounding rectangle for the point set.
    
    const PointSet2dC &operator+=(const TFVectorC<RealT,2> &offset);
    //: Translate point set by adding a vector.
    
    const PointSet2dC &operator-=(const TFVectorC<RealT,2> &offset);
    //: Translate point set by subracting a vector.
    
    const PointSet2dC &operator*=(RealT scale);
    //: Scale the point set by multiplying the points by 'scale'.

    const PointSet2dC &operator*=(const Affine2dC &trans);
    //: Transform the points in place using an affine transform

    const PointSet2dC &operator*=(const Projection2dC &trans);
    //: Transform the points in place using a projective transform
  };
   
}
#endif
