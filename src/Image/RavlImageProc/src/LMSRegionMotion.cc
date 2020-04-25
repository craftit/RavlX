// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSRegionMotion.cc"

#include "Ravl/Image/LMSRegionMotion.hh"
#include "LMSRegressionEngine.hh"

#include "Ravl/StdConst.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/VectorMatrix2d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter4.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2dIter5.hh"
#include "Ravl/Array2dIter6.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/LMSOpticFlow.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Image/SumRectangles.hh"
#include "Ravl/Image/ScaleValues.hh"
#include "Ravl/Image/SpatialDifference.hh"

namespace RavlImageN {
  
  // Constructor from image pair
  
  LMSRegionMotionC::LMSRegionMotionC (const PairC<ImageC<RealT> > &im, bool Verbose)
    : image(im),
      noise (1.0),
      verbose (Verbose)
  {
    // compute temporal image gradient
    ImageC<RealT>  dt = (image[1]-image[0]); 
    if (!dt.Rectangle().IsValid()) {
      cerr << "ERROR: Temporal difference failed.";
      return ;
    }
    //  compute spatial image gradients on mean image
    ImageC<RealT> centre ((image[0] + image[1]) / 2.0);  
    // remember coord convention:  y horizontal, x down
    
    ImageC<Vector2dC> grad;
    SpatialDifference(1,centre,grad);
    
    // compute outer products of grad I, dI/dt
    // =======================================
    ImageRectangleC rect(grad.Rectangle().Intersection(dt.Rectangle()));
    grad_grad = ImageC<Matrix2dC>(rect);
    grad_t = ImageC<Vector2dC>(rect);
    dt_sq = ImageC<RealT>(rect);
    //  ImageC<Vector2dC> grad_t(grad * -dt);  // can't get this to work 
    
    for(Array2dIter5C<Vector2dC,Vector2dC,RealT,RealT,Matrix2dC> it(grad_t,    // 1
								    grad,      // 2
								    dt,        // 3
								    dt_sq,     // 4
								    grad_grad, // 5
								    rect);it;it++) {
      it.Data2().OuterProduct(it.Data5());
      Mul(it.Data2(),-it.Data3(),it.Data1());
      it.Data4() = Sqr(it.Data3());
    }    
  }
  
  // Estimates motion for region with mask value "label"
  
  Vector2dC LMSRegionMotionC::Estimate(const ImageC<IntT> & mask, IntT label) {
    
    // find the sums of products witin region
    // ======================================
    Matrix2dC sum_grad_grad(0,0,0,0);
    Vector2dC sum_grad_t(0,0);
    RealT sum_dt_sq(0);
    region = 0;
    ImageRectangleC rect(grad_grad.Rectangle().Intersection(mask.Rectangle()));
    
    for(Array2dIter4C<Vector2dC,RealT,Matrix2dC,IntT> it(grad_t,    // 1
							 dt_sq,     // 2
							 grad_grad, // 3
							 mask,      // 4
							 rect);it;it++) {
      if(it.Data4() == label) {
	sum_grad_grad += it.Data3();
	sum_grad_t += it.Data1();
	sum_dt_sq += it.Data2();
	++region;
      }
    }
    
    // compute motion vector & stats
    // =============================
    if (region==0)  
      return Vector2dC(RavlConstN::nanReal, RavlConstN::nanReal);
    // else
    return LMSRegressionEngine (sum_grad_grad, sum_grad_t, sum_dt_sq, noise,region, lambda, var, cov);
  }
  
}
