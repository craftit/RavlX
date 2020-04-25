// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_BILINEAR_HEADER
#define RAVLIMAGE_BILINEAR_HEADER
//! docentry="Ravl.API.Images.Scaling and Warping"
//! author="Charles Galambos"
//! date="24/01/2001"
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/BilinearInterpolation.hh"

#include "Ravl/Image/Image.hh"

#define RAVL_USE_QUICKREAL2INT 1

#if RAVL_USE_QUICKREAL2INT
#include "Ravl/QInt.hh"
#endif

namespace RavlImageN {

  //! userlevel=Normal
  
  template <class PixelT,class OutT>
  inline
  void BilinearInterpolation(const ImageC<PixelT> &img,const TFVectorC<RealT,2> &ipnt,OutT &pixVal) {
    TFVectorC<RealT,2> pnt = ipnt;
#if RAVL_USE_QUICKREAL2INT && 0
    // There's some issues with QFloor, don't use it for the moment.
    IntT fx = QFloor(pnt[0]); // Row
    IntT fy = QFloor(pnt[1]); // Col
#else
    IntT fx = Floor(pnt[0]); // Row
    IntT fy = Floor(pnt[1]); // Col
#endif
    RealT u = pnt[0] - fx;
    RealT t = pnt[1] - fy;
    RealT onemu = (1.0-u);    
    RealT onemt = (1.0-t);
    const PixelT* pixel1 = &(img)[fx][fy];
    const PixelT* pixel2 = &(img)[fx+1][fy];
    pixVal = OutT((pixel1[0] * (onemt*onemu)) + 
		  (pixel1[1] * (t*onemu)) + 
		  (pixel2[0] * (onemt*u)) +
		  (pixel2[1] * (t*u)));
  }
  //: Do bilinear interpolation with different output pixel type.
  // Note: For efficency reasons this method assumes the CENTRE
  // of the pixel is at 0,0.  This means that a 0.5 offset may
  // be needed if your co-ordinate system is at the top left
  // of the pixel.
  //!param: img - The i/p image 
  //!param: ipnt - The point in the i/p image coord system for which the interpolated value is required (can be a Point2dC). 
  //!param: pixVal - The result of the interpolation (pass by reference) 

  inline
  void BilinearInterpolation(const ImageC<ByteT> &img,const TFVectorC<RealT,2> &ipnt,ByteT &pixVal) {
    TFVectorC<RealT,2> pnt = ipnt;
#if RAVL_USE_QUICKREAL2INT && 0
    // There's some issues with QFloor, don't use it for the moment.
    IntT fx = QFloor(pnt[0]); // Row
    IntT fy = QFloor(pnt[1]); // Col
#else
    IntT fx = Floor(pnt[0]); // Row
    IntT fy = Floor(pnt[1]); // Col    
#endif
    RealT u = pnt[0] - fx;
    RealT t = pnt[1] - fy;
    const ByteT* pixel1 = &(img)[fx][fy];
    const ByteT* pixel2 = &(img)[fx+1][fy];
    const RealT onemt = (1.0-t);
    const RealT onemu = (1.0-u);
    pixVal = (ByteT)(((RealT)pixel1[0] * (onemt*onemu)) + 
		    ((RealT)pixel1[1] * (t*onemu)) + 
		    ((RealT)pixel2[0] * (onemt*u)) +
		    ((RealT)pixel2[1] * (t*u)));
  }
  //: Do bilinear interpolation with different output pixel type.
  // Note: For efficency reasons this method assumes the CENTRE
  // of the pixel is at 0,0.  This means that a 0.5 offset may
  // be needed if your co-ordinate system is at the top left
  // of the pixel.
  //!param: img - The i/p image 
  //!param: ipnt - The point in the i/p image coord system for which the interpolated value is required (can be a Point2dC). 
  //!param: pixVal - The result of the interpolation (pass by reference) 

  //:-
  
  //! userlevel=Normal
  //: Bilinear sampler
  
  template<class InT,class OutT>
  class SampleBilinearC
  {
  public:
    SampleBilinearC()
    {}
    
    void operator()(const ImageC<InT> &img,const TFVectorC<RealT,2> &ipnt,OutT &pixVal)
    { BilinearInterpolation(img,ipnt,pixVal); }
    //: Do bilinear interpolation
     //!param: img - The image 
    //!param: ipnt - The point for wish the interpolated value is required (can be a Point2dC). 
    //!param: pixVal - The result of the interpolation (pass by reference) 
  };
  
  //! userlevel=Normal
  //: Nearest pixel sampler.
  
  template<class InT,class OutT>
  class SampleNearestC
  {
  public:    
    void operator()(const ImageC<InT> &img,const TFVectorC<RealT,2> &ipnt,OutT &pixVal)
    { 
#if RAVL_USE_QUICKREAL2INT
      pixVal = static_cast<OutT>(img[QRound(ipnt[0])][QRound(ipnt[1])]); 
#else
      pixVal = static_cast<OutT>(img[Round(ipnt[0])][Round(ipnt[1])]);
#endif
    }
    //: Do nearest pixel interpolation
    //!param: img - The image 
    //!param: ipnt - The point for wish the interpolated value is required (can be a Point2dC). 
    //!param: pixVal - The result of the interpolation (pass by reference) 
  };

}

#endif
