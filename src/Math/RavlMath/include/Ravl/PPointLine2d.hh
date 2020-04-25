// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPOINTLINE2D_HH
#define RAVL_PPOINTLINE2D_HH 1
/////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/2D/PPointLine2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! docentry="Ravl.API.Math.Projective Geometry.2D"
//! date="26.06.1995"
//! rcsid="$Id$"

#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Point3d.hh"

namespace RavlN {
  //! userlevel=Normal
  //: Point - line in 2D projective space
  // The class represents a point/line in 2 dimensional projective space.
  // An object contains 3 real numbers which serve as its projective
  // coordinates. An object can be treated as a point and its dual line
  // and vice versa in any time. This feature reflects the dual relationship
  // of points and lines in 2D projective space. The index of coordinates
  // starts from 0.
  
  class PPointLine2dC
    : public TFVectorC<double,3>
  {  
  public:
    using TFVectorC<double,3>::operator *;
    using TFVectorC<double,3>::operator -;

    //:----------------------------------------------
    // Constructors, assignment, copy, and destructor.
    
    inline PPointLine2dC()
    { Fill(0); }
    // Constructs the point/line whose coordinates are zeros, eg. non-existing
    // projective object.

    inline PPointLine2dC(RealT row, RealT col)
    {
      (*this)[0] = row;
      (*this)[1] = col;
      (*this)[2] = 1.0;
    }
    // Constructs the point/line (row, col, 1). row,col represent either
    // point Cartesian coordinates or a line normal expressed as vector
    // Cartesian coordinates.
    
    inline PPointLine2dC(const Point2dC & p)
    {
      (*this)[0] = p.Row();
      (*this)[1] = p.Col();
      (*this)[2] = 1.0;
    }
    // Constructs the projective point/line (p.Row(), p.Col(), 1) related
    // to the Euclidian point 'p'.
    
    inline PPointLine2dC(const Vector2dC & v)
    {
      (*this)[0] = v.Row();
      (*this)[1] = v.Col();
      (*this)[2] = 1.0;
    }
    // Constructs the projective point/line (v.Row(), v.Col(), 0) related
    // to the Euclidian direction 'v'.
    
    inline PPointLine2dC(RealT p1, RealT p2, RealT p3)
    {
      (*this)[0] = p1;
      (*this)[1] = p2;
      (*this)[2] = p3;
    }
    // Constructs the point/line (p1, p2, p3).
    
    inline PPointLine2dC(const PPointLine2dC & p0, const PPointLine2dC & p1)
      // Ref.: pp 66 - 67
    {
      (*this)[0] = Det(p0.P2(), p0.P3(),
                       p1.P2(), p1.P3());
      
      (*this)[1] = Det(p0.P3(), p0.P1(),
                       p1.P3(), p1.P1());
      
      (*this)[2] = Det(p0.P1(), p0.P2(),
                       p1.P1(), p1.P2());
    }
    // Constructs the projective point/line determined by two 
    // projective lines/points, resp.
    
    inline PPointLine2dC(const Point3dC & p)
      : TFVectorC<double, 3u>(p)
    {}
    // Creates the projective point/line in 2D space from values of 
    // the point 'p'.
    
    inline PPointLine2dC(const RavlN::TFVectorC<RealT, 3> & p)
      : TFVectorC<double, 3u>(p)
    {}
    // Creates the projective point/line in 2D space from values of 
    // the point 'p'.
    
    //:---------------------------
    // Access to the object items.
    
    inline RealT Scale() const
    { return P3(); }
    // Returns the value of the last projective coordinate P3().
    
    inline const RealT & P1() const
    { return (*this)[0]; }
    // Returns the value of the 1st coordinate.
    
    inline const RealT & P2() const
    { return (*this)[1]; }
    // Returns the value of the 2nd coordinate.
    
    inline const RealT & P3() const
    { return (*this)[2]; }
    // Returns the value of the 3rd coordinate.
    
    inline RealT & P1()
    { return (*this)[0]; }
    // Returns the value of the 1st coordinate.

    inline RealT & P2()
    { return (*this)[1]; }
    // Returns the value of the 2nd coordinate.

    inline RealT & P3()
    { return (*this)[2]; }
    // Returns the value of the 3rd coordinate.
    
    inline RealT Row() const {
      RavlAssertMsg(!IsIdealPoint(),"an ideal point or a bad projective line.");
      return P1()/Scale();
    }
    // Returns the value of the  1. Cartesian coordinate. The function
    // does not check if the point/line is ideal or not.

    inline RealT Col() const {
      RavlAssertMsg(!IsIdealPoint(),"an ideal point or a bad projective line.");
      return P2()/Scale();
    }
    // Returns the value of the 2. Cartesian coordinate. The function
    // does not check if the point/line is ideal or not.
    
    inline const PPointLine2dC & PPointLine2d() const
    { return *this; }
    // Access to this constant object.
    
    inline PPointLine2dC & PPointLine2d()
    { return *this; }
    // Access to the point/line.
     
    //:-------------------
    // Logical conditions.
    
    inline bool IsIdealPoint() const{
      const RealT sum = SumOfAbs();
      return IsAlmostZero(sum) ? false : IsAlmostZero(Scale()/sum);
    }
    // Returns true iff this point/line represents an ideal projective point.
    // The point is an ideal projective point if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to 
    // the threshold 'Point3dC::relZero'.
    
    inline bool IsIdealLine() const {
      RavlAssertMsg(IsAlmostZero(Scale()),"a bad projective line.");
      return IsAlmostZero((RavlN::Abs(P1())+RavlN::Abs(P2()))/Scale());
    }
    // Returns true iff this point/line represents an ideal projective plane.
    // The plane is an ideal projective plane if the absolute value of
    // the ratio (Abs(P1)+Abs(P2())/Scale() is smaller or equal to 
    // the threshold 'Point3dC::relZero'.

    inline bool IsValid() const
    { return !IsAlmostZero(SumOfAbs()); }
    // Returns true if the sum of absolute values of the projective coordinates
    // is bigger than the threshold 'Point3dC::zeroDistance'.
    
    //:------------------------
    // Arithmetical operations.
    
    inline PPointLine2dC Intersection(const PPointLine2dC &l2) const {
      return PPointLine2dC(l2.P3()*P2() - l2.P2()*P3(),
			   l2.P1()*P3() - l2.P3()*P1(),
			   l2.P2()*P1() - l2.P1()*P2());
    }
    //: Return the intersection of 'l2' with this one.
    
    //:-----------------------
    // Geometrical operations.
    
    inline PPointLine2dC PPointLine2d(const PPointLine2dC & p) const
    { return PPointLine2dC(*this, p); }
    
    // Returns the projective line/point determined by this point/line
    // and the point/line 'p'.
    
    TFVectorC<RealT,3> operator+(const PPointLine2dC & o) const
    { return TFVectorC<RealT,3>::operator+(o); }
    //: Element wise addition
    
    TFVectorC<RealT,3> operator-(const PPointLine2dC & o) const
    { return TFVectorC<RealT,3>::operator-(o); }
    //: Element wise substraction
    
    TFVectorC<RealT,3> operator*(const PPointLine2dC & o) const
    { return TFVectorC<RealT,3>::operator*(o); }
    //: Element wise multiplication
    
    TFVectorC<RealT,3> operator/(const PPointLine2dC & o) const
    { return TFVectorC<RealT,3>::operator/(o); }
    //: Element wise devision
    
  protected:

    //:------------------
    // Special functions.
    
    // Returns the point/line with projective coordinates related
    // to the new origin 'newOrigin'.
    
    static inline RealT Det(const RealT p00, const RealT p01, 
			    const RealT p10, const RealT p11) 
    { return p00*p11-p01*p10; }
    
    friend inline PPointLine2dC operator*(RealT lambda, const PPointLine2dC & p);
    friend inline PPointLine2dC operator/(RealT lambda, const PPointLine2dC & p);
    
    friend istream & operator>>(istream & inS, PPointLine2dC & point);
    friend class PProjection2dC;
  };
  
  inline PPointLine2dC operator*(RealT lambda, const PPointLine2dC & p)
  { return PPointLine2dC(p[0] * lambda,p[1] * lambda,p[2] * lambda); }
  // Returns the point/line which is the 'lambda' multiplication of 'p'.
  
  inline
  PPointLine2dC operator/(RealT lambda, const PPointLine2dC & p)
  { return PPointLine2dC(lambda/p.P1(),lambda/p.P2(), lambda/p.P3()); }
  // Returns the point/line which coordinates are the 'lambda' divided by the
  // corresponding coordinate of 'p'.

  ostream & operator<<(ostream & outS, const PPointLine2dC & p);
  // Writes the point/line 'p' into the output stream.

  istream & operator>>(istream & inS, PPointLine2dC & p);
  // Reads the point/line parameters from the input stream.
  
}
#endif
