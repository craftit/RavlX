// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_LMSOPTICFLOW_HEADER
#define RAVLIMAGE_LMSOPTICFLOW_HEADER 1
/////////////////////////////////////////////////////////////////////////
//! example="exLMSOptic.cc"
//! lib=RavlImageProc
//! rcsid="$Id$"
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSOpticFlow.hh"
//! userlevel=Normal
//! author="Bill Christmas"
//! date="07/12/1999"
//! docentry="Ravl.API.Images.Motion"

#include "Ravl/Image/Image.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Image/ByteYUVValue.hh"

namespace RavlImageN {
  
  //! docentry="Ravl.API.Images.Motion.Optical Flow"
  //: Least-mean-squared gradient method for optical flow
  // Computes optical flow field from spatial and temporal gradient images using simple least-mean-squared gradient method
  //<p>
  // The method finds, for each pixel, the least mean squared solution to the
  // motion constraint equation over an image patch (as described <a
  // href="http://www.ee.surrey.ac.uk/CVSSP/Publications/papers/christmas_bmvc98.pdf">here</a>).
  // It also generates a field of covariances of the motion vectors, and a field
  // of variances of the motion constraint equation error.  The default
  // parameters are:<br>
  //<pre>
  // noise (1.0),
  // region (9)
  //</pre>

  class LMSOpticFlowC {
  public:
    LMSOpticFlowC (bool verbose=false);
    // Constructor
    
    LMSOpticFlowC& SetRegionSize(IntT size)
    { region = size; return *this; }
    // Set size of patch over which motion error is minimised
    
    LMSOpticFlowC& SetNoiseLevel(RealT nNoise)
    { noise = nNoise; return *this; }
    // Set noise level for matrix inversion
    
    LMSOpticFlowC& SetGradientOrder(IntT order)
    { grad_order = order; return *this; }
    // Set order of spatial gradient estimator
    
    ImageC<Vector2dC> Estimate (const ImageC<Vector2dC> &grad,const ImageC<RealT> &dt);
    // LMS engine for method; operates on spatial and temporal gradient images

    ImageC<Vector2dC> Estimate (const PairC<ImageC<RealT> > &im);
    // Applies method to pair of (filtered) images using first-order differences

    const ImageC<Vector2dC> &Motion () const
    { return motion; }
    // Return field of motion vectors
    
    const ImageC<Matrix2dC> &MotionCov () const
    { return cov; }
    // Return field of matrices of motion estimate covariance
    
    const ImageC<RealT> &ErrorVar() const
    { return var; }
    // Return field of motion constraint equation error variance

    ImageC<Vector2dC> Eigenvalues() const;
    //: Return field of eigenvalues of spatial gradient outer product
    // Note, this is computed.
    
    static void DrawMotion(const ImageC<RealT> &im,const ImageC<Vector2dC> &motion,ImageC<ByteYUVValueC> &op);
    // Create an image where motion vectors are plotted as U/V components of
    // colour image, with 1st original image as Y component
    
    void DrawMotion(const ImageC<RealT> &im,ImageC<ByteYUVValueC> &op)
    { DrawMotion(im,motion,op); }
    // Create an image where motion vectors are plotted as U/V components of
    // colour image, with 1st original image as Y component
    
  private:
    /* params: */
    RealT noise;         // to help bad matrices invert nicely
    IntT region;         // regression is done over region of this size
    bool verbose;
    IntT grad_order;     // order of spatial gradient (1..3)
    
    /* images: */
    ImageC<Vector2dC> motion; // results of motion estimation
    ImageC<Matrix2dC> cov;   // covariance of motion vector estimation
    ImageC<RealT> var;        // variance of motion estimation errors
    ImageC<Vector2dC> lambda; // eigenvalues of gradient outer product
  };

  
} 
#endif
