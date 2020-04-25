// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PPOINT2D_HH
#define RAVL_PPOINT2D_HH 1
/////////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/2D/PPoint2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.04.1995"
//! docentry="Ravl.API.Math.Projective Geometry.2D"
//! rcsid="$Id$"

#include "Ravl/PPointLine2d.hh"
#include "Ravl/Point2d.hh"

namespace RavlN {
  class Point2dC;
  class PLine2dC;
  class Matrix3dC;
  template<class DataT> class SArray1dC;
  
  //! userlevel=Normal
  //: Point in 2D projective space
  // The class represents a point in 2 dimensional projective space.
  // The point contains 3 real numbers which serve as its projective
  // coordinates. 
  
  class PPoint2dC
    : public PPointLine2dC
  {
  public:
    
    //:----------------------------------------------
    // Constructors, assigment, copy, and destructor.
    
    inline PPoint2dC()
      : PPointLine2dC(0,0,0)
    {}
    // Constructs the point whose coordinates are zeros, eg. non-existing
    // projective point.
    
    inline PPoint2dC(RealT row, RealT col)
      : PPointLine2dC(row, col, 1)
    {}
    // Constructs the point (row, col, 1).
    
    inline PPoint2dC(const Point2dC & p, RealT s=1)
      : PPointLine2dC(p.Row(), p.Col(), s)
    {}
    // Constructs the point (p.row, p.col, s).
    // s is the projective scale for the point
    
    inline PPoint2dC(RealT p1, RealT p2, RealT p3)
      : PPointLine2dC(p1, p2, p3)
    {}
    // Constructs the point (p1, p2, p3).
    
    inline PPoint2dC(const PPointLine2dC & p)
      : PPointLine2dC(p)
    {}
    // Creates the projective point from the projective point/line.
    
    inline PPoint2dC(const PPointLine2dC & l0, const PPointLine2dC & l1)
      : PPointLine2dC(l0, l1)
    {}
    // Constructs the projective point as intersection of two 
    // projective line 'l0' and 'l1'.
    
    inline PPoint2dC(const TFVectorC<RealT,3> &base)
      : PPointLine2dC(base)
    {}
    //: Construct from a basic vector.
    
    //:--------------------------
    // Access to the object.

    inline const PPoint2dC & PPoint2d() const
    { return *this; }
    // Access to this constant object.
  
    inline PPoint2dC & PPoint2d()
    { return *this; }
    // Access to the point.
    
    inline Point2dC Point2d() const
    { return Point2dC(Row(),Col()); }
    // Returns the point in Euclidian space related to this projective point.
    // It is assumed that the point is not ideal. The function
    // does not check if the point is ideal or not.

    // Logical conditions.
    //:-------------------
  
    inline bool IsIdeal() const
    { return IsIdealPoint(); }
    // Returns true iff this point is an ideal projective point.
    // The point is an ideal projective point if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to 
    // the threshold 'Point2dC::relZero'.

    inline
    bool IsColinear(const PPoint2dC & q2, const PPoint2dC & q3) const
    { RavlAssertMsg(0,"Not implemented. "); return true; }
    // Returns true iff this point and the points 'q2' and 'q3' are
    // colinear.
    
    //:------------------------
    // Arithmetical operations.

    inline const PPoint2dC & operator+=(const Vector2dC & trans) {
      (*this)[0] += trans[0] * Scale();
      (*this)[1] += trans[1] * Scale();
      return *this;
    }
    //: Translate this point by 'trans'
    
    inline const PPoint2dC & operator-=(const Vector2dC & trans) {
      (*this)[0] -= trans[0] * Scale();
      (*this)[1] -= trans[1] * Scale();
      return *this;
    }
    //: Translate this point by -'trans'
    
    
    //:-----------------------
    // Geometrical operations.

#if 0    
    inline PPoint2dC Translation(const PPoint2dC & newOrigin) const
    { return PPointLine2dC::Translation(newOrigin); }
    // Returns the point with projective coordinates related to the new origin
    // 'newOrigin'.
#endif
    
    PLine2dC PLine(const PPoint2dC & p) const;
    // Returns the projective line passing through this point
    // and the point 'p'.
    
    RealT EuclidDistance(const PPoint2dC &other) const
    { return Point2d().EuclidDistance(other.Point2d()); }
    //: Get the euclidean distance between this point and another.
    //!param: other - Point to measure distance to.
    //!return: Euclidean distance between points.
    
  protected:
#if 0
    inline PPoint2dC(const Point3dC & p)
      : PPointLine2dC(p)
    {}
    // Creates the projective point in 2D space from values of 
    // the point 'p'.
#endif
    
    friend inline PPoint2dC operator*(RealT lambda, const PPoint2dC & p);
    friend inline PPoint2dC operator/(RealT lambda, const PPoint2dC & p);
    friend istream &operator>>(istream & inS, PPoint2dC & point);
    friend class PProjection2dC;
  };
  
  
  bool Normalise(const SArray1dC<PPoint2dC> &raw,SArray1dC<PPoint2dC> &norm,Matrix3dC &normMat);
  //: Normalise an array of points.
  // This finds the mean and variation of euclidean point position. It corrects the mean to zero
  // and the average variation to 1.
  // This assumes there are no points at infinity.
  //!param: raw - Raw points to be normalised
  //!param: norm - Normalised points.
  //!param: normMat - Normalisation matrix 
  //!return: Normalisation found and applied.
  
#if 0
  inline
  PPoint2dC operator/(RealT lambda, const PPoint2dC & p)
  { return ::operator/(lambda, (PPointLine2dC) p ); }
  // Returns the point which coordinates are the 'lambda' divided by the
  // corresponding coordinate of 'p'.
#endif
  
  ostream & operator<<(ostream & outS, const PPoint2dC & point);
  // Writes the 'point' into the output stream.

  istream & operator>>(istream & inS, PPoint2dC & point);
  // Reads the point parameters from the input stream.
  
}
#endif
  
