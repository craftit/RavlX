// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_OPENCVCONVERT_HH
#define RAVL_OPENCVCONVERT_HH

//! lib=RavlOpenCV
//! docentry="Ravl.API.Images.Converters.OpenCV"
//! example="exOpenCV.cc"

#include <opencv2/opencv.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/core/core_c.h>

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteBGRValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealBGRValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/DP/DataConv.hh"
#include "Ravl/Assert.hh"

namespace RavlImageN {
  
  using namespace RavlN;
  
//! userlevel=Normal
  
  template <class PixelT> 
  bool RavlImage2IplImage(const ImageC<PixelT> & src, IplImage*& pdest)
  {
    RavlIssueError("\nRavlImage2IplImage handles only these pixel types: ByteT, RealT, ByteRGBValueC, RealRGBValueC\n"
                   "Use DPTypeConvert() to convert to required type before calling RavlImage2IplImage.\n");
    return false;
  }
  //: Converts selected RAVL image types to OpenCVformat
  // Only converts the following RAVL image types:<br>
  // ByteT, RealT, ByteRGBValueC, RealRGBValueC

//:-
//! userlevel=Develop

  template <>
  bool RavlImage2IplImage(const ImageC<ByteT> & src, IplImage*& pdest);
  //: (template specialisation)

  template <>
  bool RavlImage2IplImage(const ImageC<ByteRGBValueC> & src, IplImage*& pdest);
  //: (template specialisation)

  template <>
  bool RavlImage2IplImage(const ImageC<RealT> & src, IplImage*& pdest);
  //: (template specialisation)

  template <>
  bool RavlImage2IplImage(const ImageC<RealRGBValueC> & src, IplImage*& pdest);
  //: (ttemplate specialisation)



//:-
//! userlevel=Normal

  template <class PixelT>
  bool IplImage2RavlImage(IplImage* src, ImageC<PixelT> & dest)
  {
    RavlAssertMsg((src->depth == (int)IPL_DEPTH_8U) || (src->depth == (int)IPL_DEPTH_8S) || (src->depth == (int)IPL_DEPTH_64F), "IplImage2RavlImage: can only convert IPL_DEPTH_8U|S and IPL_DEPTH_64F image depths");
    RavlAssertMsg((src->nChannels == 1) || (src->nChannels == 3), "IplImage2RavlImage: can only convert 1 or 3 channel formats");
    bool status(false);
    switch (src->depth) {
    case IPL_DEPTH_8U: case IPL_DEPTH_8S:
      switch (src->nChannels) {
      case 1:
        status = DPTypeConvert(ImageC<ByteT>(src->height, src->widthStep, (ByteT*)src->imageData, false), dest);
        break;
      case 3: {  // handled differently because in this case the pixels are not word aligned.
        ImageC<ByteBGRValueC> out(src->height, src->width);
        for (int r=0; r<src->height; ++r) {
          out.SetRowPtr(r, (ByteBGRValueC*)((src->imageData)+r*(src->widthStep)));
        }
        status = DPTypeConvert(out, dest);
        if(typeid(ImageC<ByteBGRValueC>) == typeid(dest))
          dest = dest.Copy();
      }
      break;
      default:
        return false;
      }
      break;
    case IPL_DEPTH_64F:
      switch (src->nChannels) {
      case 1:
        status = DPTypeConvert(ImageC<RealT>(src->height, src->widthStep/(8*src->nChannels), (RealT*)src->imageData, false), dest);
        if(typeid(ImageC<RealT>) == typeid(dest))
          dest = dest.Copy();
        break;
      case 3:
        status = DPTypeConvert(ImageC<RealBGRValueC>(src->height, src->widthStep/(8*src->nChannels), (RealBGRValueC*)src->imageData, false), dest);
        if(typeid(ImageC<RealBGRValueC>) == typeid(dest))
          dest = dest.Copy();
        break;
      default:
        return false;
      }
      break;
    case IPL_DEPTH_32F:
      switch (src->nChannels) {
      case 1:
        status = DPTypeConvert(ImageC<FloatT>(src->height, src->widthStep/(8*src->nChannels), (float*)src->imageData, false), dest);
        if(typeid(ImageC<FloatT>) == typeid(dest))
          dest = dest.Copy();
        break;
      case 3:
        status = DPTypeConvert(ImageC<BGRValueC<float> >(src->height, src->widthStep/(8*src->nChannels), (BGRValueC<float>*)src->imageData, false), dest);
        if(typeid(BGRValueC<float>) == typeid(dest))
          dest = dest.Copy();
        break;
      default:
        return false;
      }
      break;
    default:
      RavlAssertMsg(false, "IplImage2RavlImage: can only convert IPL_DEPTH_8U|S and IPL_DEPTH_64F image depths");
      return false;
    }
    //dest = ImageC<PixelT>(dest, IndexRange2dC(src->height,src->width));
    return status;
  }
  //: Converts these OpenCV images - BRG colour or grey-level, ByteT or RealT -  to any RAVL image type

}

#endif
