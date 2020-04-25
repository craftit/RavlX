// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNDAMENTALMATRIX2D_HEADER
#define RAVL_FUNDAMENTALMATRIX2D_HEADER 1
//! rcsid="$Id$"
//! lib=RavlMath
//! docentry="Ravl.API.Math.Projective Geometry.2D"
//! author="Charles Galambos"
//! file="Ravl/Math/Geometry/Projective/2D/FundamentalMatrix2d.hh"

#include "Ravl/SArray1d.hh"
#include "Ravl/PPoint2d.hh"
#include "Ravl/PLine2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/PProjection2d.hh"

namespace RavlN {
  class MatrixC;
  class LineABC2dC;

  //! userlevel=Normal
  //: Fundamental Matrix.

  class FundamentalMatrix2dC
    : public PProjection2dC
  {
  public:
    FundamentalMatrix2dC()
    {}

    FundamentalMatrix2dC(const TFMatrixC<RealT,3,3> &mat)
      : PProjection2dC(mat)
    {}

    FundamentalMatrix2dC(RealT a00,RealT a01,RealT a02,
			 RealT a10,RealT a11,RealT a12,
			 RealT a20,RealT a21,RealT a22)
      : PProjection2dC(a00,a01,a02,
		       a10,a11,a12,
		       a20,a21,a22)
    {}
    //: Construct from matrix elements.

    RealT Error(const PPoint2dC &p1,const PPoint2dC &p2) const;
    //: Compute the error distance from epipolar line.

    LineABC2dC EpipolarLine(const PPoint2dC &p1) const;
    //: Compute the epipolar line in image 2 from 'p1' in image 1.

    PLine2dC operator()(const PPoint2dC &pnt) const
    { return (*this) * pnt; }

    void NormaliseScale();
    //: Normalise scale.
    // Normalise the magnitude of the elements in the matrix by dividing them by the L2 norm of the matrix

    void MakeRank2();
    //: Make the matrix rank 2
    // Uses SVD method.

    static FundamentalMatrix2dC FitLinear(const SArray1dC<PPoint2dC> &pnts1,const SArray1dC<PPoint2dC> &pnts2);
    //: Compute the fundamental matrix from 2 sets of points.
    // This computes the fundamental matrix using a linear method (Sometimes called the normalised 8-point algorithm),
    // you need least 8 points, but more may be used to obtain a least squares fit.

    PPoint2dC Epipole1() const;
    //: Compute the epipolar point in the first image.
    // This is the point where the line connecting the two optical centres
    // intersect the first image plane.

    PPoint2dC Epipole2() const;
    //: Compute the epipolar point in the second image.
    // This is the point where the line connecting the two optical centres
    // intersect the second image plane.

    void CorrectCorrespondence(const PPoint2dC &ipnt1, const PPoint2dC &ipnt2, PPoint2dC &opnt1, PPoint2dC &opnt2);
    //: Compute corrected correspondences that minimize geometric error.
    // Subject to epipolar constraint x^TFx = 0. Implemented from Multiple View Geometry, 2nd ed,
    // Richard Hartley and Andrew Zisserman, Cambridge University Press, 2003, p318

  protected:

    static MatrixC BuildDesignMatrix(const SArray1dC<PPoint2dC> &pnts1,const Vector2dC &offset1,RealT scale1,
				     const SArray1dC<PPoint2dC> &pnts2,const Vector2dC &offset2,RealT scale2);
    //: Build a design matrix given two sets of points.

    static void NormalisePointSet(const SArray1dC<PPoint2dC> &pnts,Vector2dC &offset,RealT &scale);
    //: Compute the translation and scale normalisation
    // such that the mean of the points is zero and their average magnitude is equal to the average scale factor.

  };


}


#endif
