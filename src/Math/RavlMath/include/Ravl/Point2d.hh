// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POINT2D_HEADER
#define RAVL_POINT2D_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Math/Geometry/Euclidean/2D/Point2d.hh"
//! lib=RavlMath
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Math.Geometry.2D"

#include "Ravl/FPoint.hh"

namespace RavlN {
  class Matrix3dC;
  template<class DataT> class PairC;
  template<class DataT> class SArray1dC;
  template<class DataT> class DListC;
  template<class DataT> class Slice1dC;
  //! userlevel=Normal
  //: 2D Point
  
  class Point2dC
    : public FPointC<2>
  {
  public:
    Point2dC()
    {}
    //: Default constructor.
    
    Point2dC(const TFVectorC<RealT,2> &base)
      : FPointC<2>(base)
    {}
    //: Base class constructor.
    
    explicit Point2dC(UIntT size)
    { RavlAssert(size == 2); }
    //: Construct with size.
    // This is used in some templates that are
    // designed to work with PointNdC as well as instances of FPointC.
    
    Point2dC(const FIndexC<2> &ind) 
      : FPointC<2>(ind)
    {}
    //: Construct from a 2d index.
    
    Point2dC(RealT v1,RealT v2) { 
      data[0] = v1;
      data[1] = v2;
    }
    //: Construct from two reals.
    
    RealT &X()
    { return data[0]; }
    //: First component of vector.
    //: Note use of this method is obsolete as its causes confusion with
    //: coordinate systems. In RAVL code its safer to use Row,Col exclusively.
    
    RealT X() const
    { return data[0]; }
    //: First component of vector.
    //: Note use of this method is obsolete as its causes confusion with
    //: coordinate systems.  In RAVL code its safer to use Row,Col exclusively.

    RealT &Y()
    { return data[1]; }
    //: Second component of vector.
    //: Note use of this method is obsolete as its causes confusion with
    //: coordinate systems.  In RAVL code its safer to use Row,Col exclusively.

    RealT Y() const
    { return data[1]; }
    //: Second component of vector.
    //: Note use of this method is obsolete as its causes confusion with
    //: coordinate systems.  In RAVL code its safer to use Row,Col exclusively.

    RealT &Row()
    { return data[0]; }
    //: First component of vector.
    
    RealT Row() const
    { return data[0]; }
    //: First component of vector.

    RealT &Col()
    { return data[1]; }
    //: Second component of vector.

    RealT Col() const
    { return data[1]; }
    //: Second component of vector.
    
    RealT Area2(const Point2dC& second, const Point2dC& third) const {
      // Area of tringle (*this, second, third) is equal to the area
      // of the tringle which the first point represents the origin
      // of the coordinate system. In fact the points 'aa' and 'bb'
      // represents two vectors and the computed area is equal to
      // the size of the cross product of these two vectors.
      //Point2dC aa(second - *this);   // O'Rourke 1.2
      //Point2dC bb(third  - *this);
      //return aa[0]*bb[1] - aa[1]*bb[0];
      return (second[0] - data[0]) * (third[1] - data[1]) - (second[1] - data[1]) * (third[0] - data[0]);
    }
    //: return twice the area contained by the three points
    
     RealT Angle() const
    { return atan2(data[1],data[0]); }
    //: Returns the oriented angle (rad) from the origin in
    //: the  range -PI to PI.

  };
  
  bool Normalise(const SArray1dC<Point2dC> &raw,SArray1dC<Point2dC> &norm,Matrix3dC &normMat);
  //: Normalise an array of points.
  // This finds the mean and variation of the Euclidean point positions. It corrects the mean to zero
  // and the average variation to 1.
  //!param: raw - Points to be normalised
  //!param: norm - Array to write normalised points into. A new array is always created as assigned to this handle. Note: If the array is of the required size it may be resused.
  //!param: normMat - Normalisation matrix. Multiplying the normalised points by this matrix will map them to the original space.
  //!return: true if normalisation found and applied.


  bool Normalise(const DListC<Point2dC> &raw,DListC<Point2dC> &norm,Matrix3dC &normMat);
  //: Normalise a list of points.
  // This finds the mean and variation of the Euclidean point positions. It corrects the mean to zero
  // and the average variation to 1.
  //!param: raw - Points to be normalised
  //!param: norm - List to write normalised points into. A new list is always created as assigned to this handle.
  //!param: normMat - Normalisation matrix. Multiplying the normalised points by this matrix will map them to the original space.
  //!return: true if normalisation found and applied.

  
  bool Normalise(const Slice1dC<Point2dC> &raw,Slice1dC<Point2dC> &norm,Matrix3dC &normMat);
  //: Normalise a slice of points.
  // This finds the mean and variation of the Euclidean point positions. It corrects the mean to zero
  // and the average variation to 1.
  //!param: raw - Points to be normalised
  //!param: norm - Slice to write normalised points into. A new slice is always created as assigned to this handle.
  //!param: normMat - Normalisation matrix. Multiplying the normalised points by this matrix will map them to the original space.
  //!return: true if normalisation found and applied.
}

#endif
