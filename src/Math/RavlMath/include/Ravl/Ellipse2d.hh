// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ELLIPSE2D_HEADER
#define RAVL_ELLIPSE2D_HEADER 1
//! author="Charles Galambos"
//! date="6/4/2004"
//! docentry="Ravl.API.Math.Geometry.2D"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Ellipse2d.hh"

#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Affine2d.hh"

namespace RavlN {
  class Conic2dC;
  
  //! userlevel=Normal
  //: Ellipse 
  // This uses a form of inverted Euclidean representation, in contrast to the general 2-D conic <a href="RavlN.Conic2dC.html">Conic2dC</a>.<br>  The representation is the affine transform that transforms (scales, rotates, translates) a point from the unit circle to the "corresponding" point on the ellipse.
  
  class Ellipse2dC {
  public:
    Ellipse2dC()
    {}
    //: Default constructor.
    // The parameters of the ellipse are left undefined.
    
    Ellipse2dC(const TFVectorC<RealT,6> &conicParams);
    //: Create from conic parameters.
    //!param: conicParams - Conic parameters a to f, where a * Sqr(row) + b * row * col + c * Sqr(col) + d * row + e * col + f = 0
    
    Ellipse2dC(const Affine2dC &np)
      : p(np)
    {}
    //: Construct from affine transform from unit circle centered on the origin
    //!param: np - Transform from unit circle centered on the origin

    Ellipse2dC(const Matrix2dC &sr,const Vector2dC &off)
      : p(sr,off)
    {}
    //: Construct from affine transform from unit circle centered on the origin
    //!param: sr - scale rotation matrix.
    //!param: off - offset from origin
    
    Ellipse2dC(const Point2dC &centre,RealT major,RealT minor,RealT angle);
    //: Create an new ellipse
    //!param: centre - Centre of ellipse.
    //!param: major - Size of major axis. (at given angle)
    //!param: minor - Size of minor axis.
    //!param: angle - Angle of major axis.
    
    Point2dC Point(RealT angle) const
    { return p * Angle2Vector2d(angle); }
    //: Compute point on ellipse.
    
    const Affine2dC &Projection() const
    { return p; }
    //: Access as projection from unit circle centered on the origin
    
    inline Point2dC Centre() const
    { return p.Translation(); }
    //: Centre of the ellipse.
    
    bool IsOnCurve(const Point2dC &pnt) const;
    //: Is point on the curve ?
    
    bool EllipseParameters(Point2dC &centre,RealT &major,RealT &minor,RealT &angle) const;
    //: Compute various ellipse parameters.
    //!param: centre - Centre of ellipse.
    //!param: major - Size of major axis.
    //!param: minor - Size of minor axis
    //!param: angle - Angle of major axis.
    
    bool Size(RealT &major,RealT &minor) const;
    //: Compute the size of major and minor axis.
    //!param: major - Size of major axis.
    //!param: minor - Size of minor axis
    
  protected:    
    Affine2dC p; // Projection from unit circle.
  };
  
  bool FitEllipse(const SArray1dC<Point2dC> &points,Ellipse2dC &ellipse);
  //: Fit ellipse to points.
  //!param: points -  Set of points to fit to an ellipse.
  //!param: ellipse - Ellipse structure to store result in.
  // Based on method presented in 'Numerically Stable Direct Least Squares Fitting of Ellipses' 
  // by Radim Halir and Jan Flusser.
  
  //! docentry="Ravl.API.Math.Statistics;Ravl.API.Math.Geometry.2D"
  Ellipse2dC EllipseMeanCovariance(const Matrix2dC &covar,const Point2dC &mean,RealT stdDev = 1.0);
  //: Compute an ellipse from a 2d covariance matrix, mean, and standard deviation.
  // The ellipse is the contour of a 2-D Gaussian random variable which lies "stdDev" standard deviations from the mean.
  
  //:-
  //! docentry="Ravl.API.Math.Geometry.2D"
  
  ostream &operator<<(ostream &s,const Ellipse2dC &obj);
  //: Write ellipse to text stream.
  
  istream &operator>>(istream &s,Ellipse2dC &obj);
  //: Read ellipse from text stream.
  
  BinOStreamC &operator<<(BinOStreamC &s,const Ellipse2dC &obj);
  //: Write ellipse to binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,Ellipse2dC &obj);
  //: Read ellipse from binary stream.
  
  
}


#endif
