// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Projective/2D/PProjection2d.cc"

#include "Ravl/PProjection2d.hh"
#include "Ravl/Projection2d.hh"

namespace RavlN {

  PProjection2dC::PProjection2dC(const PPoint2dC & ix0, const PPoint2dC & ix1,
				 const PPoint2dC & iy0, const PPoint2dC & iy1,
				 const PPoint2dC & o0,  const PPoint2dC & o1,
				 const PPoint2dC & u0,  const PPoint2dC & u1)
    // Ref.: pp. 120-121
  {
#if 0
    // Projection from the normalized projective plane onto the plane 0.
    Point3dC c0(Matrix3dC(ix0, iy0, o0, 3).Inverse() * u0);
    Matrix3dC t0(c0.X()*ix0, c0.Y()*iy0, c0.Z()*o0, 3);
    
    // Projection from the normalized projective plane onto the plane 1.
    Point3dC   c1(Matrix3dC(ix1, iy1, o1, 3).Inverse() * u1);
    Matrix3dC t1(c1.X()*ix1, c1.Y()*iy1, c1.Z()*o1, 3);
    (*this) = (t1 * t0.Inverse());
#endif    
    RavlAssert(0);
  }
  
  //: Construct from a euclidean projection. 
  
  PProjection2dC::PProjection2dC(const Projection2dC &proj) 
    : Matrix3dC(proj.Homography())
  {}
  
  PProjection2dC::PProjection2dC(istream & inS)
  { inS >> *this; }
  
  ostream & operator<<(ostream & outS, const PProjection2dC & p) {
    for (UIntT i = 0; i< 3; ++i) {
      for (UIntT j = 0; j < 3; ++j)
	outS << p[i][j] << ' ';
      outS << '\n';
    }
    return outS;
  }
  
  istream & operator>>(istream & inS, PProjection2dC & p) {
    for (UIntT i = 0; i < 3; ++i)
      for (UIntT j = 0; j < 3; ++j)
	inS >> p[i][j];
    return inS;
  }
  
}
