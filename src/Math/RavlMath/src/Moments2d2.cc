// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! date="6/8/1995"
//! author="Radek Marik"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Moments2d2.cc"

#include "Ravl/Moments2d2.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  //: Return the covariance matrix.
  
  Matrix2dC Moments2d2C::Covariance() const {
    if( IsSmall(m00) )
      return Matrix2dC(1,0,0,1); // if m00 is too small (prevent effective division by zero)
    RealT cent1 = CentroidX();
    RealT cent2 = CentroidY();
    RealT diag = m11 / m00 - cent1*cent2;
    return Matrix2dC(m20 / m00 - Sqr(cent1),
		     diag,
		     diag,
		     m02 / m00 - Sqr(cent2));
  }
  
  Vector2dC Moments2d2C::PrincipalAxisSize() const {
    Matrix2dC mat = Covariance();
    Vector2dC vec;
    EigenValuesIP(mat,vec);
    if(vec[0] < vec[1])
      Swap(vec[0],vec[1]);
    return vec; 
  }
  
  ostream &operator<<(ostream & os, const  Moments2d2C & mom) {
    os << mom.M00() << ' ' << mom.M10() << ' ' << mom.M01() << ' '
       << mom.M20() << ' ' << mom.M11() << ' ' << mom.M02();
    return os;
  }
  
  istream &operator>>(istream & is, Moments2d2C & mom) {
    is >> mom.m00 >> mom.m10 >> mom.m01
       >> mom.m20 >> mom.m11 >> mom.m02;
    return is;
  }
  
}
