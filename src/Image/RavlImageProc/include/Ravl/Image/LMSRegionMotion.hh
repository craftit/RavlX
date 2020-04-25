// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_LMSREGIONMOTION_HEADER
#define RAVLIMAGE_LMSREGIONMOTION_HEADER
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSRegionMotion.hh"
//! userlevel=Normal
//! rcsid="$Id$"
//! author="Bill Christmas"
//! date="07/12/1999"
//! example=exLMSRegionMotion.cc
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Motion"

#include "Ravl/Types.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Pair.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::PairC;
  using RavlN::Vector2dC;
  using RavlN::Matrix2dC;
#endif
  
  //! example="exLMSRegionMotion.cc"
  //! lib=LMSMotion
  //! docentry="Ravl.API.Images.Motion.Model Based"
  
  //: Computes a 2D motion vector from 2 images over a region using a least-mean-square fit
  //
  // This method computes the solution to the motion constraint equation over an
  // image patch (as described <a
  // href="http://www.ee.surrey.ac.uk/CVSSP/Publications/papers/christmas_bmvc98.pdf">here</a>).
  // The covariance matrix for the motion vector and the variance of the fit are
  // also available.  The default noise parameter value = 1.0.
  
  class LMSRegionMotionC {

  public:
    LMSRegionMotionC (bool verbose=false)
      : noise (1.0),
	verbose (verbose)
    {}
    
    LMSRegionMotionC (const PairC<ImageC<RealT> > &im, bool verbose=false);
    // Constructor from <i> filtered</i> image pair

    Vector2dC Estimate(const ImageC<IntT> & mask, IntT label);
    // Estimates motion for region with mask value "label"

    void SetNoiseLevel(RealT Noise)
    { noise = Noise; }
    // Sets noise level for matrix inversion
    
    Matrix2dC MotionCov() const
    { return cov; }
    // Returns  matrix of motion estimate covariance

    RealT ErrorVar() const
    { return var; }
    // Returns motion constraint equation error variance
  
    Vector2dC Eigenvalues() const
    { return lambda/region; }
    // Returns eigenvalues of spatial gradient outer product
    
  private:
    PairC<ImageC<RealT> > image;
    ImageC<Matrix2dC> grad_grad;
    ImageC<Vector2dC> grad_t;
    ImageC<RealT> dt_sq;
    Matrix2dC cov;      // covariance of motion vector estimation
    RealT var;           // variance of motion estimation errors
    Vector2dC lambda;    // eigenvalues of gradient outer product
    IntT region;         // size of region
    RealT noise;         // to help bad matrices invert nicely
    bool verbose;
  };

}

#endif
