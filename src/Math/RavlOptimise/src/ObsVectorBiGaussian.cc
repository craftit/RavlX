// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/ObsVectorBiGaussian.cc"

#include "Ravl/ObsVectorBiGaussian.hh"

namespace RavlN {

  //: Constructor
  ObsVectorBiGaussianBodyC::ObsVectorBiGaussianBodyC(const VectorC &nz,
						     const MatrixRSC &nNi,
						     RealT nvarScale,
						     RealT nchi2Thres)
    : ObsVectorBodyC(nz,nNi)
  {
    varInvScale = 1.0/nvarScale;
    chi2Thres = nchi2Thres;
    chi2Offset = (1.0 - varInvScale)*chi2Thres;
    outlier = previousOutlierFlag = true;
  }

  //: Constructor
  ObsVectorBiGaussianBodyC::ObsVectorBiGaussianBodyC(const VectorC &nz,
						     const MatrixRSC &nNi,
						     const VectorC &nzstep,
						     RealT nvarScale,
						     RealT nchi2Thres)
    : ObsVectorBodyC(nz,nNi,nzstep)
  {
    varInvScale = 1.0/nvarScale;
    chi2Thres = nchi2Thres;
    chi2Offset = (1.0 - varInvScale)*chi2Thres;
    outlier = previousOutlierFlag = true;
  }

  //: Return residual adjusted for robust aspects to the observation
  double ObsVectorBiGaussianBodyC::Residual(const VectorC &v,
					    const MatrixRSC &Ni)
  {
    // firstly store outlier flag
    previousOutlierFlag = outlier;

    // now compute residual as usual

    // compute N^-1*v
    VectorC Niv = Ni*v;

    // compute v^T*N^-1*v
    double residual = Niv.Dot(v);

    if ( residual <= chi2Thres )
      // inlier distribution: increment residual with squared innovation as
      // normal
      outlier = false;
    else { // residual > chi2Thres
      // adjust residual using outlier distribution offset
      residual = residual*varInvScale + chi2Offset;
      outlier = true;
    }

    return residual;
  }

  //: Adjust information matrix/vector term for any robustness
  bool ObsVectorBiGaussianBodyC::AdjustInformation(MatrixRSC &Aterm,
						   VectorC &aterm)
  {
    if ( outlier ) {
      // adjust increments to information matrix and vector
      Aterm *= varInvScale;
      aterm *= varInvScale;
    }

    return true;
  }

  //: Restore values are an aborted modification
  bool ObsVectorBiGaussianBodyC::Restore()
  {
    // restore previous outlier flag
    outlier = previousOutlierFlag;
    return true;
  }

  //: Get outlier flag
  bool ObsVectorBiGaussianBodyC::Outlier() const
  { return outlier; }

  //: Set observation to be an inlier
  void ObsVectorBiGaussianBodyC::SetAsInlier()
  { outlier = false; }

  //: Set observation to be an outlier
  void ObsVectorBiGaussianBodyC::SetAsOutlier()
  { outlier = true; }
}
