// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPOINTPLANE3D_HH
#define RAVL_PPOINTPLANE3D_HH 1
////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/3D/PPointPlane3d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.06.1995"
//! docentry="Ravl.API.Math.Projective Geometry.3D"
//! rcsid="$Id$"

#include "Ravl/Point4d.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/Vector3d.hh"

namespace RavlN {
  
  class PlaneABCD3dC;
  
  //! userlevel=Normal
  //: Point - plane in 3D projective space
  // The class represents a point/plane in 3 dimensional projective space.
  // An object contains 4 real numbers which serve as its projective
  // coordinates. An object can be treated as a point and its dual plane
  // and vice versa in any time. This feature reflects the dual relationship
  // of points and planes in 3D projective space.
  
  class PPointPlane3dC
    : public TFVectorC<RealT,4>
  {
  public:
    
    //:----------------------------------------------
    // Constructors, assigment, copy, and destructor.
    
    inline PPointPlane3dC()
    { Fill(0); }
    //: Constructs the point/plane whose coordinates are zeros
    
    inline PPointPlane3dC(RealT x, RealT y, RealT z)
      : TFVectorC<RealT,4>(Point4d(x, y, z, 1))
    {
    }
    // Constructs the point/plane (x, y, z, 1). x,y,z represent either
    // point Cartesian coordinates or plane normal expressed as vector
    // Cartesian coordinates.

    inline PPointPlane3dC(const Point3dC & p)
      : TFVectorC<RealT,4>(Point4d(p[0], p[1], p[2], 1))
    {}
    // Constructs the projective point/line (p.X(), p.Y(), p.Z(), 1) related
    // to the Euclidian point 'p'.

    inline PPointPlane3dC(const Vector3dC & v)
      : TFVectorC<RealT,4>(Point4d(v[0], v[1], v[2], 0))
    {}
    // Constructs the projective point/line (v.X(), v.Y(), v.Z(), 0) related
    // to the Euclidian direction 'v'.

    inline PPointPlane3dC(RealT p0, RealT p1, RealT p2, RealT p3)
      : TFVectorC<RealT,4>(Point4d(p0, p1, p2, p3))
    {}
    // Constructs the point/plane (p0, p1, p2, p3).
    
    inline PPointPlane3dC(const RavlN::TFVectorC<RealT, 4> & p)
      : TFVectorC<RealT,4>(p)
    {}
    // Creates the projective point/plane in 3D space from values of 
    // the point 'p'.
    
    PPointPlane3dC(const PlaneABCD3dC & p);
    // Constructs the projective point/plane corresponding to the Euclidian
    // plane 'p'.
    
    PPointPlane3dC(const PPointPlane3dC & p0,
                   const PPointPlane3dC & p1,
                   const PPointPlane3dC & p2);
    // Constructs the projective point/plane determined by three 
    // projective planes/points, resp.
    
    //:--------------------------------
    // Access to the object parameters.
    
    inline RealT Scale() const
    { return P4(); }
    // Returns the value of the last projective coordinate P4().
    
    inline const RealT & P1() const
    { return (*this)[0]; }
    // Returns the value of the 1st coordinate.

    inline const RealT & P2() const
    { return (*this)[1]; }
    // Returns the value of the 2nd coordinate.
    
    inline const RealT & P3() const
    { return (*this)[2]; }
    // Returns the value of the 3rd coordinate.

    inline const RealT & P4() const
    { return (*this)[3]; }
    // Returns the value of the 4th coordinate.
    
    inline RealT & P1()
    { return (*this)[0]; }
    // Returns the value of the 1st coordinate.
    
    inline RealT & P2()
    { return (*this)[1]; }
    // Returns the value of the 2nd coordinate.

    inline RealT & P3()
    { return (*this)[2]; }
    // Returns the value of the 3rd coordinate.

    inline RealT & P4()
    { return (*this)[3]; }
    // Returns the value of the 4th coordinate.
    
    inline RealT X() const  {
      RavlAssertMsg(!IsIdealPoint(),"an ideal point or a bad projective plane.");
      return P1()/Scale();
    }
    // Returns the value of the  1st Cartesian coordinate. The function
    // does not check if the point/plane is ideal or not.
    
    inline RealT Y() const {
      RavlAssertMsg(!IsIdealPoint(),"an ideal point or a bad projective plane.");
      return P2()/Scale();
    }
    // Returns the value of the 2nd Cartesian coordinate. The function
    // does not check if the point/plane is ideal or not.

    inline RealT Z() const {
      RavlAssertMsg(!IsIdealPoint(),"an ideal point or a bad projective plane.");
      return P3()/Scale();
    }
    // Returns the value of the 3rd Cartesian coordinate. The function
    // does not check if the point/plane is ideal or not.
    
    inline Point3dC Point3d() const { 
      RavlAssertMsg(!IsIdealPoint(),"an ideal point or a bad projective plane.");
      return Point3dC(P1(), P2(), P3())/Scale(); 
    }
    // Returns the point in 3D Euiclidian space corresponding to this
    // projective object. This object is treated as a projective point.
    // It is not checked if the projective point is ideal or not.
    
    inline const PPointPlane3dC & PPointPlane3d() const
    { return *this; }
    // Access to this constant object.

    inline PPointPlane3dC & PPointPlane3d()
    { return *this; }
    // Access to the point/plane.
    
    //:-------------------
    // Logical conditions.
    
    inline bool operator==(const PPointPlane3dC & p) const
    { return TFVectorC<RealT,4>::operator==(p); }
    // Returns true iff 2 points/planes are the same projective point/plane.
    // Two projective points/planes are equal iff their 4D Cartesian
    // representants are equal.

    inline bool operator!=(const PPointPlane3dC & p) const
    { return TFVectorC<RealT,4>::operator!=(p); }
    // Returns true iff 2 points/planes are different projective points/planes.
    // Two projective points/planes are different iff they are not
    // equal in the sense of the operator '=='.
    
    inline bool IsIdealPoint() const  {
      const RealT sum = SumOfAbs();
      return IsAlmostZero(sum) 
	? false 
	: IsAlmostZero(Scale()/sum);
    }
    // Returns true iff this point/plane represents an ideal projective point.
    // The point is an ideal projective point if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to 
    // the threshold 'Point4dC::relZero'.

    inline bool IsIdealPlane() const {
      RavlAssertMsg(!IsAlmostZero(Scale()),"a bad projective plane.");
      return IsAlmostZero((RavlN::Abs(P1())+RavlN::Abs(P2())+RavlN::Abs(P3()))/Scale());
    }
    // Returns true iff this point/plane represents an ideal projective plane.
    // The plane is an ideal projective plane if the absolute value of
    // the ratio (Abs(P1)+Abs(P2()+Abs(P3))/Scale() is smaller or equal to 
    // the threshold 'Point4dC::relZero'.
    
    inline bool IsValid() const
    { return !IsAlmostZero(SumOfAbs()); }
    // Returns true if the sum of absolute values of the projective coordinates
    // is bigger than the threshold 'Point4dC::zeroDistance'.
    
    //:------------------------
    // Arithmetical operations.

    inline RealT Sum() const
    { return TFVectorC<RealT,4>::Sum(); }
    // Returns the sum of coordinates.
    
    inline RealT SumOfAbs() const
    { return TFVectorC<RealT,4>::SumOfAbs(); }
    // Returns the sum of absolute value of coordinates.
    
    inline const PPointPlane3dC & operator+=(const PPointPlane3dC & p)
    { TFVectorC<RealT,4>::operator+=(p); return *this; }
    // Adds the values of the coordinates of the 'p' to this point/plane.
    
    inline const PPointPlane3dC & operator-=(const PPointPlane3dC & p)
    { TFVectorC<RealT,4>::operator-=(p); return *this; }
    // Subtracts the values of the coordinates of the 'p' from this
    // point/plane.
    
    inline const PPointPlane3dC & operator*=(const RealT alpha)
    { TFVectorC<RealT,4>::operator*=(alpha); return *this; }
    // Multiplies all coordinates by 'alpha'.

    inline const PPointPlane3dC & operator/=(const RealT alpha)
    { TFVectorC<RealT,4>::operator/=(alpha); return *this; }
    // Divides all coordinates by 'alpha'.
    
    inline PPointPlane3dC operator+(const PPointPlane3dC & p) const
    { return TFVectorC<RealT,4>::operator+(p); }
    // Returns the point/plane which is the sum of this point and the 'p'.

    inline PPointPlane3dC operator*(const RealT alpha) const
    { return TFVectorC<RealT,4>::operator*(alpha); }
    // Returns the point/plane which coordinates are multiplied by 'alpha'.

    inline PPointPlane3dC operator/(const RealT alpha) const
    { return TFVectorC<RealT,4>::operator/(alpha); }
    // Returns the point/plane which coordinates are divided by 'alpha'.
    
  protected:
    static TFVectorC<RealT,4> Point4d(RealT x,RealT y,RealT z,RealT w) {
      TFVectorC<RealT,4> ret;
      ret[0] = x;
      ret[1] = y;
      ret[2] = z;
      ret[3] = w;
      return ret;
    }
    // Assemble point from components
    
    inline PPointPlane3dC Translation(const PPointPlane3dC & newOrigin) const
    { return TFVectorC<RealT,4>::operator+(newOrigin); }
    // Returns the point/plane with projective coordinates related
    // to the new origin 'newOrigin'.
    
    
    //:------------------
    // Special functions.
    
    static inline 
    RealT AdjointDet(RealT b00, RealT b01, RealT b02,
                     RealT b10, RealT b11, RealT b12,
                     RealT b20, RealT b21, RealT b22) {
      return b00 * (b11*b22 - b12*b21)
	+ b10 * (b21*b02 - b22*b01)
	+ b20 * (b01*b12 - b02*b11);
    }
    // Computes the determinant of the matrix 3x3 created from b[ij] 
    // elements.
    

    friend inline PPointPlane3dC operator*(RealT lambda, const PPointPlane3dC & p);
    friend inline PPointPlane3dC operator/(RealT lambda, const PPointPlane3dC & p);
    friend istream & operator>>(istream & inS, PPointPlane3dC & point);
    friend class PProjection3dC;
  };

  inline
  PPointPlane3dC operator*(RealT lambda, const PPointPlane3dC & p)
  { return p * lambda; }
  // Returns the point/plane which is the 'lambda' multiplication of 'p'.

#if 0
  inline
  PPointPlane3dC operator/(RealT lambda, const PPointPlane3dC & p)
  { return ::operator/(lambda, (Point4dC) p ); }
  // Returns the point/plane which coordinates are the 'lambda' divided by the
  // corresponding coordinate of 'p'.
#endif
  
  ostream & operator<<(ostream & outS, const PPointPlane3dC & p);
  // Writes the point/plane 'p' into the output stream.

  istream & operator>>(istream & inS, PPointPlane3dC & p);
  // Reads the point/plane parameters from the input stream.
  
  BinOStreamC & operator<<(BinOStreamC & strm, const PPointPlane3dC & p);
  // Writes the 'point' into the output stream.
  
  BinIStreamC & operator>>(BinIStreamC & strm, PPointPlane3dC & p);
  // Reads the point parameters from the input stream.
  
}
#endif

