// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PLINE2D_HH
#define RAVL_PLINE2D_HH 1
///////////////////////////////////////////////////////////////////////
//! file="Ravl/Math/Geometry/Projective/2D/PLine2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Radek Marik"
//! date="26.05.1995"
//! docentry="Ravl.API.Math.Projective Geometry.2D"
//! rcsid="$Id$"

#include "Ravl/PPointLine2d.hh"
#include "Ravl/PPoint2d.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {

  class Point2dC;
  class Vector2dC;
  class LinePP2dC;
  class RealRange2dC;
  class IndexRange2dC;

  //! userlevel=Normal
  //: Line in 2D projective space
  // The class represents a line in 2 dimensional projective space.
  // A 2D line contains 3 real numbers which serve as its projective
  // coordinates.

  class PLine2dC
    : public PPointLine2dC
  {
  public:

    //:----------------------------------------------
    // Constructors, assigment, copy, and destructor.

    inline PLine2dC()
      : PPointLine2dC(0,0,0)
    {}
    //: Constructs the line whose coordinates are zeros, eg. non-existing
    //: projective line.

    inline PLine2dC(const Point2dC & p1, const Point2dC & p2)
      : PPointLine2dC(p1, p2)
    {}
    //: Constructs the projective line passing through two Euclidian
    //: points p1, p2.

    inline PLine2dC(const Point2dC  & p,const Vector2dC & v)
      : PPointLine2dC(p, v)
    {}
    //: Constructs the projective line passing through the Euclidian
    //: point 'p' and the Euclidian line direction vector 'v'.

    PLine2dC(const LinePP2dC & l, RealT s=1);
    //: Constructs the projective line from the Euclidian line 'l'.
    // s is the projective scale for the line endpoints

    inline PLine2dC(RealT p1, RealT p2, RealT p3)
      : PPointLine2dC(p1, p2, p3)
    {}
    //: Constructs the line (p1, p2, p3).
    // Example: if the normal to the line from the origin has length <code>r</code> and
    // orientation <code>&theta;</code>, the line can be constructed as:
    // <code>PLine2dC(cos(&theta;), sin(&theta;), -r)</code>

    inline PLine2dC(const Vector2dC & n, RealT d)
      : PPointLine2dC(n.Row(), n.Col(), d)
    {}
    //: Constructs the projective line from Euclidean parameters
    // Constructs the projective line equivalent to the Euiclidian
    // line determined by the equation n.Row()*x + n.Col()*y + d = 0.

    inline PLine2dC(const PPointLine2dC & p0, const PPointLine2dC & p1)
      : PPointLine2dC(p0, p1)
    {}
    //: Constructs the line passing through two projective points 'p0' and 'p1'.

    inline PLine2dC(const PPointLine2dC & p)
      : PPointLine2dC(p)
    {}
    //: Constructs the line from the point/line object 'p'.

    //:---------------------
    // Access to the object.

    Vector2dC Normal() const;
    //: Returns the normal of a corresponding Euclidian line.
    // If this projective line is ideal the function returns the zero vector.

    inline const PLine2dC & PLine2d() const
    { return *this; }
    //: Access to this constant object.

    inline PLine2dC & PLine2d()
    { return *this; }
    //: Access to the point.

    //:-------------------
    // Logical conditions.

    inline bool IsIdeal() const
    { return IsIdealLine(); }
    // Returns true iff this line is an ideal projective line.
    // The line is an ideal projective line if the absolute value of
    // the ratio Scale()/SumAbs() is smaller or equal to
    // the threshold 'Point3dC::relZero'.

    //:------------------------
    // Arithmetical operations.

#if 0
    inline const PLine2dC & operator*=(RealT alpha) {
      PPointLine2dC::operator*=(alpha);
      return *this;
    }
    // Multiplies all coordinates by 'alpha'.

    inline const PLine2dC & operator/=(RealT alpha) {
      PPointLine2dC::operator/=(alpha);
      return *this;
    }
    // Divides all coordinates by 'alpha'.

    inline PLine2dC operator*(RealT alpha) const
    { return PPointLine2dC::operator*(alpha); }
    // Returns the line which coordinates are multiplied by 'alpha'.

    inline PLine2dC operator/(RealT alpha) const
    { return PPointLine2dC::operator/(alpha); }
    // Returns the line which coordinates are divided by 'alpha'.
#endif

    //:--------------------------
    // Geometrical constructions.

    PPoint2dC Intersection(const PLine2dC & l) const;
    //: Returns the projective point that is the intersection of both lines.

    inline PPoint2dC ClosestPointToOrigin() const {
      return PPoint2dC(-P1()*P3(), -P2()*P3(), Sqr(P1()) + Sqr(P2()));
    }
    //: Compute closest point on line to origin

    RealT Distance(const PPoint2dC &p) const;
    //: Compute the distance of p from this line.

    bool ClipBy(LinePP2dC& PPLine, const RealRange2dC& Rectangle);
    //: Creates a line segment from this line, clipped by the rectangle
    // Returns false if line does not intersect rectangle, true otherwise

    bool ClipBy(LinePP2dC& PPLine, const IndexRange2dC& Rectangle);
    //: Creates a line segment from this line, clipped by the rectangle
    // Returns false if line does not intersect rectangle, true otherwise

    //-
#if 0
    //friend inline PLine2dC operator*(RealT lambda, const PLine2dC & p);
    //friend inline PLine2dC operator/(RealT lambda, const PLine2dC & p);
#endif
    friend istream & operator>>(istream & inS, PLine2dC & point);
  };


#if 0
  inline
  PLine2dC operator*(RealT lambda, const PLine2dC & l)
  { return l * lambda; }
  // Returns the line which is the 'lambda' multiplication of 'l'.

  inline
  PLine2dC operator/(RealT lambda, const PLine2dC & p)
  { return ::operator/(lambda, (PPointLine2dC) p ); }
  // Returns the line which coordinates are the 'lambda' divided by the
  // corresponding coordinate of 'l'.
#endif

  ostream & operator<<(ostream & outS, const PLine2dC & l);
  // Writes the projective line 'l' into the output stream.

  istream & operator>>(istream & inS, PLine2dC & l);
  // Reads the projective line parameters from the input stream.

}
#endif

