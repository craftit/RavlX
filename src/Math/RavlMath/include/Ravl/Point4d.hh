// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POINT4D_HEADER
#define RAVL_POINT4D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/Point4d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Geometry.4D"

#include "Ravl/FPoint.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Point in 4D space.
  
  class Point4dC
    : public FPointC<4>
  {
  public:
    Point4dC()
    {}
    //: Default constructor.
    
    Point4dC(const TFVectorC<RealT,4> &base)
      : FPointC<4>(base)
    {}
    //: Constructor.

    Point4dC(const FIndexC<4> &ind) 
      : FPointC<4>(ind)
    {}
    //: Construct from a 4d index.
    
    Point4dC(RealT v1,RealT v2,RealT v3,RealT v4) { 
      data[0] = v1;
      data[1] = v2;
      data[2] = v3;
      data[3] = v4;
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
    
    RealT &W()
    { return data[3]; }
    //: Fourth component of vector.
    
    RealT W() const
    { return data[3]; }
    //: Fourth component of vector.
    
  };
}

#endif
