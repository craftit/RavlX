// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef PPoint1d_HH
#define PPoint1d_HH
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/1D/PPoint1d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.04.1995"
//! docentry="Ravl.API.Math.Projective Geometry.1D"
//! rcsid="$Id$"

#include "Ravl/Point2d.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Point in 1D projective space
  // The class represents a point in 1 dimensional projective space.
  // The point contains 2 real numbers which serve as its projective
  // coordinates. 

  class PPoint1dC
    : private Point2dC
  {  
  public:
    //:----------------------------------------------
    //: Constructors, assigment, copy, and destructor.
    
    inline PPoint1dC()
      : Point2dC(0,0)
    {}
    // Constructs the point whose coordinates are zeros, eg. non-existing
    // projective point.
    
    inline PPoint1dC(RealT x)
      : Point2dC(x, 1)
    {}
    // Constructs the point (x, 1).
  
    inline PPoint1dC(RealT p1, RealT p2)
      : Point2dC(p1, p2)
    {}
    // Constructs the point (p1, p2).
    
    //:--------------------------
    //: Access to the coordinates.
  
    inline const RealT & operator[](IndexC i) const
    { return Point2dC::operator[](i.V()); }
    // Returns the value of the (i+1)-th projective coordinate.

    inline RealT &operator[](IndexC i)
    { return Point2dC::operator[](i.V()); }
    // Access the value of the (i+1)-th projective coordinate.
    
    inline RealT Scale() const
    { return P2(); }
    // Returns the value of the last projective coordinate P2().
    
    inline const RealT & P1() const
    { return (*this)[0]; }
    // Returns the value of the 1-st coordinate.
  
    inline const RealT & P2() const
    { return (*this)[1]; }
    // Returns the value of the 2-st coordinate.
  
    inline RealT & P1()
    { return (*this)[0]; }
    // Returns the value of the 1-st coordinate.
  
    inline RealT & P2()
    { return (*this)[1]; }
    // Returns the value of the 2-st coordinate.
    
    inline RealT Value() const {
      RavlAssertMsg(IsIdeal(),"an ideal point.");
      return P1()/Scale();
    }
    //: Returns the value of the Cartesian coordinate. 
    // The function does not check if the point is ideal or not.
    
    inline const PPoint1dC & PPoint1d() const
    { return *this; }    
    // Access to this constant object.
  
    inline PPoint1dC & PPoint1d()
    { return *this; }
    // Access to the point.
    
    //:-------------------
    // Logical conditions.
    
    inline bool operator==(const PPoint1dC & p) const
    { return Point2dC::operator==(p); }
    // Returns true iff 2 points are the same projective point.
    // Two projective points are equal iff their 2D Cartesian representants
    // are equal.

    inline bool operator!=(const PPoint1dC & p) const
    { return Point2dC::operator!=(p); }
    // Returns true iff 2 points are different projective points.
    // Two projective points are different iff they are not
    // equal in the sense of the operator '=='.
  
    inline bool IsIdeal() const   {
      const RealT sum = Point2dC::SumOfAbs();
      return  IsAlmostZero(sum) ? false : IsSmall(Scale(),sum);
    }
    // Returns true iff this point is an ideal projective point.
    // The point is an ideal projective point if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to 
    // the threshold 'Point2dC::relZero'.
  
    inline
    bool IsColinear(const PPoint1dC & q2, const PPoint1dC & q3) const
    { return true; }
    // Returns always true.
    
    inline bool IsValid() const
    { return !IsAlmostZero(Point2dC::SumOfAbs()); }
    // Returns true if the sum of absolute values of the projective coordinates
    // is bigger than the threshold 'Point2dC::zeroDistance'.
        
    //:------------------------
    // Arithmetical operations.
    
    inline RealT Sum() const
    { return Point2dC::Sum(); }
    // Returns the sum of coordinates.
    
    inline RealT SumOfAbs() const
    { return Point2dC::SumOfAbs(); }
    // Returns the sum of absolute value of projective coordinates.
    
    inline const PPoint1dC & operator+=(const PPoint1dC & point)   {
      Point2dC::operator+=(point);
      return *this;
    }
    // Adds the values of the coordinates of the 'point' to this point.
    
    inline const PPoint1dC & operator-=(const PPoint1dC & point)  {
      Point2dC::operator-=(point);
      return *this;
    }
    // Subtracts the values of the coordinates of the 'point' from this point.
    
    inline const PPoint1dC & operator*=(const RealT alpha) {
      Point2dC::operator*=(alpha);
      return *this;
    }
    // Multiplies both coordinates by 'alpha'.
    
    inline const PPoint1dC & operator/=(const RealT alpha) {
      Point2dC::operator/=(alpha);
      return *this;
    }
    // Divides both coordinates by 'alpha'.
    
    inline PPoint1dC operator+(const PPoint1dC & point) const
    { return Point2dC::operator+ (point); }
    // Returns the point which is the sum of this point and the 'point'.
    
    inline PPoint1dC operator*(const RealT alpha) const
    { return Point2dC::operator*(alpha); }
    // Returns the point which coordinates are multiplied by 'alpha'.
    
    inline PPoint1dC operator/(const RealT alpha) const
    { return Point2dC::operator/(alpha); }
    // Returns the point which coordinates are divided by 'alpha'.
    
  protected:
    
    inline PPoint1dC(const Point2dC & p)
      : Point2dC(p)
    {}
    // Creates the projective point in 1D space from values of 
    // the point 'p'.

    inline PPoint1dC(const RavlN::TFVectorC<RealT, 2> & p)
      : Point2dC(p)
    {}
    // Creates the projective point in 1D space from values of 
    // the point 'p'.
    
    friend inline PPoint1dC operator*(RealT lambda, const PPoint1dC & p);
    friend inline PPoint1dC operator/(RealT lambda, const PPoint1dC & p);
    friend istream & operator>>(istream & inS, PPoint1dC & point);
    friend class PProjection1dC;
  };

  inline
  PPoint1dC operator*(RealT lambda, const PPoint1dC & p)
  { return p * lambda; }
  // Returns the point which is the 'lambda' multiplication of 'p'.

#if 0
  inline
  PPoint1dC operator/(RealT lambda, const PPoint1dC & p)
  { return ::operator/(lambda, (Point2dC &) p ); }
  // Returns the point which coordinates are the 'lambda' divided by the
  // corresponding coordinate of 'p'.
#endif

  ostream & operator<<(ostream & outS, const PPoint1dC & point);
  // Writes the 'point' into the output stream.

  istream & operator>>(istream & inS, PPoint1dC & point);
  // Reads the point parameters from the input stream.
  
}
#endif


