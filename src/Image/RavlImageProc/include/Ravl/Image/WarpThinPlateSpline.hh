// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WARPTHINPLATESPLINE_HEADER
#define RAVLIMAGE_WARPTHINPLATESPLINE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Scaling and Warping"
//! author="Charles Galambos"
//! date="22/11/2002"
//! file="Ravl/Image/Processing/Filters/Warp/WarpThinPlateSpline.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/Image/PixelMixer.hh"
#include "Ravl/Image/BilinearInterpolation.hh"

namespace RavlImageN {
  using namespace RavlN;

  //! userlevel=Develop
  //: Some common methods for Warping an image with a thin plate spline.
  
  class WarpThinPlateSplineBaseC {
  public:
    WarpThinPlateSplineBaseC(RealT nSigma,bool nFillBackground = true)
      : sigma(nSigma),
	fillBackground(nFillBackground)
    { 
      sigma2_2 = Sqr(sigma) * 2;
      log_sigma2 = Log(Sqr(sigma)); 
    }
    //: Constructor.
    
    inline RealT U(RealT dist2) const { 
      if(dist2 == 0)
	return 0; // Avoid Log(0).
#if 1
      return (dist2 * (Log(dist2) - log_sigma2))/ sigma2_2;
#else
      RealT dist = Sqrt(dist2);
      return Sqr(dist/sigma) * Log(dist/sigma);
#endif
    }
    // Compute U(x).
    
    void ComputeWUd(const Array1dC<Point2dC> &orgPos,const Point2dC &pos,const MatrixC &w,Point2dC &at) const;
    //: Calculate WUd.
    
    MatrixC ComputeW(const Array1dC<Point2dC> &orgPos,const Array1dC<Point2dC> &newPos) const;
    //: Compute weight matrix.
    
  protected:
    RealT sigma;
    RealT sigma2_2; // Sqr(sigma) * 2
    RealT log_sigma2; // Log(Sqr(sigma))
    bool fillBackground;
  };
  
  //! userlevel=Normal
  //: Warp an image with a thin plate spline.
  
  template<class InT,class OutT = InT,class MixerT = PixelMixerAssignC<OutT,OutT> >
  class WarpThinPlateSplineC 
    : public WarpThinPlateSplineBaseC
  {
  public:
    WarpThinPlateSplineC(RealT nSigma,bool nFillBackground = true,const MixerT &mix = MixerT())
      : WarpThinPlateSplineBaseC(nSigma,nFillBackground),
	mixer(mix)
    {}    
    //: Constructor.
    
    bool Apply(const ImageC<InT> &src,const Array1dC<Point2dC> &orgPos,const Array1dC<Point2dC> &newPos,ImageC<OutT> &ret) {
      if(ret.IsEmpty())
	ret = ImageC<OutT>(src.Frame());
      RealRange2dC irng(src.Frame());
      irng = irng.Expand(-1.1); // There's an off by a bit error somewhere in here...
      MatrixC w = ComputeW(newPos,orgPos); // We need a mapping from new positions to old.
      if(w.IsEmpty())
	return false;
      Point2dC rstart(ret.Frame().Origin());
      rstart += Point2dC(0.5,0.5);
      Point2dC at,pat;
      for(Array2dIterC<OutT> it(ret);it;) {
	at = rstart;
	do {
	  ComputeWUd(newPos,at,w,pat);
	  //cerr << " " << at << " => " << pat << "\n";
	  if(irng.Contains(pat)) {
	    OutT val;
	    BilinearInterpolation(src,pat - Point2dC(0.5,0.5),val);
	    mixer(*it,val);
	  } else {
	    if(fillBackground)
	      SetZero(*it);
	  }
	  at[1]++;
	} while(it.Next()) ;
	rstart[0]++;
      }
      return true;
    }
    //: Warp image.
    // This method will warp the input image 'src' so that the points in array 'orgPos' are in position
    // 'newPos' in the output image 'ret'.
    
  protected:
    MixerT mixer;
  };
}

#endif
