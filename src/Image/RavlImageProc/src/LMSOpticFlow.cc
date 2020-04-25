// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSOpticFlow.cc"

#include "Ravl/StdConst.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/VectorMatrix2d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter4.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2dIter5.hh"
#include "Ravl/Array2dIter7.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/LMSOpticFlow.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Image/SumRectangles.hh"
#include "Ravl/Image/ScaleValues.hh"
#include "Ravl/Image/SpatialDifference.hh"

#include "LMSRegressionEngine.hh"

namespace RavlImageN {


  // default constructor
  
  LMSOpticFlowC::LMSOpticFlowC (bool Verbose)
    : noise (1.0),
      region (9),
      verbose (Verbose),
      grad_order(1)
  {}

  ImageC<Vector2dC> LMSOpticFlowC::Estimate (const ImageC<Vector2dC> &grad, const ImageC<RealT> &dt)
  {
    // compute intersection of gradient fields
    ImageRectangleC rect = grad.Frame();
    rect.ClipBy(dt.Frame());
    
    // compute outer products of grad I, dI/dt
    // =======================================
    ImageC<Matrix2dC> grad_grad(rect);
    ImageC<Vector2dC> grad_t (rect);
    ImageC<RealT> dt_sq (rect);
    
    // grad_t = grad * -dt
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
    
    // find the sums of products
    // ======================
    
    IndexRange2dC mask(Index2dC(0,0),region,region); // Create mask of region by region centered on 0,0 
    
    ImageC<Matrix2dC> sum_grad_grad;
    SumRectangles(grad_grad,mask,sum_grad_grad);
    
    ImageC<Vector2dC> sum_grad_t;
    SumRectangles(grad_t ,mask,sum_grad_t);
    
    ImageC<RealT> sum_dt_sq;
    SumRectangles(dt_sq,mask,sum_dt_sq);
    
    // hence solve for motion and covariance fields
    // ============================================
    
    ImageRectangleC sum_rect (sum_grad_grad.Rectangle());
    if (!sum_rect.IsValid()) {
      if (verbose) cerr << "Images too small or filters too large to compute motion\n";
      motion = ImageC<Vector2dC>();
      return motion;
    }
    motion = ImageC<Vector2dC>(sum_rect);
    cov = ImageC<Matrix2dC>(sum_rect);
    var = ImageC<RealT>(sum_rect);
    lambda = ImageC<Vector2dC>(sum_rect);

    RealT min_val(RavlConstN::maxReal), max_val(-RavlConstN::maxReal);
    
    Index2dC pixel_min, pixel_max;
    Vector2dC sum_v(0.0, 0.0);
    RealT sum_v_sq(0.0);
    IntT area = region * region;
    for(Array2dIter7C<Matrix2dC,Vector2dC,RealT,Vector2dC,RealT,Matrix2dC,Vector2dC> mit(sum_grad_grad,  // 1
											 sum_grad_t,     // 2
											 sum_dt_sq,      // 3
											 lambda,         // 4
											 var,            // 5
											 cov,            // 6
											 motion          // 7
											 );mit;mit++) {
      // compute motion vector
      Vector2dC v = LMSRegressionEngine(mit.Data1(),mit.Data2(),mit.Data3(),noise,area,mit.Data4(),mit.Data5(),mit.Data6());
      mit.Data7() = v;
      if (mit.Data4()[0] > max_val) { max_val = mit.Data4()[0]; pixel_max = mit.Index(); }
      if (mit.Data4()[1] < min_val) { min_val = mit.Data4()[1]; pixel_min = mit.Index(); }
      sum_v += v;
      sum_v_sq += v.Dot(v);
    }
    if (verbose) {
      cerr << "Eigenvalue range: mins = " << min_val << ", maxes = " << max_val << '\n';
      const IntT a = sum_rect.Area();
      cerr << "Mean V = " << sum_v/a << '\n';
      cerr << "S.D. = " << sqrt(sum_v_sq/a - Vector2dC(sum_v/a).SqrNorm())<< '\n';
    }
    return motion;
  }
  
  
  ImageC<Vector2dC> LMSOpticFlowC::Estimate (const PairC<ImageC<RealT> > & im)
  {
    // compute spatial image gradient from mean image
    if (((IntT)im[0].Rows() < 2*grad_order+1) || ((IntT)im[0].Cols() < 2*grad_order+1)) {
      cerr << "Image too small for gradient calculation\n";
      return ImageC<Vector2dC>();
    }
    ImageC<RealT> aver(im[0].Frame());
    ImageC<RealT> diff(im[0].Frame());
    for(Array2dIter4C<RealT,RealT,RealT,RealT> it(aver,diff,im[0],im[1]);it;it++) {
      it.Data1() = (it.Data3() + it.Data4())/2;
      it.Data2() = it.Data3() - it.Data4();
    }
    
    ImageC<Vector2dC> grad;
    SpatialDifference(grad_order,aver,grad);
    
    // compute motion from spatial gradient and frame difference
    return Estimate(grad, diff);
  }

  
  // Return field of eigenvalues of spatial gradient outer product
  
  ImageC<Vector2dC> LMSOpticFlowC::Eigenvalues() const { 
    ImageC<Vector2dC> ret(lambda.Frame());
    RealT div = (RealT) (region*region);
    for(Array2dIter2C<Vector2dC,Vector2dC> it(ret,lambda);it;it++)
      it.Data1() = it.Data2() / div;
    return ret;
  }
  
  void LMSOpticFlowC::DrawMotion(const ImageC<RealT> &im,const ImageC<Vector2dC> &motion,ImageC<ByteYUVValueC> &op) {
    // find max motion value
    RealT max(0.0);
    for (Array2dIterC<Vector2dC> i(motion); i.IsElm(); i.Next())
      if (i.Data().SqrNorm()>max)  max = i.Data().SqrNorm();
    RealT factor(127.0/sqrt(max));
    // write motion as colour to o/p image, along with original image in faint
    // grey
    
    ImageC<RealT> scaled;
    ScaleOffsetValues(ImageC<RealT>(im,motion.Frame()),0.0,128.0,scaled);
    
    op = ImageC<ByteYUVValueC>(motion.Rectangle());
    for (Array2dIter3C<ByteYUVValueC,Vector2dC,RealT> it(op, motion, scaled);it;it++)
      it.Data1() = ByteYUVValueC((IntT) it.Data3() + 128,(IntT) (it.Data2()[0]*factor), (IntT) (it.Data2()[1]*factor));
    // add a colour circle key
    
    IndexC keysize(op.Cols()/10);
    if (keysize > op.Rows())  
      keysize = op.Rows();
    IndexRange2dC rect(op.TRow(), op.TRow()+keysize-1, op.RCol()-keysize+1, op.RCol());
    ImageC<ByteYUVValueC> key(op, rect);
    Index2dC centre(rect.Center());
    for (Array2dIterC<ByteYUVValueC> itk(key); itk.IsElm(); itk.Next()) {
      itk.Data().Y() = 255;
      itk.Data().U() = (SByteT) ((itk.Index()-centre)[0]*255.0/keysize);
      itk.Data().V() = (SByteT) ((itk.Index()-centre)[1]*255.0/keysize);
    }
  }

}
