// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WARPAFFINE_HEADER
#define RAVLIMAGE_WARPAFFINE_HEADER 1
///////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Images.Scaling and Warping"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! rcsid="$Id$"
//! date="16/07/2002"
//! file="Ravl/Image/Processing/Filters/Warp/WarpAffine.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Affine2d.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/Image/PixelMixer.hh"
#include "Ravl/Image/BilinearInterpolation.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Performs an affine warp of an image using bi-Linear Interpolation.
  //
  
  template <class InT, class OutT = InT,class WorkT = OutT,class MixerT = PixelMixerAssignC<WorkT,OutT>,class SampleT = SampleBilinearC<InT,WorkT> >
  class WarpAffineC
  {
  public:
    WarpAffineC(const ImageRectangleC &outRec,const Affine2dC &ntrans,bool nFillBackground = true,const MixerT &mix = MixerT())
      : rec(outRec),
	trans(ntrans),
	fillBackground(nFillBackground),
	mixer(mix),
	useMidPixelCorrection(true)
    {}
    //: Constructor.
    // 'outRec' is the output rectangle.
    // 'ntrans' is the transform to use. Maps output image pixel to one in input.
    
    WarpAffineC(const ImageRectangleC &outRec,bool nFillBackground = true,bool doMidPointCorrection = true,const MixerT &mix = MixerT())
      : rec(outRec),
	fillBackground(nFillBackground),
	mixer(mix),
	useMidPixelCorrection(doMidPointCorrection)
    {}
    //: Constructor.
    // 'outRec' is the output rectangle.
    
    void Apply(const ImageC<InT> &inImg,ImageC<OutT> &outImg);
    //: Interpolate input image working rectangle into output image.
    // If the supplied transformation transforms points in image A to
    // corresponding positions in image B
    // (i.e. transforms coordinate system of B to that of A),
    // Then this method warps image B to coordinate system of A.
    
    ImageC<OutT> Apply(const ImageC<InT> &img) {
      ImageC<OutT> out(rec);
      Apply(img,out);
      return out;
    }
    //: Interpolate input image working rectangle into
    //: output image rectangle.
    // The output rectangle is specified in the constructor.
    // If the supplied transformation transforms points in image A to
    // corresponding positions in image B
    // (i.e. transforms coordinate system of B to that of A),
    // Then this method warps image B to coordinate system of A.
    
    void SetTransform(const Affine2dC &transform) {
      trans = transform;
    }
    //: Set the current transform.
    
    ImageRectangleC InputRectangle() const { 
      RealRange2dC orng(rec);
      RealRange2dC rret(trans * orng.TopRight(),0);
      rret.Involve(trans * orng.TopLeft());
      rret.Involve(trans * orng.BottomRight());
      rret.Involve(trans * orng.BottomLeft());
      return rret.IndexRange();
    }
    //: Get range of input rectangle that will be used.
    // Note: This may be larger than the actual input provided.

    ImageRectangleC InputRectangle(const ImageRectangleC &outRec) const { 
      RealRange2dC orng(outRec);
      RealRange2dC rret(trans * orng.TopRight(),0);
      rret.Involve(trans * orng.TopLeft());
      rret.Involve(trans * orng.BottomRight());
      rret.Involve(trans * orng.BottomLeft());
      return rret.IndexRange();
    }
    //: Get range of input rectangle that is need to fill 'outRec'
    
    ImageRectangleC OutputRectangle(const ImageRectangleC &inrec) const { 
      RealRange2dC irng(inrec);
      Affine2dC itrans = trans.I();
      RealRange2dC rret(itrans * irng.TopRight(),0);
      rret.Involve(itrans * irng.TopLeft());
      rret.Involve(itrans * irng.BottomRight());
      rret.Involve(itrans * irng.BottomLeft());
      return rret.IndexRange();
    }
    //: Get range of output rectangle that will required to get get all of 'inrec'.
    
    MixerT &Mixer() 
    { return mixer; }
    //: Access mixer class.
    
    void SetOutputRectangle(const IndexRange2dC &rng)
    { rec = rng; }
    //: Set the output rectangle.
    
    void SetMidPixelCorrection(bool correction)
    { useMidPixelCorrection = correction; }
    //: Set mid pixel correction flag.
    //!param: correction = true - coordinate system is at top l.h corner of pixel (0,0)
    //!param: correction = false - coordinate system is at centre of pixel (0,0)
    
  protected:
    ImageRectangleC rec;   // Output rectangle.
    Affine2dC trans;       // Transform.
    bool fillBackground;   // Fill background with zero ?
    MixerT mixer;
    bool useMidPixelCorrection;
  };
  
  template <class InT, class OutT,class WorkT,class MixerT,class SampleT >
  void WarpAffineC<InT,OutT,WorkT,MixerT,SampleT>::Apply(const ImageC<InT> &src,ImageC<OutT> &outImg) {
    RealRange2dC irng(src.Frame());
    irng = irng.Expand(-1.001); // There's an off by a bit error somewhere in here...
    RealRange2dC orng(rec);
    if(!outImg.IsValid())
      outImg = ImageC<OutT>(rec);
    else
      orng = RealRange2dC(outImg.Frame());
    
    //cerr << "Trans0=" << trans * orng.TopRight() << " from " << orng.TopRight() << "\n";
    
    const Matrix2dC &srm = trans.SRMatrix();
    Vector2dC ldir(srm[0][1],srm[1][1]);
    Vector2dC sdir(srm[0][0],srm[1][0]);
    Affine2dC localTrans = trans;
    if (useMidPixelCorrection)
      // Equivalent to: shift 1/2 pixel; transform; shift back again.
      localTrans.Translate(srm*Vector2dC(0.5,0.5)+Vector2dC(-0.5,-0.5));
    Point2dC lstart = localTrans * Point2dC(orng.Origin());
    Array2dIterC<OutT> it(outImg);
    
    WorkT tmp;
    SampleT sampler;
    if(irng.Contains(localTrans * orng.TopRight()) &&
       irng.Contains(localTrans * orng.TopLeft()) &&
       irng.Contains(localTrans * orng.BottomRight()) &&
       irng.Contains(localTrans * orng.BottomLeft())) {
      // Output maps entirely within input, so we don't have to do any checking!
      for(;it;) {
	Point2dC pat = lstart;
	do {
	  sampler(src,pat,tmp);
	  mixer(*it,tmp);
	  pat += ldir;
	} while(it.Next()) ;
	lstart += sdir;
      }
      return;
    }    
#if 0
    Vector2dC endv(0,((RealT) orng.Cols()));
    // This attempts to be clever project the line back into
    // the source space, clipping it and the projecting it back
    // again.
    for(;it;) {
      Point2dC pat = lstart;
      LinePP2dC rline(pat,endv);
      if(!rline.ClipBy(irng)) {
	it.NextRow();
	lstart += sdir;
	continue;
      }
      // Map clipped line back into output image.
      Point2dC sp = ilocalTrans * rline.P1();
      Point2dC ep = ilocalTrans * rline.P2();
      
      int c = (((int) sp[1]) - outImg.LCol()).V();
      if(fillBackground) {
	for(;c >= 0;c--,it++)
	  SetZero(*it);
      } else
	it.NextCol(c);
      int ce = (((int) ep[1]) - outImg.LCol()).V();
      const OutT *end = &(*it) + ce;
      for(;&(*it) < end;it.NextCol()) {
	sampler(src,pat,*it);
	pat += ldir;
      }
      if(fillBackground) {
	for(;it.IsColElm();it.NextCol())
	  SetZero(*it);
      } else
	it.NextRow();
      lstart += sdir;
    }
#else 
    // Do simple check for each pixel that its contained in the input image.
    for(;it;) {
      Point2dC pat = lstart;
      if(fillBackground) {
	do {
	  if(irng.Contains(pat)) {
	    sampler(src,pat,tmp);
	    mixer(*it,tmp);
	  } else
	    SetZero(*it);
	  pat += ldir;
	} while(it.Next()) ;
      } else {
	do {
	  if(irng.Contains(pat)) {
	    sampler(src,pat,tmp);
	    mixer(*it,tmp);
	  }
	  pat += ldir;
	} while(it.Next()) ;
      }
      lstart += sdir;
    }
    
#endif    
  }
  
}
#endif
