// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPOINT3D_HH
#define RAVL_PPOINT3D_HH 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/3D/PPoint3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math.Projective Geometry.3D"
//! rcsid="$Id$"
//! date="06/08/1995"

#include "Ravl/PPointPlane3d.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {
  class Point3dC;
  template<unsigned int N> class FAffineC;
  class Vector3dC;
  class Point4dC;
  class Matrix4dC;
  template<typename DataT> class SArray1dC;
  
  //! userlevel=Normal
  //: Point in 3D projective space
  // The class represents a point in 3 dimensional projective space.
  // The point contains 4 real numbers which serve as its projective
  // coordinates. 

  class PPoint3dC
    : public PPointPlane3dC
  {
  public:
    
    //:----------------------------------------------
    // Constructors, assigment, copy, and destructor.
    
    inline PPoint3dC()
      : PPointPlane3dC(0,0,0,0)
    {}
    //: Constructs the point whose coordinates are zeros, eg. non-existing
    //: projective point.
    
    inline PPoint3dC(RealT x, RealT y, RealT z)
      : PPointPlane3dC(x,y,z, 1)
    {}
    //: Constructs the point (x, y, z, 1).
    
    inline PPoint3dC(const Point3dC & p)
      : PPointPlane3dC(p[0], p[1], p[2], 1)
    {}
    //: Construct the projective point (p.X(), p.Y(), p.Z(), 1).
    
    explicit inline PPoint3dC(const Vector3dC & v,RealT p3 = 1.0)
      : PPointPlane3dC(v[0], v[1], v[2], p3)
    {}
    //: Construct the projective point (v.X(), v.Y(), v.Z(), 1).
    // Note: Really p3 should default to 0.
    
    inline PPoint3dC(RealT p0, RealT p1, RealT p2, RealT p3)
      : PPointPlane3dC(p0, p1, p2, p3)
    {}
    //: Constructs the point (p1, p2, p3, p4).
    
    inline PPoint3dC(const PPointPlane3dC & p)
      : PPointPlane3dC(p)
    {}
    //: Constructs the point from the point/plane object 'p'.
    
    inline PPoint3dC(const PPointPlane3dC & p0,
		     const PPointPlane3dC & p1,
		     const PPointPlane3dC & p2)
      : PPointPlane3dC(p0, p1, p2)
    {}
    //: Constructs the point of intersection of three projective planes 'p0', 'p1', and 'p2'.
    
    inline PPoint3dC(const RavlN::TFVectorC<RealT, 4> & p)
      : PPointPlane3dC(p)
    {}
    //: Creates the projective point in 1D space from values of the point 'p'.
    
    // Access to the object.
    // ---------------------

    inline const PPoint3dC & PPoint3d() const;
    //: Access to this constant object.

    inline PPoint3dC & PPoint3d();
    //: Access to the point.

    // Logical conditions.
    // -------------------

    inline bool IsIdeal() const
    { return IsIdealPoint(); }
    //: Returns true iff this point is an ideal projective point.
    // The point is an ideal projective point if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to 
    // the threshold 'Point4dC::relZero'.
    
    inline Point3dC Point3d() const
    { RavlAssert(!IsIdeal()); return Point3dC((*this)[0]/(*this)[3],(*this)[1]/(*this)[3],(*this)[2]/(*this)[3]); }
    //: Returns the point in Euclidian space related to this projective point.
    // It is assumed that the point is not ideal. The function
    // does not check if the point is ideal or not.
    
    inline
    bool IsColinear(const PPoint3dC & q2, const PPoint3dC & q3) const
    { RavlAssertMsg(0,"not implemented"); return true; }
    // Returns true iff this point and the points 'q2' and 'q3' are
    // colinear. NOT IMPLEMENTED
    
    //:------------------------
    // Arithmetical operations.

    inline const PPoint3dC & operator+=(const PPoint3dC & point) 
    { PPointPlane3dC::operator+=(point); return *this; }      
    //: Adds the values of the coordinates of the 'point' to this point.
    
    inline const PPoint3dC & operator-=(const PPoint3dC & point) 
    { PPointPlane3dC::operator-=(point); return *this; }
    //: Subtracts the values of the coordinates of the 'point' from this point.
    
    inline const PPoint3dC & operator*=(const RealT alpha)
    { PPointPlane3dC::operator*=(alpha); return *this; }
    //: Multiplies both coordinates by 'alpha'.
    
    inline const PPoint3dC & operator/=(const RealT alpha)
    { PPointPlane3dC::operator/=(alpha); return *this; }
    //: Divides both coordinates by 'alpha'.

    inline PPoint3dC operator+(const PPoint3dC & point) const
    { return PPointPlane3dC::operator+(point); }
    //: Returns the point which is the sum of this point and the 'point'.

    inline PPoint3dC operator-(const PPoint3dC & point) const
    { return PPointPlane3dC::operator-(point); }
    //: Returns the point which is the difference of this point and the 'point'.

    inline PPoint3dC operator*(const RealT alpha) const
    { return PPointPlane3dC::operator*(alpha); }
    //: Returns the point which coordinates are multiplied by 'alpha'.

    inline PPoint3dC operator/(const RealT alpha) const;
    //: Returns the point which coordinates are divided by 'alpha'.
    
    //:-----------------------
    // Geometrical operations.
    
    inline PPoint3dC Translation(const PPoint3dC & newOrigin) const
    { return PPointPlane3dC::Translation(newOrigin); }
    //: Returns the point with projective coordinates related to the new origin 'newOrigin'.
    
  protected:
    
#if 0
    inline PPoint3dC(const Point4dC & p)
      : PPointPlane3dC(p)
    {}
    // Creates the projective point in 1D space from values of 
    // the point 'p'.
#endif
    
    
    friend inline PPoint3dC operator*(RealT lambda, const PPoint3dC & p);
    friend inline PPoint3dC operator/(RealT lambda, const PPoint3dC & p);
    friend istream & operator>>(istream & inS, PPoint3dC & point);
    friend class PProjection3dC;
  };
  
  bool Normalise(const SArray1dC<PPoint3dC> &raw,SArray1dC<PPoint3dC> &norm,Matrix4dC &normMat);
  //: Normalise an array of points.
  // This finds the mean and variation of euclidean point position. It corrects the mean to zero
  // and the average variation to 1.
  // This assumes there are no points at infinity.
  //!param: raw - Raw points to be normalised
  //!param: norm - Normalised points.
  //!param: normMat - Normalisation matrix 
  //!return: Normalisation found and applied.
  
  inline
  PPoint3dC operator*(RealT lambda, const PPoint3dC & p)
  { return p * lambda; }
  // Returns the point which is the 'lambda' multiplication of 'p'.
  
  PPoint3dC Transform(const FAffineC<3> &transform,const PPoint3dC & p);
  // Affine transform of the space.
  
  ostream & operator<<(ostream & outS, const PPoint3dC & point);
  // Writes the 'point' into the output stream.

  istream & operator>>(istream & inS, PPoint3dC & point);
  // Reads the point parameters from the input stream.
  
}

#endif
