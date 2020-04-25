// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/ImageConv2.cc"

#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/Image/ByteYUVAValue.hh"
#include "Ravl/Image/RGBcYUV.hh"

namespace RavlImageN {  
  
  // Byte YUVA to Byte YUV 
  
  ImageC<ByteYUVValueC> ByteYUVAImageCT2ByteYUVImageCT(const ImageC<ByteYUVAValueC> &dat) {
    ImageC<ByteYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUVValueC,ByteYUVAValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = ByteYUVValueC(it.Data2().Y(),it.Data2().U(),it.Data2().V());
    return ret;
  }
  
  // Byte YUV to Byte YUVA 
  
  ImageC<ByteYUVAValueC> ByteYUVImageCT2ByteYUVAImageCT(const ImageC<ByteYUVValueC> &dat) {
    ImageC<ByteYUVAValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUVAValueC,ByteYUVValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = ByteYUVAValueC(it.Data2().Y(),it.Data2().U(),it.Data2().V(),(char) 0xff);
    }
    return ret;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  
  // Byte colour to real colour image.
  
  ImageC<RealYUVValueC> ByteYUVImageCT2RealYUVImageCT(const ImageC<ByteYUVValueC> &dat) {
    ImageC<RealYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealYUVValueC,ByteYUVValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = RealYUVValueC(it.Data2().Y(),it.Data2().U(),it.Data2().V());
    }
    return ret;
  }
  
  // Real colour to byte colour image.

  ImageC<ByteYUVValueC> RealYUVImageCT2ByteYUVImageCT(const ImageC<RealYUVValueC> &dat) {
    ImageC<ByteYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUVValueC,RealYUVValueC> it(ret,dat);it.IsElm();it.Next()) {
      RealYUVValueC v(it.Data2());
      v.LimitYUV(0,255,-128,127);
      it.Data1() = ByteYUVValueC((ByteT)v.Y(),(ByteT)v.U(),(ByteT)v.V());
    }
    return ret;
  }
  
  // Real YUV -> RGB
  
  ImageC<RealRGBValueC> RealYUVImageCT2RealRGBImageCT(const ImageC<RealYUVValueC> &dat) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,RealYUVValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = RealRGBValueC(it.Data2());
    return ret;
  }


  // Real RGB -> YUV

  ImageC<RealYUVValueC> RealRGBImageCT2RealYUVImageCT(const ImageC<RealRGBValueC> &dat) {
    ImageC<RealYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealYUVValueC,RealRGBValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = RealRGBValueC(it.Data2());
    return ret;
  }


  // Byte grey level to byte YUV colour image.
  ImageC<ByteT> ByteYUVImageCT2ByteImageCT(const ImageC<ByteYUVValueC> &dat) { 
    ImageC<ByteT> ret(dat.Rectangle());
    for(Array2dIter2C<ByteT,ByteYUVValueC> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = it.Data2().Y();
    return ret;  
  }

  // Byte colour to byte grey image.
  
  ImageC<ByteT> RGBImageCT2ByteImageCT(const ImageC<ByteRGBValueC> &dat) {
    ImageC<ByteT> ret(dat.Rectangle());
    for(Array2dIter2C<ByteT,ByteRGBValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = (ByteT) ((IntT) ((IntT) it.Data2().Red() + it.Data2().Green() + it.Data2().Blue()) / 3);
    return ret;
  }
  // Byte Colour to double image.
  
  ImageC<RealT> RGBImageCT2DoubleImageCT(const ImageC<ByteRGBValueC> &dat)  { 
    ImageC<RealT> ret(dat.Rectangle());
    for(Array2dIter2C<RealT,ByteRGBValueC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = (RealT) ((IntT) it.Data2().Red() + it.Data2().Green() + it.Data2().Blue()) / 3.0;
    return ret;
  }
  
  // Byte YUV to byte RGB image.

  ImageC<ByteRGBValueC> YUVImageCT2RGBImageCT(const ImageC<ByteYUVValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,ByteYUVValueC> it(ret,dat);it.IsElm();it.Next()) {
#if 1
      ByteYUV2RGB(it.Data2().Y(),it.Data2().U(),it.Data2().V(),it.Data1());
#else
      // VIA reals for testing.
      RealRGBValueC v(RealYUVValueC(it.Data2()));
      v.Limit(0,255);
      it.Data1().Set((ByteT) v.Red(),(ByteT) v.Green(),(ByteT) v.Blue());
#endif
      
    }
    return ret;
  }

  // Byte RGB to byte YUV image.
  
  ImageC<ByteYUVValueC> RGBImageCT2YUVImageCT(const ImageC<ByteRGBValueC> &dat) {
    ImageC<ByteYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUVValueC,ByteRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      RealYUVValueC v(RealRGBValueC(it.Data2()));
      v.LimitYUV(0,255,-128,127);
      it.Data1().Set((ByteT) v.Y(),(ByteT) v.U(),(ByteT) v.V());
    }
    return ret;
  }

}
