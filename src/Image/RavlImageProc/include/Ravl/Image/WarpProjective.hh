// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WARPPROJECTIVE_HEADER
#define RAVLIMAGE_WARPPROJECTIVE_HEADER 1
//! rcsid="$Id$"
//! lib=RavlImageProc
//! docentry="Ravl.API.Images.Scaling and Warping"
//! author="Charles Galambos"
//! date="16/07/2002"
//! file="Ravl/Image/Processing/Filters/Warp/WarpProjective.hh"

#include "Ravl/Image/PixelMixer.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/LinePP2d.hh"
#include "Ravl/Projection2d.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Warp image with a projective transformation.
  // <p> By default, the coordinate system origin is taken to be the top left
  // corner of pixel[0][0].</p>
  //
  // <p> The output image frame is determined as follows:<ul>
  // <li> if the output image is gven, with a valid frame, use that frame
  // <li> else if the constructor specifies a valid frame, use that
  // <li> else find the bounding box of the input image in the output coords,
  // shrink it to the nearest pixel position, and use that.</ul>
  // </p>
  
  template <class InT, class OutT = InT,class MixerT = PixelMixerAssignC<InT,OutT>,class SampleT = SampleBilinearC<InT,OutT>  >
  class WarpProjectiveC
  {
  public:
    WarpProjectiveC()
    {}
    //: Default constructor.

    WarpProjectiveC(const IndexRange2dC &orec,
		    const Projection2dC &transform,
		    bool nFillBackground = true,
		    const MixerT &mix = MixerT())
      : trans(transform.Matrix()),
	iz(transform.IZ()),
	oz(transform.OZ()),
	rec(orec),
	fillBackground(nFillBackground),
	mixer(mix),
        pixelShift(0.5)
    { Init(); }
    //: Constructor from Projection2dC.
    //!param: orec - Rectangle for output image.
    //!param: transform - Projective transform to use
    //!param: nFillBackground - If true, background is filled with black.
    //!param: mix - Pixel mixer instance to use.
    // N.B. <code>transform</code> transforms points in the <i>input</i> image to the <i>output</i> image
    
    WarpProjectiveC(const Projection2dC &transform,
		    bool nFillBackground = true,
		    const MixerT &mix = MixerT())
      : trans(transform.Matrix()),
	iz(transform.IZ()),
	oz(transform.OZ()),
	fillBackground(nFillBackground),
	mixer(mix),
        pixelShift(0.5)
    { Init(); }
    //: Constructor from Projection2dC.
    
    WarpProjectiveC(const IndexRange2dC &orec,
		    const Matrix3dC &transform,
		    RealT niz = 1,
		    RealT noz = 1,
		    bool nFillBackground = true,
		    const MixerT &mix = MixerT())
      : trans(transform),
	iz(niz),
	oz(noz),
	rec(orec),
	fillBackground(nFillBackground),
	mixer(mix),
        pixelShift(0.5)
    { Init(); }
    //: Constructor from Matrix3dC.
    // Where orec is the size of the output rectangle.
    // See above for argument descriptions
    
    WarpProjectiveC(const Matrix3dC &transform,
		    RealT niz = 1,
		    RealT noz = 1,
		    bool nFillBackground = true,
		    const MixerT &mix = MixerT())
      : trans(transform),
	iz(niz),
	oz(noz),
	fillBackground(nFillBackground),
	mixer(mix),
        pixelShift(0.5)
    { Init(); }
    //: Constructor from Matrix3dC.
    // See above for argument descriptions
    
    void Apply(const ImageC<InT> &img,ImageC<OutT> &out);
    //: Warp image 'img' with the given transform and write it into 'out'
    // If the output image"<code>out</code>" has a valid frame, it will take precedence over the one specified in the constructor.
    
    ImageC<OutT> Apply(const ImageC<InT> &img) {
      ImageC<OutT> out(rec);
      Apply(img,out);
      return out;
    }
    //: Interpolate input image working rectangle into output image rectangle.
    // If the output rectangle is specified in the constructor, it is used.
    // Otherwise it is created from the bounding box of the projected input
    // image.
    
    void SetTransform(const Matrix3dC &transform) {
      trans = transform;
      Init();
    }
    //: Set projective transform.
    
    ImageRectangleC InputRectangle() const { 
      RealRange2dC orng(rec);
      ImageRectangleC ret(Project(orng.TopRight()),0);
      ret.Involve(Project(orng.TopLeft()));
      ret.Involve(Project(orng.BottomRight()));
      ret.Involve(Project(orng.BottomLeft()));
      return ret;
    }
    //: Get range of input rectangle that will be used.
    // Note: This may be larger than the actual input provided.
    
    Point2dC BackProject(const Point2dC &pnt) const;
    //: Transform a point from the source to destination.

    Point2dC Project(const Point2dC &pnt) const;
    //: Transform a point from the destination to source.

    void SetMidPixelCorrection(bool correction) {
      pixelShift =  correction ?  0.5 : 0.0;
      Init();
    }
    //: Set mid pixel correction flag.
    //!param: correction = true - coordinate system is at top l.h corner of pixel (0,0) (the default)
    //!param: correction = false - coordinate system is at centre of pixel (0,0)
    
  protected:
    void Init();
    
    Matrix3dC trans;
    Matrix3dC inv;
    RealT iz, oz;
    IndexRange2dC rec;
    bool fillBackground;
    MixerT mixer;
    RealT pixelShift;
  };

  template <class InT, class OutT,class MixerT,class SampleT>
  void WarpProjectiveC<InT,OutT,MixerT,SampleT>::Init() {
    // If mid-pixel correction is needed, transform is replaced by:
    //   shift 1/2 pixel; transform; shift back again.
    Matrix3dC shiftIn (1,0,-pixelShift/iz, 0,1,-pixelShift/iz, 0,0,1);
    Matrix3dC shiftOut(1,0, pixelShift/oz, 0,1, pixelShift/oz, 0,0,1);
    inv = shiftIn*trans.Inverse()*shiftOut;
  }
  
  template <class InT, class OutT,class MixerT,class SampleT>
  Point2dC WarpProjectiveC<InT,OutT,MixerT,SampleT>::BackProject(const Point2dC &pnt) const {
    Vector3dC vi = trans * Vector3dC(pnt[0],pnt[1],iz);
    return Point2dC(oz*vi[0]/vi[2],oz*vi[1]/vi[2]);
  }
  
  template <class InT, class OutT,class MixerT,class SampleT>
  Point2dC WarpProjectiveC<InT,OutT,MixerT,SampleT>::Project(const Point2dC &pnt) const {
    Vector3dC vo = inv * Vector3dC(pnt[0],pnt[1],oz);
    return Point2dC(iz*vo[0]/vo[2],iz*vo[1]/vo[2]);          
  }
  
  template <class InT, class OutT,class MixerT,class SampleT>
  void WarpProjectiveC<InT,OutT,MixerT,SampleT>::Apply(const ImageC<InT> &src,ImageC<OutT> &outImg) {
    
    // First we have to decide what our output image frame is
    if (outImg.Frame().IsEmpty()) {
      if (!rec.IsEmpty())  outImg = ImageC<OutT>(rec);
      else { // use i/p frame projected into o/p coords
        RealRange2dC irng(src.Frame());
        Point2dC pnt = BackProject(irng.TopRight());
        RealRange2dC trng(pnt,pnt);
        trng.Involve(BackProject(irng.TopLeft()));
        trng.Involve(BackProject(irng.BottomRight()));
        trng.Involve(BackProject(irng.BottomLeft()));
        IndexRange2dC oclip(Ceil(trng.TRow()),Floor(trng.BRow()),
                            Ceil(trng.LCol()),Floor(trng.RCol()));
        outImg = ImageC<OutT>(oclip);
      }
    }

    // Then we do the interpolation

    // pat:  o/p pt in o/p image cartesian coords
    // at:   o/p pt in i/p image homogeneous coords
    // ipat: o/p pt in i/p image cartesian coords
    // ldir: "horizontal" o/p increment in i/p image homogeneous coords
    // checkRange: valid region in i/p coords for truncated o/p pixel positions

    // set pat as top-left pixel in output image
    Point2dC pat(outImg.Frame().Origin());
    Vector3dC ldir(inv[0][1] * iz,inv[1][1] * iz,inv[2][1]);
    ImageRectangleC checkRange(src.Frame());
    checkRange.BRow() -= 1;
    checkRange.RCol() -= 1;
    OutT tmp;
    SampleT sampler;
    
    // Do simple check for each pixel that its contained in the input image.
    // This could be sped up by projecting the line corresponding to the
    // current row into the source image space, clipping it
    // and then projecting back into the output image and only iterate
    // along that bit of the line.
    // If the output maps entirely within input, we dont't have to do any
    // checking.  But we do.

    Array2dIterC<OutT> it(outImg);  
    for(;it;) { // iterate over rows
      Vector3dC at = inv * Vector3dC(pat[0],pat[1],oz);
      at[0] *= iz;
      at[1] *= iz;
      
      do { // iterate over cols
        Point2dC ipat(at[0]/at[2],at[1]/at[2]);
        if(checkRange.Contains(Index2dC(Floor(ipat[0]),Floor(ipat[1])))) {
          sampler(src,ipat,tmp);
          mixer(*it,tmp);
        } else
          if(fillBackground) SetZero(*it);
        at += ldir;
      } while(it.Next()) ;
      pat[0]++;
    }
  }
}


#endif
