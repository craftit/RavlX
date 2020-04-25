// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLMATH_OBSVECTORBIGAUSSIAN_HEADER
#define RAVLMATH_OBSVECTORBIGAUSSIAN_HEADER 1
//! userlevel=Normal
//! author="Phil McLauchlan"
//! date="24/7/2002"
//! rcsid="$Id$"
//! docentry="Ravl.API.Pattern Recognition.Optimisation2"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObsVectorBiGaussian.hh"

#include "Ravl/ObsVector.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Robust bi-gaussian observation vector body.
  
  class ObsVectorBiGaussianBodyC
    : public ObsVectorBodyC
  {
  public:
    ObsVectorBiGaussianBodyC(const VectorC &z, const MatrixRSC &Ni,
			     RealT varScale, RealT chi2Thres);
    //: Constructor.

    ObsVectorBiGaussianBodyC(const VectorC &z, const MatrixRSC &Ni,
			     const VectorC &zstep,
			     RealT varScale, RealT chi2Thres);
    //: Constructor.

    virtual double Residual(const VectorC &v, const MatrixRSC &Ni);
    //: Return residual adjusted for any robust aspects to the observation

    virtual bool AdjustInformation(MatrixRSC &Aterm, VectorC &aterm);
    //: Adjust information matrix/vector term for any robustness

    virtual bool Restore();
    //: Restore values are an aborted modification

    bool Outlier() const;
    //: Get outlier flag

    void SetAsInlier();
    //: Set observation to be an inlier

    void SetAsOutlier();
    //: Set observation to be an outlier

  protected:
    RealT varInvScale; // inverse scaling of outlier covariance
    RealT chi2Thres; // cut-off point for chi^2 to switch to outlier
                      // distribution
    RealT chi2Offset; // adjustment to chi^2 residual for outlier distribution
    bool outlier; // whether the observation is an outlier (true) or an inlier
    bool previousOutlierFlag; // stored outlier flag
  };

  //! userlevel=Normal
  //! autoLink=on
  //: Robust bi-gaussian observation vector class
  // This class adds robustness to the ObsVectorC class, using a simple
  // bi-Gaussian error model with a narrow inlier Gaussian and a wider
  // outlier Gaussian distribution, as described
  // <a href="../../../LevenbergMarquardt/node1.html">here</a>.
  
  class ObsVectorBiGaussianC
    : public ObsVectorC
  {
  public:
    ObsVectorBiGaussianC(const VectorC &z, const MatrixRSC &Ni,
			 RealT varScale, RealT chi2Thres)
      : ObsVectorC(*new ObsVectorBiGaussianBodyC(z,Ni,varScale,chi2Thres))
    {}
    //: Constructor
    // varScale is the covariance scaling K parameter in the
    // <a href="../../../LevenbergMarquardt/node1.html">theory document</a>,
    // and chi2Thres is the chi-squared cutoff parameter.

    ObsVectorBiGaussianC(const VectorC &z, const MatrixRSC &Ni,
			 const VectorC &zstep,
			 RealT varScale, RealT chi2Thres)
      : ObsVectorC(*new ObsVectorBiGaussianBodyC(z,Ni,zstep,varScale,chi2Thres))
    {}
    //: Constructor
    // varScale is the covariance scaling K parameter in the
    // <a href="../../../LevenbergMarquardt/node1.html">theory document</a>,
    // and chi2Thres is the chi-squared cutoff parameter.
    // This constructor also allows you to specify a vector zstep of step
    // sizes for numerical differentiation with respect to the elements of z,
    // overriding the default step size (1e-6).

    ObsVectorBiGaussianC(const ObsVectorC &obs)
      : ObsVectorC(dynamic_cast<const ObsVectorBiGaussianBodyC *>(BodyPtr(obs)))
    {}
    //: Base class constructor.
    
    ObsVectorBiGaussianC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    ObsVectorBiGaussianC(ObsVectorBiGaussianBodyC &bod)
      : ObsVectorC(bod)
    {}
    //: Body constructor.
    
    ObsVectorBiGaussianC(const ObsVectorBiGaussianBodyC *bod)
      : ObsVectorC(bod)
    {}
    //: Body constructor.
    
    ObsVectorBiGaussianBodyC &Body()
    { return static_cast<ObsVectorBiGaussianBodyC &>(ObsVectorC::Body()); }
    //: Access body.

    const ObsVectorBiGaussianBodyC &Body() const
    { return static_cast<const ObsVectorBiGaussianBodyC &>(ObsVectorC::Body()); }
    //: Access body.
    
  public:

    bool Outlier() const
    { return Body().Outlier(); }
    //: Get outlier flag

    void SetAsInlier()
    { Body().SetAsInlier(); }
    //: Set observation to be an inlier

    void SetAsOutlier()
    { Body().SetAsOutlier(); }
    //: Set observation to be an outlier
  };
}  


#endif
