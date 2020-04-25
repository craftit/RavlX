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
//! file="Ravl/Math/Geometry/Euclidean/3D/PointSet3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="James Smith"
//! date="26/9/2002"
//! docentry="Ravl.API.Math.Geometry.2D"

#include "Ravl/Point3d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DList.hh"
#include "Ravl/RealRange3d.hh"

namespace RavlN {  
  //! userlevel=Normal
  //: A set of points in 2d space
  
  class PointSet3dC: public DListC<Point3dC>
  {
  public:
    PointSet3dC() {}
    //: Empty list of points.
      
    PointSet3dC(const DListC<Point3dC> & points)
      : DListC<Point3dC>(points)
    {}
    //: Construct from list of points
      
    Point3dC Centroid() const;
    //: Returns the centroid (mean) of the points
    
    RealRange3dC BoundingRectangle() const;
    //: Compute the bounding rectangle for the point set.
    
    const PointSet3dC &operator+=(const TFVectorC<RealT,3> &offset);
    //: Translate point set by adding a vector.
    
    const PointSet3dC &operator-=(const TFVectorC<RealT,3> &offset);
    //: Translate point set by subracting a vector.
    
    const PointSet3dC &operator*=(RealT scale);
    //: Scale the point set by multiplying the points by 'scale'.
  };
   
}
#endif
