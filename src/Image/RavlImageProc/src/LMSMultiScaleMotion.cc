// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Motion/LMSGradient/LMSMultiScaleMotion.cc"

#include "Ravl/Image/ImageRectangle.hh"
#include "Ravl/Image/LMSMultiScaleMotion.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/LMSOpticFlow.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Matrix2d.hh"
#include "Ravl/Image/SumRectangles.hh"
#include "Ravl/Image/ScaleValues.hh"
#include "Ravl/Image/SpatialDifference.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter5.hh"
#include "Ravl/Array2dIter6.hh"
#include "Ravl/StrStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {


  typedef PairC<ImageC<RealT> > ImPairT;
  
  // computes the LMS covariance matrix from known motion vectors
  static ImageC<Matrix2dC> GetCov(const PairC<ImageC<RealT> > &im,
				   const ImageC<Vector2dC> & vec,
				   const int windowsize,
				   const double n) {

    ImageC<Vector2dC> grad;
    ImageC<RealT> centre = (im[0] + im[1]) / 2.0; 
    SpatialDifference(1,centre,grad);
    ImageC<RealT> dt(im[1]-im[0]);
    
    // compute intersection of gradient fields
    ImageRectangleC rect(grad.Rectangle().Intersection(vec.Rectangle()));
    //cout<<rect<<endl;
    // compute outer products of grad I, dI/dt
    // =======================================
    ImageC<Matrix2dC> grad_grad(rect);
    ImageC<Vector2dC> grad_t (rect);
    ImageC<RealT> dt_sq (rect);
    ImageC<Vector2dC> motion_vector(rect); //added by frank
    for(Array2dIter5C<Vector2dC,Vector2dC,RealT,RealT,Matrix2dC> 
	  it(grad_t,    // 1
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
    
    // Create mask of region by region centered on 0,0 
    IndexRange2dC mask(Index2dC(0,0),windowsize,windowsize); 

    ImageC<Matrix2dC> sum_grad_grad;
    SumRectangles(grad_grad,mask,sum_grad_grad);
    
    ImageC<Vector2dC> sum_grad_t;
    SumRectangles(grad_t ,mask,sum_grad_t);
    
    ImageC<RealT> sum_dt_sq;
    SumRectangles(dt_sq,mask,sum_dt_sq);
    
    // hence solve for motion and covariance fields
    // ============================================
    ImageRectangleC sum_rect (sum_grad_grad.Rectangle().Intersection(vec.Rectangle()));
    if (!sum_rect.IsValid()) {
      cerr << "Images too small or filters too large to compute motion in GetCov\n"; 
      return ImageC<Matrix2dC>();
    }
    /*
      cout<<"grad_grad=   "<<grad_grad.Rectangle()<<endl;
      cout<<"windowsize=   "<<windowsize<<endl;
      cout<<"sum_rect=   "<<sum_rect<<endl;
      cout<<"vec_rect=   "<<vec.Rectangle()<<endl;
      cout<<"sum_grad_grad=   "<<sum_grad_grad.Rectangle()<<endl;
    */
    ImageC<Matrix2dC> cov(sum_rect);
    //NumImageC<RealT> var(sum_rect);
    ImageC<Vector2dC> lambda(sum_rect);
    //RealT min_val(StdConstC::maxReal), max_val(-StdConstC::maxReal);
    //PixelC pixel_min, pixel_max;
    //Matrix2d2C A_max;
    //Vector2dC sum_v(0.0, 0.0);
    // RealT sum_v_sq(0.0);
    RealT var;
    int N = windowsize*windowsize;
    RealT Sqr_N_n = Sqr(N*n);
    for(Array2dIter6C<Matrix2dC,Vector2dC,RealT,Vector2dC,Matrix2dC,Vector2dC>
	  mit(sum_grad_grad,  // 1
	      sum_grad_t,     // 2
	      sum_dt_sq,      // 3
	      lambda,         // 4
	      cov,            // 5
	      vec             // 6
	      );mit;mit++) {
      // compute the covariance matrix
      Matrix2dC E;
      EigenVectors(mit.Data1(),E,mit.Data4()); // do eigen decomposition
      RealT l_inv0 = mit.Data4()[0] / (Sqr(mit.Data4()[0]) + Sqr_N_n); // (pseudo) inverse
      RealT l_inv1 = mit.Data4()[1] / (Sqr(mit.Data4()[1]) + Sqr_N_n); // of eigenvalues
      Matrix2dC A_inv;
      // A_inv = E * Matrix2dC(l_inv0, 0.0, 0.0, l_inv1).MulT(E);
      Matrix2dC mtmp(l_inv0 * E[0][0] ,l_inv1 * E[0][1],
		     l_inv0 * E[1][0] ,l_inv1 * E[1][1]);
      MulT<RealT,2,2,2>(mtmp,E,A_inv);
      // and statistics
      var = (mit.Data3() + mit.Data6().Dot(mit.Data2())) / (N-2);
      if (var < 0.0)  var = 0; 
      mit.Data5() = A_inv * var; 
    }
    return cov;
  }
  
  /*
static void 
SaveLevel(IntT level, const PairC<NumImageC<RealT> > &im,
	  const NumImageC<RealT> &compensated, const LMSOpticFlowC &m,
	  const NumImageC<Vector2dC> &motion_total, RealT scale, RealT vec_subsample)
{
  using namespace StdIO;
  IntT factor(1);  for (IntT i=0; i<level; ++i) factor *= 2;

  // save current image pair that mtion is working on
  Save (StringC("in") + StringC(level) + "_0.pgm", im[0]);
  Save (StringC("in") + StringC(level) + "_1.pgm", im[1]);
  cerr << "Computed motion on image " << level <<": ";
  if (m.Motion().IsEmpty())  cerr << "empty motion field\n";
  else {
    cerr << "motion field rectangle is "<< m.Motion().Rectangle() <<"\n";
    // display centre of motion image
    ImageRectangleC mr(im[0].Rectangle());
    if (mr.IsValid()) {
      //      IntT sub(((mr.Rnum()>mr.Cnum())?mr.Rnum():mr.Cnum())/30);
      PSImageC ps(StringC("motion") + level + ".ps",
		  mr, (factor>vec_subsample) ? 1 : vec_subsample/factor);
      ps.SetSizeMm (300);
      ps.PrintImage(im[0].ClipToFitByte());
      ps.SetPenColour(1,1,0);
      Index2dC pt(50,50);  pt /= factor;  // reference pixel coords in yellow
      ps.DrawEllipse(pt, Matrix2d2C(5.0/factor,0,0,5.0/factor));  
      ps.SetFont("Helvetica", (IntT) (8.0/factor));
      ps.DrawText(pt, StringC(" (") + pt[0] + "," + pt[1] + ")"); 
      ps.SetPenColour(0,0,0);
      ps.PrintArrows(m.Motion()*scale);   // current contribution in black
      ps.SetPenColour(1,0,0);
      ps.PrintArrows(motion_total*scale); // total motion in red
      ps.SetPenColour(0,1,0);
      ps.PrintEllipses(m.MotionCov()*scale);    // motion covariance in green
    }
    OStreamC mot (StringC("motion") + level);
    mot << m.Motion() <<"\n";
    OStreamC tot (StringC("total") + level);
    tot << motion_total <<"\n";
    OStreamC cov (StringC("cov") + level);
    cov << m.MotionCov() <<"\n";
    OStreamC var (StringC("var") + level);
    var << m.ErrorVar() <<"\n";
    NumImageC<RealT> sd(m.ErrorVar().Rectangle());
    for (Image2IterC<RealT, RealT> i(sd, m.ErrorVar()); i.IsElm(); i.Next()) 
      i.Data1() = sqrt(i.Data2());
    Save (StringC("sd") + level + ".pgm", sd);
    if (compensated.IsValid()) {
      Save (StringC("comp")+level+".pgm", ImageC<RealT>(compensated));
      Save (StringC("err")+level+".pgm", ImageC<RealT>(im[0]-compensated+128));
    }
  }
}
*/


  static bool IsInside(const Vector2dC &v, const ImageRectangleC &r) { 
    return ((floor(v.X()) >= r.TRow()) && (ceil(v.X()) <= r.BRow())
	    && (floor(v.Y()) >= r.LCol()) && (ceil(v.Y()) <= r.RCol()));
  }
  

  static ImageC<RealT> 
  motion_comp (const ImageC<RealT> &im, const ImageC<Vector2dC> &motion){
    ImageC<RealT> comp(im.Rectangle());
    // check that image and motion field are conformant
    if (im.Rectangle() != motion.Rectangle()) {
      cerr << "Image rectangle:  " << im.Rectangle()
	   << "\nmotion rectangle: " << motion.Rectangle() << "\n";
      RavlAssert(0);
    }
    IndexRange2dC frame = im.Rectangle();
#if 0
    frame.BRow()--;
    frame.RCol()--;
#endif
    
    for (Array2dIter2C<RealT,Vector2dC> it(comp,motion);it.IsElm();it.Next()) {
      // find best compensated pixel in image & write it as current pixel.
      Index2dC p(it.Index());
      Vector2dC rp(p[0],p[1]);
      Vector2dC p_shifted(rp + it.Data2());
      if (IsInside(p_shifted, frame)) {
#if 1
	IndexC r(p_shifted.X());
	IndexC r1(ceil (p_shifted.X()));
	RealT alpha = p_shifted.X() - (RealT) r;
	IndexC c(p_shifted.Y());
	IndexC c1(ceil (p_shifted.Y()));
	RealT beta = p_shifted.Y() - (RealT) c;
	it.Data1() = (1.0-alpha) * ((1.0-beta) * im[r][c] +  beta * im[r][c1])
	  + alpha * ((1.0-beta) * im[r1][c] + beta * im[r1][c1]);
#else
        BilinearInterpolation(im,p_shifted,it.Data1());
#endif
      }
      else it.Data1() = im[im.Frame().Nearest(p_shifted)];
    }
    return comp;
  }

  
  // Scale up image by factor of 2, embedded in another rectangle
  static ImageC<Vector2dC> 
  scaled_up (const ImageC<Vector2dC> &motion, const ImageRectangleC &rect) {
    if (motion.IsEmpty()) {
      ImageC<Vector2dC>ret (rect);
      ret.Fill(Vector2dC(0.0,0.0));
      return ret;
    }
    // scale up image x 2 and embed result in larger image  
    ImageC<Vector2dC> scaled(rect);
    scaled.Fill(Vector2dC(0,0));
    ImageRectangleC local (ImageRectangleC(
             motion.Frame().Origin()*2, motion.Frame().End()*2));
    for (IndexC r=local.TRow(); r<=local.BRow(); ++r) {
      if (r%2 == 0)
	for (IndexC c=local.LCol(); c<=local.RCol(); ++c) {
	  if (c%2 == 0)  scaled[r][c] = motion[r/2][c/2]*2;
	  else scaled[r][c] = scaled[r][c] = motion[r/2][c/2] + motion[r/2][c/2+1];
	}
      else
	for (IndexC c=local.LCol(); c<=local.RCol(); ++c) {
	  if (c%2 == 0)  scaled[r][c] = motion[r/2][c/2] + motion[r/2+1][c/2];
	  else scaled[r][c] = (motion[r/2][c/2] + motion[r/2][c/2+1]
			       + motion[r/2+1][c/2] + motion[r/2+1][c/2+1]) /2;
	}
    }
    return scaled;
  }
  
  
  static ImPairT 
  Subsample(const ImPairT &im_pair, ConvolveSeparable2dC<RealT> &filter) {
    ImPairT binimated;
    for (IntT i=0; i<=1; ++i) {
      ImageC<RealT> filt (filter.Apply(im_pair[i]));
      binimated[i] = ImageC<RealT>(ImageRectangleC(
	(filt.TRow()+1)/2, filt.BRow()/2, (filt.LCol()+1)/2, filt.RCol()/2));
      for (Array2dIterC<RealT> it(binimated[i]); it; it++)
	it.Data() = filt[it.Index()*2];
#if DODEBUG
      if (i==0)  cerr << "in: " << im_pair[0].Rectangle()
		      << "; filtered: " << filt.Rectangle()
		      << "; subsampled: " << binimated[0].Rectangle() << '\n';
#endif
    }
    return binimated;
  }
  
  
  ImageC<Vector2dC> 
  LMSMultiScaleMotionC::BottomLevel(const ImPairT &im_pair, IndexC times) {
    optical_flow.Estimate(im_pair);
    //if (verbose) SaveLevel (top_level-times, im_pair, ImageC<RealT>(), optical_flow, optical_flow.Motion(), scale, vec_subsample);
    return optical_flow.Motion();
  }

  //: Default constructor.
  // Creates motion estimate with a 2:1 antialias filter.
  
  LMSMultiScaleMotionC::LMSMultiScaleMotionC() 
    : verbose(false),
      scale(1),
      vec_subsample(1),
      top_level(-1) // i.e. uninitialised
  {
    Array1dC<RealT> coeffs(-4,4);
    StrIStreamC ("-4 4 -0.03008995 0.01247519 0.13510284 0.29130294 0.36395804 0.29130294 0.13510284 0.01247519 -0.03008995") >> coeffs;
    // preserve d.c. level
    RealT norm(0);
    for (IndexC i(coeffs.IMin()); i<=coeffs.IMax(); ++i) norm += coeffs[i];
    for (IndexC i(coeffs.IMin()); i<=coeffs.IMax(); ++i) coeffs[i] /= norm; 
    filter = ConvolveSeparable2dC<RealT>(coeffs,coeffs);
  }
  
  LMSMultiScaleMotionC::LMSMultiScaleMotionC(const ConvolveSeparable2dC<RealT>& Filter, const LMSOpticFlowC& Flow)
    : filter(Filter),
      verbose(false),
      scale(1),
      vec_subsample(1),
      optical_flow(Flow),
      top_level(-1) // i.e. uninitialised
  {}
  
  
  ImageC<Vector2dC> LMSMultiScaleMotionC::
  EstimateRecursive (const PairC<ImageC<RealT> > &im_pair, IndexC times) { 
    // If this is the bottom level (or if anything untoward happens), we
    // return the optical flow field for this image pair.
    if ((times == 1) || (im_pair[0].Rows() < 5) || (im_pair[0].Cols() < 5)) {
      /*
	NumImageC<Vector2dC> cm(optical_flow.Estimate(im_pair).Pad(im_pair[0].Rectangle()));
	NumImageC<RealT> mc(motion_comp(im_pair[1], cm));
	optical_flow.Estimate(impairT(im_pair[0], mc));
	if (verbose)
	SaveLevel (top_level-times, im_pair, mc, optical_flow, 
	optical_flow.Motion()+cm, scale, vec_subsample);
	return optical_flow.Motion()+cm;
      */
      optical_flow.Estimate(im_pair);
      //if (verbose) SaveLevel (top_level-times, im_pair, NumImageC<RealT>(), optical_flow, optical_flow.Motion(), scale, vec_subsample);
      return optical_flow.Motion();
    }
    // Otherwise, we compute the motion recursively.
    // =============================================
    // filter and subsample the pair of images
    ImPairT binimated(Subsample(im_pair, filter));
    if (binimated[0].IsEmpty())  return BottomLevel(im_pair, times);
    // recursive call to do next level down, using subsampled image
    ImageC<Vector2dC> current_motion (EstimateRecursive (binimated, times-1));
    // up-convert resulting motion
    ImageC<Vector2dC>
      motion_total(scaled_up(current_motion, im_pair[1].Rectangle()));
    // use this motion field to motion-compensate 2nd frame
    ImageC<RealT> compensated (motion_comp(im_pair[1], motion_total));
    // calculate residual motion
    optical_flow.Estimate(ImPairT(im_pair[0], compensated));
    // check for empty field
    if (!optical_flow.Motion().IsValid())  return BottomLevel(im_pair, times);
    // add residual motion to previous scaled-up motion
    ImageRectangleC intersection(motion_total.Rectangle().Intersection(optical_flow.Motion().Rectangle()));
    for (Array2dIter2C<Vector2dC,Vector2dC> i(motion_total,
         optical_flow.Motion(), intersection); i; ++i)
      i.Data1() += i.Data2();
    //if (verbose) SaveLevel (top_level-times, im_pair, compensated, optical_flow, motion_total, scale, vec_subsample);
    motvec_map=motion_total;
    filtered = im_pair;
    return motion_total;
  }
  
  ImageC<Vector2dC> LMSMultiScaleMotionC::
  Estimate (const PairC<ImageC<RealT> > &im_pair, IndexC times) {
    top_level = times; // records max no. of levels
    return EstimateRecursive (im_pair, times);
  }
  
  ImageC<Matrix2dC> LMSMultiScaleMotionC::MotionCov(const int windowsize, const RealT n) {
    //cout<<"region = "<< windowsize<<endl;
    //cout<<"noise = "<< n<<endl;
    ImageC<Matrix2dC> cov = GetCov(filtered, motvec_map, windowsize, n);
    //cout<<"cov in Multiscale MotionCov() = "<<cov.Rectangle()<<endl;
    return cov;
  }
  
}
