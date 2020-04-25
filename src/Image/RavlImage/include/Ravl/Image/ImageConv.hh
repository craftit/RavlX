// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMAGECONV_HEADER
#define RAVLIMAGE_IMAGECONV_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/ImageConv.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="16/03/1999"
//! docentry="Ravl.API.Images.Converters"

#include "Ravl/Image/Image.hh"

namespace RavlImageN {

  class ByteYUVValueC;
  class ByteYUVAValueC;
  class ByteYUV422ValueC;
  class ByteVYUValueC;
  class RealYUVValueC;
  class ByteRGBValueC;
  class RealRGBValueC;
  class ByteRGBAValueC;
  class RealRGBAValueC;
  class ByteRGBMedianC;
  class RealRGBAverageC;
  class UInt16RGBValueC;
  class RealHSVValueC ; 
  template<typename CompT> class RGBValueC; 
  template<typename CompT> class YCbCrBT601ValueC; 
  template<typename CompT> class YCbCrBT709ValueC;
  template<typename CompT> class YCbCr422BT601ValueC; 
  template<typename CompT> class YCbCr422BT709ValueC;
  class ByteYCbCrBT601ValueC ; 
  class UInt16YCbCrBT601ValueC ;
  class UInt16YCbCr422BT601ValueC ; 
  class ByteYCbCrBT709ValueC ; 
  class UInt16YCbCrBT709ValueC ;
  class UInt16YCbCr422BT709ValueC ; 

  //:-
  //: This is the complete list of image converters
  // For more fine-grained lists see the <a href="../Tree/Ravl.API.Images.Pixel_Types.html">individual pixel categories</a>
  
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.Grey Level"

  ImageC<RealT> ByteImageCT2DoubleImageCT(const ImageC<ByteT> &dat);
  //: Image conversion: byte grey-level &rarr; double grey-level
    
  ImageC<ByteT> DoubleImageCT2ByteImageCT(const ImageC<RealT> &dat) ; 
  //: Image conversion: double grey-level &rarr; byte grey-level (clipped to fit)
  // This will clip, then round the double value (NOT floor!) to fit into the range 0 to 255.
  
  ImageC<IntT> ByteImageCT2IntImageCT(const ImageC<ByteT> &dat);
  //: Image conversion: byte grey-level &rarr; IntT grey-level
  
  ImageC<RealT> IntImageCT2DoubleImageCT(const ImageC<IntT> &dat);
  //: Image conversion: IntT grey-level &rarr; double grey-level
  
  ImageC<RealT> UIntTImageCT2DoubleImageCT(const ImageC<UIntT> &dat);
  //: Image conversion: UIntT grey-level &rarr; double grey-level

  ImageC<UIntT> UInt16TImageCT2UIntTImageCT(const ImageC<UInt16T> &dat);
  //: Image conversion: UInt16T grey-level &rarr; UIntT grey-level

  ImageC<UInt16T> UIntTImageCT2UInt16TImageCT(const ImageC<UIntT> &dat);
  //: Image conversion: UIntT grey-level &rarr; UInt16T grey-level

  ImageC<RealT> FloatTImageCT2RealImageCT(const ImageC<FloatT> &dat);
  //: Image conversion: float grey-level &rarr; double grey-level
  
  ImageC<FloatT> RealTImageCT2FloatImageCT(const ImageC<RealT> &dat);
  //: Image conversion: double grey-level &rarr; float grey-level
  
  ImageC<ByteT> BoolImageC2ByteImageCT(const ImageC<bool> &dat);
  //: Image conversion: boolean grey-level &rarr; byte grey-level
  
  ImageC<bool> ByteImage2BoolImage(const ImageC<ByteT> &dat);
  //: Image conversion: byte grey-level &rarr; boolean grey-level
  
  ImageC<ByteT> FloatImageCT2ByteImageCT(const ImageC<FloatT> &dat);
  //: Image conversion: float grey-level &rarr; byte grey-level

  ImageC<FloatT> ByteImageCT2FloatImageCT(const ImageC<ByteT> &dat);
  //: Image conversion: float grey-level &rarr; byte grey-level

  //:-
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.RGB"

  ImageC<ByteRGBValueC> ByteImageCT2ByteRGBImageCT(const ImageC<ByteT> &dat);
  //: Image conversion: byte grey-level &rarr; byte RGB
    
  ImageC<RealRGBValueC> RealImageCT2RealRGBImageCT(const ImageC<RealT> &dat);
  //: Image conversion: double grey-level &rarr; double RGB
    
  ImageC<ByteT> RGBImageCT2ByteImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; byte grey-level
  
  ImageC<RealT> RGBImageCT2DoubleImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; double grey-level
  
  ImageC<FloatT> ByteRGBImageCT2FloatImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; float grey-level
  
  ImageC<RealRGBValueC>  ByteRGBImageCT2RealRGBImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; double RGB

  ImageC<ByteRGBValueC>  RealRGBImageCT2ByteRGBImageCT(const ImageC<RealRGBValueC> &dat);
  //: Image conversion: double RGB &rarr; byte RGB (clipped to fit)

  ImageC<UInt16RGBValueC> RealRGBImageCT2UInt16RGBImageCT(const ImageC<RealRGBValueC> &dat);
  //: Image conversion: double RGB &rarr; UInt16 RGB

  ImageC<RealT>  RealRGBImageCT2DoubleImageCT(const ImageC<RealRGBValueC> &dat);
  //: Image conversion: double RGB &rarr; double
  // Mean of R,G,B, returned.

  ImageC<UInt16RGBValueC> ByteRGBImageCT2UInt16RGBImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; UInt16 RGB

  ImageC<ByteRGBValueC>  UInt16RGBImageCT2ByteRGBImageCT(const ImageC<UInt16RGBValueC> &dat);
  //: Image conversion: UInt16 RGB &rarr; byte RGB

  ImageC<RealRGBValueC>  UInt16RGBImageCT2RealRGBImageCT(const ImageC<UInt16RGBValueC> &dat);
  //: Image conversion: UInt16 RGB &rarr; double RGB

  ImageC<RGBValueC<float> > RealRGBImageCT2RGBImageCFloat(const ImageC<RealRGBValueC> & dat );
  //: Image conversion: double RGB &rarr; float RGB
  
  ImageC<RealRGBValueC> RGBImageCFloat2RealRGBImageCT(const ImageC<RGBValueC<float> > & dat );
  //: Image conversion: float RGB &rarr; double RGB
  
  ImageC<ByteRGBValueC> ByteRGBAImageCT2ByteRGBImageCT(const ImageC<ByteRGBAValueC> &dat);
  //: Image conversion: byte RGBA &rarr; byte RGB
  
  ImageC<ByteRGBAValueC> ByteRGBImageCT2ByteRGBAImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; byte RGBA
  
  ImageC<RealRGBValueC> RealRGBAImageCT2RealRGBImageCT(const ImageC<RealRGBAValueC> &dat);
  //: Image conversion: double RGBA &rarr; double RGB
  
  ImageC<RealRGBAValueC> RealRGBImageCT2RealRGBAImageCT(const ImageC<RealRGBValueC> &dat);
  //: Image conversion: double RGB &rarr; double RGBA
  
  ImageC<ByteRGBValueC>  RealRGBAverageImageC2ByteRGBImageCT(const ImageC<RealRGBAverageC> &dat);
  //: Image conversion: <a href="../Class/RavlImageN.RealRGBAverageC.html">Real RGB rolling average</a> &rarr; RGB

  ImageC<ByteRGBValueC>  ByteRGBMedianImageC2ByteRGBImageCT(const ImageC<ByteRGBMedianC> &dat);
  //: Image conversion:  <a href="../Class/RavlImageN.ByteRGBMedianC.html">Byte RGB median</a> &rarr; RGB

  //:-
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.YUV"

  ImageC<RealYUVValueC> ByteYUVImageCT2RealYUVImageCT(const ImageC<ByteYUVValueC> &dat);
  //: Image conversion: byte YUV &rarr; double YUV
  
  ImageC<ByteYUVValueC> RealYUVImageCT2ByteYUVImageCT(const ImageC<RealYUVValueC> &dat);
  //: Image conversion: double YUV &rarr; byte YUV
  
  ImageC<ByteT> ByteYUVImageCT2ByteImageCT(const ImageC<ByteYUVValueC> &dat);
  //: Image conversion: byte YUV &rarr; byte grey-level
  
  ImageC<ByteYUVValueC> ByteImageCT2ByteYUVImageCT(const ImageC<ByteT> &dat);
  //: Image conversion: byte grey-level &rarr; byte YUV
  
  ImageC<ByteRGBValueC> YUVImageCT2RGBImageCT(const ImageC<ByteYUVValueC> &dat);
  //: Image conversion: byte YUV &rarr; byte RGB
  
  ImageC<ByteYUVValueC> RGBImageCT2YUVImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; byte YUV
  
  ImageC<RealRGBValueC> RealYUVImageCT2RealRGBImageCT(const ImageC<RealYUVValueC> &dat);
  //: Image conversion: double YUV &rarr; double RGB
  
  ImageC<RealYUVValueC> RealRGBImageCT2RealYUVImageCT(const ImageC<RealRGBValueC> &dat);
  //: Image conversion: double RGB &rarr; double YUV

  ImageC<ByteYUVValueC> ByteYUVAImageCT2ByteYUVImageCT(const ImageC<ByteYUVAValueC> &dat);
  //: Image conversion: byte YUVA &rarr; byte YUV
  
  ImageC<ByteYUVAValueC> ByteYUVImageCT2ByteYUVAImageCT(const ImageC<ByteYUVValueC> &dat);
  //: Image conversion: byte YUV &rarr; byte YUVA

  ImageC<ByteVYUValueC> ByteYUVImageCT2ByteVYUImageCT(const ImageC<ByteYUVValueC> &dat);
  //: Image conversion: byte YUV &rarr; byte VYU
  
  ImageC<ByteYUVValueC> ByteVYUImageCT2ByteYUVImageCT(const ImageC<ByteVYUValueC> &dat);
  //: Image conversion: byte VYU &rarr; byte YUV
  
  //:-
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.YUV.YUV422"

  ImageC<ByteYUVValueC> ByteYUV422ImageCT2ByteYUVImageCT(const ImageC<ByteYUV422ValueC> &dat);
  //: Image conversion: byte YUV422 &rarr; byte YUV
  
  ImageC<ByteRGBValueC> ByteYUV422ImageCT2ByteRGBImageCT(const ImageC<ByteYUV422ValueC> &dat);
  //: Image conversion: byte YUV &rarr; byte YUV422

  ImageC<ByteT> ByteYUV422ImageCT2ByteImageCT(const ImageC<ByteYUV422ValueC> &dat);
  //: Image conversion: byte YUV422 &rarr; byte grey-level
  
  ImageC<ByteYUV422ValueC> ByteImageCT2ByteYUV422ImageCT(const ImageC<ByteT> &dat);
  //: Image conversion: byte grey-level &rarr; byte YUV422

  ImageC<ByteYUV422ValueC> ByteRGBImageCT2ByteYUV422ImageCT(const ImageC<ByteRGBValueC> &dat);
  //: Image conversion: byte RGB &rarr; byte YUV422

  //:-
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.HSV"

  ImageC<RealHSVValueC> RealRGBImageCT2RealHSVImageCT(const ImageC<RealRGBValueC> & dat) ;
  //: Image conversion: double RGB &rarr; double HSV
  
  ImageC<RealRGBValueC> RealHSVImageCT2RealRGBImageCT(const ImageC<RealHSVValueC> & dat) ;
  //: Image conversion: double HSV &rarr; double RGB
  
  //:-
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.YCbCr"

  ImageC<ByteT> ByteYCbCrBT601ValueImage2ByteImage(const ImageC<ByteYCbCrBT601ValueC> &dat);
  //: Image conversion: byte YCbCr  &rarr; byte grey-level
  
  ImageC<UInt16T> UInt16YCbCrBT601ValueImage2UInt16Image(const ImageC<UInt16YCbCrBT601ValueC> &dat);
  //: Image conversion: UInt16 YCbCr &rarr; UInt16T grey-level
    
  ImageC<UInt16YCbCrBT601ValueC> ByteYCbCrBT601ValueImage2UInt16YCbCrBT601ValueImage(const ImageC<ByteYCbCrBT601ValueC> &dat);
  //: Image conversion: byte YCbCrB &rarr;  UInt16 YCbCr
  
  ImageC<ByteYCbCrBT601ValueC> UInt16YCbCrBT601ValueImage2ByteYCbCrBT601ValueImage(const ImageC<UInt16YCbCrBT601ValueC> &dat);
  //: Image conversion: UInt16 YCbCr &rarr; byte YCbCr
////////////////////////////////////////////////////////////////
  ImageC<UInt16T> UInt16YCbCrBT709ValueImage2UInt16Image(const ImageC<UInt16YCbCrBT709ValueC> &dat);
  //: Image conversion: UInt16 YCbCr &rarr; UInt16T grey-level
    
  ImageC<UInt16YCbCrBT709ValueC> ByteYCbCrBT709ValueImage2UInt16YCbCrBT709ValueImage(const ImageC<ByteYCbCrBT709ValueC> &dat);
  //: Image conversion: byte YCbCrB &rarr;  UInt16 YCbCr
  
  ImageC<ByteYCbCrBT709ValueC> UInt16YCbCrBT709ValueImage2ByteYCbCrBT709ValueImage(const ImageC<UInt16YCbCrBT709ValueC> &dat);
  //: Image conversion: UInt16 YCbCr &rarr; byte YCbCr
  //////////////////////////////////////////////////////////////
  ImageC<ByteRGBValueC> ByteYCbCrBT601ValueImage2ByteRGBImage(const ImageC<ByteYCbCrBT601ValueC > &dat);
  //: Image conversion: byte YCbCr &rarr;  byte RGB

  ImageC<ByteRGBValueC> ByteYCbCrBT709ValueImage2ByteRGBImage(const ImageC<ByteYCbCrBT709ValueC > &dat);
  //: Image conversion: byte YCbCr &rarr;  byte RGB
  
  ImageC<RGBValueC<float> > YCbCrBT601ValueFloatImage2ImageRGBValueFloat(const ImageC<YCbCrBT601ValueC<float> > &dat);
  //: Image conversion: float YCbCr &rarr;  float RGB

  ImageC<RGBValueC<float> > YCbCrBT709ValueFloatImage2ImageRGBValueFloat(const ImageC<YCbCrBT709ValueC<float> > &dat);
  //: Image conversion: float YCbCr &rarr;  float RGB
  
  ImageC<ByteYCbCrBT601ValueC > ByteRGBImage2ByteYCbCrBT601ValueImage(const ImageC<ByteRGBValueC>  &dat);
  //: Image conversion: byte RGB &rarr; byte YCbCr 

  ImageC<ByteYCbCrBT709ValueC > ByteRGBImage2ByteYCbCrBT709ValueImage(const ImageC<ByteRGBValueC>  &dat);
  //: Image conversion: byte RGB &rarr; byte YCbCr 

  ImageC<ByteRGBValueC> UInt16YCbCrBT601ValueImage2ByteRGBImage(const ImageC<UInt16YCbCrBT601ValueC > &dat);
  //: Image conversion: UInt 16YCbCr &rarr;  byte RGB

  ImageC<ByteRGBValueC> UInt16YCbCrBT709ValueImage2ByteRGBImage(const ImageC<UInt16YCbCrBT709ValueC > &dat);
  //: Image conversion: UInt 16YCbCr &rarr;  byte RGB

  ImageC<RGBValueC<float> > UInt16YCbCrBT601ValueImage2ImageRGBValueFloat(const ImageC<UInt16YCbCrBT601ValueC > &dat);
  //: Image conversion: UInt 16YCbCr &rarr;  float RGB

  ImageC<RGBValueC<float> > UInt16YCbCrBT709ValueImage2ImageRGBValueFloat(const ImageC<UInt16YCbCrBT709ValueC > &dat);
  //: Image conversion: UInt 16YCbCr &rarr;  float RGB

  ImageC<UInt16YCbCrBT601ValueC> UInt16YCbCr422BT601ImageCT2UInt16YCbCrBT601ImageCT(const ImageC<UInt16YCbCr422BT601ValueC> &dat);
  //: Image conversion: UInt 16YCbCr422 &rarr; 16YCbCr

  ImageC<UInt16YCbCrBT709ValueC> UInt16YCbCr422BT709ImageCT2UInt16YCbCrBT709ImageCT(const ImageC<UInt16YCbCr422BT709ValueC> &dat);
  //: Image conversion: UInt 16YCbCr422 &rarr; 16YCbCr
  
  ImageC<YCbCrBT601ValueC<float> > ImageRGBValueFloat2YCbCrBT601ValueFloatImage(const ImageC<RGBValueC<float> > &dat);
  //: Image conversion: float RGB &rarr; float YCbCr

  ImageC<YCbCrBT709ValueC<float> > ImageRGBValueFloat2YCbCrBT709ValueFloatImage(const ImageC<RGBValueC<float> > &dat);
  //: Image conversion: float RGB &rarr; float YCbCr

  //:-
  //! docentry="Ravl.API.Images.Converters;Ravl.API.Images.Pixel Types.Grey Level"
  //! userlevel=Obsolete

  ImageC<ByteT> BoolImage2ByteImage(const ImageC<bool> &dat);
  //: Image conversion: boolean grey-level &rarr; byte grey-level
  
  
};

#endif
