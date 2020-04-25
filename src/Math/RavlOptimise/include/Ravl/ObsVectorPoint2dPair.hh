// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSVECTORPOINT2DPAIR_HEADER
#define RAVLMATH_OBSVECTORPOINT2DPAIR_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2.Cases"
//! example="Homography2dFitTest.cc"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObsVectorPoint2dPair.hh"

#include "Ravl/ObsVectorBiGaussian.hh"
#include "Ravl/Vector2d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Homogeneous 2D point pair observation vector body
  
  class ObsVectorPoint2dPairBodyC
    : public ObsVectorBodyC
  {
  public:
    ObsVectorPoint2dPairBodyC(const Vector2dC &z1, const MatrixRSC &Ni1,
			      const Vector2dC &z2, const MatrixRSC &Ni2);
    //: Constructor.

  private:
    static VectorC BuildZ(const Vector2dC &z1, const Vector2dC &z2);
    //: static method to build observation vector from sub-vectors

    static MatrixRSC BuildNi(const MatrixRSC &Ni1, const MatrixRSC &Ni2);
    //: static method to build observation inverse covariance from sub-matrices
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Homogeneous 2D point pair observation vector class
  // When you have a matched pair of 2D points, used for instance for computing
  // the motion between two images, and errors are to be modelled in both
  // image points, this class will be useful. It provides a constructor to
  // stick together the two points into one 4-vector, and similarly construct
  // a 4x4 inverse covariance matrix from 2x2 sub-blocks.
  // <p>
  // This is the non-robust version. To generate robustified observations of
  // point pairs use the ObsVectorBGPoint2dPairC class.
  class ObsVectorPoint2dPairC
    : public ObsVectorC
  {
  public:
    ObsVectorPoint2dPairC(const Vector2dC &z1, const MatrixRSC &Ni1,
			  const Vector2dC &z2, const MatrixRSC &Ni2)
      : ObsVectorC(*new ObsVectorPoint2dPairBodyC(z1,Ni1,z2,Ni2))
    {}
    //: Constructor
    // This sticks the individual point observation vectors and inverse
    // covariance matrices together into a single vector and matrix.
    
    ObsVectorPoint2dPairC(const ObsVectorC &obs)
      : ObsVectorC(dynamic_cast<const ObsVectorPoint2dPairBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
    ObsVectorPoint2dPairC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  public:
    ObsVectorPoint2dPairC(ObsVectorPoint2dPairBodyC &bod)
      : ObsVectorC(bod)
    {}
    //: Body constructor.
    
    ObsVectorPoint2dPairBodyC &Body()
    { return static_cast<ObsVectorPoint2dPairBodyC &>(ObsVectorC::Body()); }
    //: Access body.

    const ObsVectorPoint2dPairBodyC &Body() const
    { return static_cast<const ObsVectorPoint2dPairBodyC &>(ObsVectorC::Body()); }
    //: Access body.
    
  public:
  };

  //! userlevel=Develop
  //: Robust bi-gaussian homogeneous 2D point pair observation vector body
  
  class ObsVectorBGPoint2dPairBodyC
    : public ObsVectorBiGaussianBodyC
  {
  public:
    ObsVectorBGPoint2dPairBodyC(const Vector2dC &z1, const MatrixRSC &Ni1,
				const Vector2dC &z2, const MatrixRSC &Ni2,
				RealT varScale, RealT chi2Thres);
    //: Constructor.

  private:
    static VectorC BuildZ(const Vector2dC &z1, const Vector2dC &z2);
    //: static method to build observation vector from sub-vectors

    static MatrixRSC BuildNi(const MatrixRSC &Ni1, const MatrixRSC &Ni2);
    //: static method to build observation inverse covariance from sub-matrices
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Robust bi-gaussian homogeneous 2D point pair observation vector class
  // When you have a matched pair of 2D points, used for instance for computing
  // the motion between two images, and errors are to be modelled in both
  // image points, this class will be useful. It provides a constructor to
  // stick together the two points into one 4-vector, and similarly construct
  // a 4x4 inverse covariance matrix from 2x2 sub-blocks.
  // <p>
  // This class uses the bi-Gaussian error model, with a narrow Gaussian
  // distribution for inlier point pairs, and a wider distribution for
  // outliers. To generate non-robust observations of point pairs use the
  // ObsVectorPoint2dPairC class.
  class ObsVectorBGPoint2dPairC
    : public ObsVectorBiGaussianC
  {
  public:
    ObsVectorBGPoint2dPairC(const Vector2dC &z1, const MatrixRSC &Ni1,
			    const Vector2dC &z2, const MatrixRSC &Ni2,
			    RealT varScale, RealT chi2Thres)
      : ObsVectorBiGaussianC(*new ObsVectorBGPoint2dPairBodyC(z1,Ni1,z2,Ni2,varScale,chi2Thres))
    {}
    //: Constructor
    // This sticks the individual point observation vectors and inverse
    // covariance matrices together into a single vector and matrix.
    // It also passes the parameters of the bi-Gaussian distribution onto
    // the relevant ObsVectorBiGaussianC constructor.

    ObsVectorBGPoint2dPairC(const ObsVectorBiGaussianC &obs)
      : ObsVectorBiGaussianC(dynamic_cast<const ObsVectorBGPoint2dPairBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
    ObsVectorBGPoint2dPairC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  public:
    ObsVectorBGPoint2dPairC(ObsVectorBGPoint2dPairBodyC &bod)
      : ObsVectorBiGaussianC(bod)
    {}
    //: Body constructor.
    
    ObsVectorBGPoint2dPairBodyC &Body()
    { return static_cast<ObsVectorBGPoint2dPairBodyC &>(ObsVectorBiGaussianC::Body()); }
    //: Access body.

    const ObsVectorBGPoint2dPairBodyC &Body() const
    { return static_cast<const ObsVectorBGPoint2dPairBodyC &>(ObsVectorBiGaussianC::Body()); }
    //: Access body.
    
  public:
  };
}  


#endif
