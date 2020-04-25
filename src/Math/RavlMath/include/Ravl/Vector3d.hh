// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTOR3D_HEADER
#define RAVL_VECTOR3D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/LinearAlgebra/FixedSize/Vector3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Geometry.3D"

#include "Ravl/FVector.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: 3D Vector
  
  class Vector3dC
    : public FVectorC<3>
  {
  public:
    Vector3dC()
    {}
    //: Default constructor.
    
    Vector3dC(const TFVectorC<RealT,3> &base)
      : FVectorC<3>(base)
    {}
    //: Base constructor.
    
    Vector3dC(RealT v1,RealT v2,RealT v3) { 
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

    RealT &Row()
    { return data[0]; }
    //: First component of vector.

    RealT Row() const
    { return data[0]; }
    //: First component of vector.

    RealT &Col()
    { return data[1]; }
    //: Second component of vector.

    RealT Col() const
    { return data[1]; }
    //: Second component of vector.
    
    RealT &Z()
    { return data[2]; }
    //: Third component of vector.
    
    RealT Z() const
    { return data[2]; }
    //: Third component of vector.
    
    inline Vector3dC Cross(const Vector3dC & v) const {
      return Vector3dC(    data[1] * v[2] - data[2] * v[1] ,
			   -(data[0] * v[2] - data[2] * v[0]),
			   data[0] * v[1] - data[1] * v[0]
			   );
    }
    // Returns the cross product of this vector and the 'vector'.    
  };
  
  template<>
  inline RealT TFVectorC<RealT,3>::Dot(const TFVectorC<RealT,3> &oth) const 
  { return data[0] * oth.data[0] + data[1] * oth.data[1] + data[2] * oth.data[2]; }
  //: Calculate the dot product of this and 'oth' vector.

}

#endif
