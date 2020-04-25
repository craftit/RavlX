// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPLANE3D_HH
#define RAVL_PPLANE3D_HH 1
///////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/3D/PPlane3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.05.1995"
//! docentry="Ravl.API.Math.Projective Geometry.3D"
//! rcsid="$Id$"

#include "Ravl/PPointPlane3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/PlaneABCD3d.hh"

namespace RavlN {
  
  class Point3dC;
  class PPoint3dC;
  class Vector3dC;
  class PlanePVV3dC;
  class PlaneABCD3dC;
  class Point4dC;
  template<class DataT> class SArray1dC;
  
  //! userlevel=Normal
  //: Plane in 3D projective space
  // The class represents a plane in 3 dimensional projective space.
  // A plane contains 4 real numbers which serve as its projective
  // coordinates. 
  
  class PPlane3dC
    : public PPointPlane3dC
  {    
  public:
    
    //:----------------------------------------------
    // Constructors, assigment, copy, and destructor.
    
    inline PPlane3dC()
      : PPointPlane3dC(0,0,0,0)
    {}
    //: Constructs the plane whose coordinates are zeros,
    //: an invalid projective plane.
    
    inline PPlane3dC(const Vector3dC & n, RealT d)
      : PPointPlane3dC(n.X(), n.Y(), n.Z(), d)
    {}
    //: Constructs the projective plane equivalent to the Euiclidian
    //: plane determined by the equation n.X()*x + n.Y()*y + n.Z()*z + d = 0.
    
    inline PPlane3dC(const Point3dC & p1,
		     const Point3dC & p2,
		     const Point3dC & p3);
    // Constructs the projective plane passing through three Euclidian
    // points p1, p2, p3.

    inline PPlane3dC(const Point3dC  & p, 
		     const Vector3dC & v1,
		     const Vector3dC & v2)
      : PPointPlane3dC(p, v1, v2)
    {}
    // Constructs the projective plane passing through the Euclidian
    // point 'p' and two Euclidian plane direction vectors 'v1' and 'v2'.
    
    inline PPlane3dC(RealT p1, RealT p2, RealT p3, RealT p4)
      : PPointPlane3dC(p1, p2, p3, p4)
    {}
    // Constructs the plane (p1, p2, p3, p4).
    
    inline PPlane3dC(const PlaneABCD3dC & p)
      : PPointPlane3dC(p)
    {}
    // Constructs the projective plane from the Euclidian plane 'p'.

    inline PPlane3dC(const PPointPlane3dC & p0,
		     const PPointPlane3dC & p1,
		     const PPointPlane3dC & p2)
      : PPointPlane3dC(p0, p1, p2)
    {}
    // Constructs the plane from the projective points 'p0', 'p1', and 'p2'.
    
    inline PPlane3dC(const PPointPlane3dC & p)
      : PPointPlane3dC(p)
    {}
    // Constructs the plane from the point/plane object 'p'.
    
    //:---------------------
    // Access to the object.

    inline const PPlane3dC & PPlane3d() const
    { return *this; }
    // Access to this constant object.
    
    inline PPlane3dC & PPlane3d()
    { return *this; }
    // Access to the point.
    
    // Logical conditions.
    // -------------------

    inline bool IsIdeal() const
    { return IsIdealPlane(); }
    // Returns true iff this plane is an ideal projective plane.
    // The plane is an ideal projective plane if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to 
    // the threshold 'Point4dC::relZero'.

    //:------------------------
    // Arithmetical operations.

    inline const PPlane3dC & operator*=(const RealT alpha)
    { PPointPlane3dC::operator*=(alpha); return *this; }
    // Multiplies both coordinates by 'alpha'.

    inline const PPlane3dC & operator/=(const RealT alpha)
    { PPointPlane3dC::operator/=(alpha); return *this; }
    // Divides both coordinates by 'alpha'.
    
    inline PPlane3dC operator*(const RealT alpha) const
    { return PPointPlane3dC::operator*(alpha); }
    // Returns the point which coordinates are multiplied by 'alpha'.

    inline PPlane3dC operator/(const RealT alpha) const
    { return PPointPlane3dC::operator/(alpha); }
    // Returns the point which coordinates are divided by 'alpha'.

    //:-----------------------
    // Geometrical operations.
    
    inline PPointPlane3dC Intersection(const PPlane3dC & p1,
				       const PPlane3dC & p2) const
    { return PPointPlane3dC(*this, p1, p2); }
    // Returns the projective point of intersection of three planes -
    // this plane, 'p1', and 'p2'.

    friend inline PPlane3dC operator*(RealT lambda, const PPlane3dC & p);
    friend inline PPlane3dC operator/(RealT lambda, const PPlane3dC & p);
    friend istream & operator>>(istream & inS, PPlane3dC & point);
  };
  
  bool FitPPlane3d(const SArray1dC<PPoint3dC> &points,PPlane3dC &plane);
  //: Fit plane to a set of points.
  
  inline
  PPlane3dC operator*(RealT lambda, const PPlane3dC & p)
  { return p * lambda; }
  //: Returns the point which is the 'lambda' multiplication of 'p'.
  
  ostream & operator<<(ostream & outS, const PPlane3dC & point);
  //: Writes the 'point' into the output stream.

  istream & operator>>(istream & inS, PPlane3dC & point);
  //: Reads the point parameters from the input stream.
  
  BinOStreamC & operator<<(BinOStreamC & strm, const PPlane3dC & p);
  //: Writes the 'point' into the output stream.
  
  BinIStreamC & operator>>(BinIStreamC & strm, PPlane3dC & p);
  //: Reads the point parameters from the input stream.
  
}
#endif
