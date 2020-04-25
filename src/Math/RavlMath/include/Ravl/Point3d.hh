// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POINT3D_HEADER
#define RAVL_POINT3D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/3D/Point3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Geometry.3D"

#include "Ravl/FPoint.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Point in 3d space.
  
  class Point3dC
    : public FPointC<3>
  {
  public:
    Point3dC()
    {}
    //: Default constructor.
    
    Point3dC(const TFVectorC<RealT,3> &base)
      : FPointC<3>(base)
    {}
    //: Constructor.

    Point3dC(const FIndexC<3> &ind) 
      : FPointC<3>(ind)
    {}
    //: Construct from a 3d index.
    
    Point3dC(RealT v1,RealT v2,RealT v3) { 
      data[0] = v1;
      data[1] = v2;
      data[2] = v3;
    }
    //: Constructor.
    
    RealT &X()
    { return data[0]; }
    //: First component of vector.

    RealT X() const
    { return data[0]; }
    //: First component of vector.

    RealT &Y()
    { return data[1]; }
    //: Second component of vector.

    RealT Y() const
    { return data[1]; }
    //: Second component of vector.
    
    RealT &Z()
    { return data[2]; }
    //: Third component of vector.
    
    RealT Z() const
    { return data[2]; }
    //: Third component of vector.

    
  };
}

#endif
