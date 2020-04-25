// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CONIC2D_HEADER
#define RAVL_CONIC2D_HEADER 1
//! author="Charles Galambos"
//! date="21/03/2004"
//! docentry="Ravl.API.Math.Geometry.2D"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Conic2d.hh"

#include "Ravl/Point2d.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  class Matrix3dC;
  class Vector2dC;
  class Matrix2dC;
  class LineABC2dC;
  class Ellipse2dC;
  
  //! userlevel=Normal
  //: Conic in 2d space.
  //  a * Sqr(row) + b * row * col + c * Sqr(col) + d * row + e * col + f = 0
  
  class Conic2dC {
  public:
    Conic2dC()
    {}
    //: Default constructor.
    // The conic is undefined.
    
    Conic2dC(const TFVectorC<RealT,6> &params)
      : p(params)
    {}
    //: Construct from parameter vector.
    //!param: params - Parameters, entry 0 = a, 1 = b, 2 = c, 3 = d, 4 = e, 5 = f 

    Conic2dC(RealT a,RealT b,RealT c,RealT d,RealT e,RealT f)
    { p[0] = a; p[1] = b; p[2] = c; p[3] = d; p[4] = e; p[5] = f;  }
    //: Construct from parameters
    
    Conic2dC(Matrix3dC &matrix);
    //: Construct from matrix C in projective ellipse equation x.T() * C * x = 0.
    // Thus matrix stores parameters as:<pre>
    //      (  a  b/2 d/2 )
    //  C = ( b/2  c  e/2 )
    //      ( d/2 e/2  f  )</pre>
    
    bool IsOnCurve(const Point2dC &pnt) const
    { return IsSmall(Residue(pnt)); }
    //: Is point on curve ?
    
    RealT Residue(const Point2dC &pnt) const {
      return 
	p[0] * Sqr(pnt[0]) + 
	p[1] * pnt[0] * pnt[1] +
	p[2] * Sqr(pnt[1]) +
	p[3] * pnt[0] +
	p[4] * pnt[1] + 
	p[5];
    }
    //: Compute the residue
    // Compute x.T() * C * x, where x is projective version of pnt. <br>
    // Hence gives a measure of distance of point from curve
    
    Matrix3dC C() const;
    //: Get the coefficient matrix. 'C'
    // Such that  x.T() * C * x = 0
    
    LineABC2dC Tangent(const Point2dC &pnt);
    //: Find the tangent at point 'pnt', where pnt is on the conic.
    
    const TFVectorC<RealT,6> &Parameters() const
    { return p; }
    //: Access parameter vector.
    // 0-a 1-b 2-c 3-d 4-e 5-f 
    
    bool AsEllipse(Ellipse2dC &ellipse) const;
    //: Represent conic as an ellipse.
    //!param: ellipse - Ellipse into which representation is written.
    //!return: true if conversion is possible.
    // Returns false if conic is a hyperbola or is
    // otherwise degenerate.
    
    bool EllipseParameters(Point2dC &centre,RealT &major,RealT &minor,RealT &angle) const;
    //: Compute various ellipse parameters.
    //!param: centre - Centre of ellipse.
    //!param: major - Size of major axis.
    //!param: minor - Size of minor axis
    //!param: angle - Angle of major axis.
    
  protected:
    bool ComputeEllipse(Point2dC &centre,Matrix2dC &mat) const;
    //: Compute ellipse parameters.
    // Assumes conic is ellipse, computes ellipse centre and returns remaining parameters as symmetric 2D matrix
    
    TFVectorC<RealT,6> p; // 0-a 1-b 2-c 3-d 4-e 5-f 
  };
  
  Conic2dC FitConic(const SArray1dC<Point2dC> &points);
  //: Fit a conic to a set of points.
  
  Conic2dC FitConic(const Array1dC<Point2dC> &points);
  //: Fit a conic to a set of points.
  
  bool FitEllipse(const SArray1dC<Point2dC> &points,Conic2dC &conic);
  //: Fit ellipse
  // Based on method presented in 'Numerically Stable Direct Least Squares Fitting of Ellipses' 
  // by Radim Halir and Jan Flusser.
  
  ostream &operator<<(ostream &s,const Conic2dC &obj);
  //: Write ellipse to text stream.
  
  istream &operator>>(istream &s,Conic2dC &obj);
  //: Read ellipse from text stream.
  
  BinOStreamC &operator<<(BinOStreamC &s,const Conic2dC &obj);
  //: Write ellipse to binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,Conic2dC &obj);
  //: Read ellipse from binary stream.
  
}


#endif
