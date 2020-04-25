// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MEANCOVARIANCE3D_HEADER
#define RAVL_MEANCOVARIANCE3D_HEADER 1
//! author="Charles Galambos"
//! date="30/4/2002"
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/MeanCovariance3d.hh"

#include "Ravl/FMeanCovariance.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Mean covariance in 3 dimensions.

  class MeanCovariance3dC
    : public FMeanCovarianceC<3>
  {
  public:
    MeanCovariance3dC()
    {}
    //: Default constructor.
    // Creates zero mean and zero covariance matrix representing
    // the fixed-dimensional set containing no data points.

    MeanCovariance3dC(const FMeanCovarianceC<3> & mc)
      : FMeanCovarianceC<3>(mc)
    {}
    //: Construct from base class.
    
    MeanCovariance3dC(const FVectorC<3> & point)
      : FMeanCovarianceC<3>(point)
    {}
    //: Constructor 
    // Creates the mean vector and zero covariance matrix representing
    // the data set containing just one data point. 
    
    MeanCovariance3dC(const FMeanC<3> & mean)
      : FMeanCovarianceC<3>(mean)
    {}
    //: Creates the mean vector and zero covariance matrix representing
    //: the data set represented by the 'mean'.
    
    MeanCovariance3dC(SizeT  n, 
		     const FVectorC<3> & mean, 
		     const FMatrixC<3,3> & ncov)
      : FMeanCovarianceC<3>(n,mean,ncov)
    {}
    //: Creates the mean vector and zero covariance matrix representing
    //: the data set containing 'n' points and represented by the 'mean'
    //: and the covariance matrix 'cov'. 
    
  };
}


#endif
