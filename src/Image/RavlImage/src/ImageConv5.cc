// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImage
//! file="Ravl/Image/Base/ImageConv5.cc"

#include "Ravl/Image/ByteYCbCrBT601Value.hh"
#include "Ravl/Image/UInt16YCbCrBT601Value.hh"
#include "Ravl/Image/ByteYCbCr422BT601Value.hh"
#include "Ravl/Image/UInt16YCbCr422BT601Value.hh"
#include "Ravl/Image/ByteYCbCrBT709Value.hh"
#include "Ravl/Image/UInt16YCbCrBT709Value.hh"
#include "Ravl/Image/ByteYCbCr422BT709Value.hh"
#include "Ravl/Image/UInt16YCbCr422BT709Value.hh"
#include "Ravl/Image/RGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/Image.hh"

namespace RavlImageN {
  
  //: Convert an ByteYCbCrBT601ValueC to a byte value.
  
  ImageC<ByteT> ByteYCbCrBT601ValueImage2ByteImage(const ImageC<ByteYCbCrBT601ValueC> &dat) {
    ImageC<ByteT> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteT,ByteYCbCrBT601ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2().Y();
    return ret;
  }
  
  //: Convert an UInt16YCbCrBT601ValueC to a UInt16T value.
  
  ImageC<UInt16T> UInt16YCbCrBT601ValueImage2UInt16Image(const ImageC<UInt16YCbCrBT601ValueC> &dat) {
    ImageC<UInt16T> ret(dat.Rectangle()); 
    for(Array2dIter2C<UInt16T,UInt16YCbCrBT601ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2().Y();
    return ret;
  }
  
  //: Convert an ByteYCbCrBT601ValueC to a UInt16YCbCrBT601ValueC value.
  
  ImageC<UInt16YCbCrBT601ValueC> ByteYCbCrBT601ValueImage2UInt16YCbCrBT601ValueImage(const ImageC<ByteYCbCrBT601ValueC> &dat) {
    ImageC<UInt16YCbCrBT601ValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<UInt16YCbCrBT601ValueC,ByteYCbCrBT601ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret;
  }

  ImageC<UInt16YCbCrBT709ValueC> ByteYCbCrBT709ValueImage2UInt16YCbCrBT709ValueImage(const ImageC<ByteYCbCrBT709ValueC> &dat) {
    ImageC<UInt16YCbCrBT709ValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<UInt16YCbCrBT709ValueC,ByteYCbCrBT709ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret;
  }

  ImageC<ByteYCbCrBT601ValueC> UInt16YCbCrBT601ValueImage2ByteYCbCrBT601ValueImage(const ImageC<UInt16YCbCrBT601ValueC> &dat) {
    ImageC<ByteYCbCrBT601ValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteYCbCrBT601ValueC,UInt16YCbCrBT601ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret;    
  }

  ImageC<ByteYCbCrBT709ValueC> UInt16YCbCrBT709ValueImage2ByteYCbCrBT709ValueImage(const ImageC<UInt16YCbCrBT709ValueC> &dat) {
    ImageC<ByteYCbCrBT709ValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteYCbCrBT709ValueC,UInt16YCbCrBT709ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret;    
  }

  ImageC<ByteYCbCrBT709ValueC> UInt16YCbCrBT601ValueImage2ByteYCbCrBT709ValueImage(const ImageC<UInt16YCbCrBT709ValueC> &dat) {
    ImageC<ByteYCbCrBT709ValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteYCbCrBT709ValueC,UInt16YCbCrBT709ValueC> it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret;    
  }
  //: Convert an UInt16YCbCrBT601Value to a ByteYCbCrBT601ValueC value.
  
  
  //: Convert YCbCrBT601 to RGB in floating point format.
  
  ImageC<RGBValueC<float> > YCbCrBT601ValueFloatImage2ImageRGBValueFloat(const ImageC<YCbCrBT601ValueC<float> > &dat) {
    ImageC<RGBValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<RGBValueC<float>,YCbCrBT601ValueC<float> > it(ret,dat);it;it++) 
      YCbCrBT601Float2RGBFloat(it.Data2(),it.Data1());
    return ret;
  }

  //: Convert YCbCrBT709 to RGB in floating point format.
  
  ImageC<RGBValueC<float> > YCbCrBT709ValueFloatImage2ImageRGBValueFloat(const ImageC<YCbCrBT709ValueC<float> > &dat) {
    ImageC<RGBValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<RGBValueC<float>,YCbCrBT709ValueC<float> > it(ret,dat);it;it++) 
      YCbCrBT709Float2RGBFloat(it.Data2(),it.Data1());
    return ret;
  }
  
  //: Convert RGB to YCbCrBT601 in floating point format.
  

  ImageC<YCbCrBT601ValueC<float> > ImageRGBValueFloat2YCbCrBT601ValueFloatImage(const ImageC<RGBValueC<float> > &dat) {
    ImageC<YCbCrBT601ValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<YCbCrBT601ValueC<float>,RGBValueC<float> > it(ret,dat);it;it++)
      RGBFloat2YCbCrBT601Float(it.Data2(),it.Data1());
    return ret;
  }

  ImageC<YCbCrBT709ValueC<float> > ImageRGBValueFloat2YCbCrBT709ValueFloatImage(const ImageC<RGBValueC<float> > &dat) {
    ImageC<YCbCrBT709ValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<YCbCrBT709ValueC<float>,RGBValueC<float> > it(ret,dat);it;it++)
      RGBFloat2YCbCrBT709Float(it.Data2(),it.Data1());
    return ret;
  }
  
  ImageC<RGBValueC<float> > RealRGBImageCT2RGBImageCFloat(const ImageC<RealRGBValueC> & dat ) {
    ImageC<RGBValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<RGBValueC<float>,RealRGBValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret ;
  }
  
  ImageC<RealRGBValueC> RGBImageCFloat2RealRGBImageCT(const ImageC<RGBValueC<float> > & dat ) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,RGBValueC<float> > it(ret,dat);it;it++) 
      it.Data1() = it.Data2();
    return ret ;
  }

  //: Convert YCbCrBT601 to RGB in byte point format.
  
  ImageC<ByteRGBValueC> ByteYCbCrBT601ValueImage2ByteRGBImage(const ImageC<ByteYCbCrBT601ValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,ByteYCbCrBT601ValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2().ByteRGB();
    return ret ;
  }

  //: Convert YCbCrBT709 to RGB in byte point format.
  
  ImageC<ByteRGBValueC> ByteYCbCrBT709ValueImage2ByteRGBImage(const ImageC<ByteYCbCrBT709ValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,ByteYCbCrBT709ValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2().ByteRGB();
    return ret ;
  }
  
  //: Convert UInt16YCbCrBT601 to RGB in byte point format.
  
  ImageC<ByteRGBValueC> UInt16YCbCrBT601ValueImage2ByteRGBImage(const ImageC<UInt16YCbCrBT601ValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,UInt16YCbCrBT601ValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2().ByteRGB();
    return ret ;
  }

  //: Convert UInt16YCbCrBT709 to RGB in byte point format.
  
  ImageC<ByteRGBValueC> UInt16YCbCrBT709ValueImage2ByteRGBImage(const ImageC<UInt16YCbCrBT709ValueC> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,UInt16YCbCrBT709ValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2().ByteRGB();
    return ret ;
  }

  //: Convert UInt16YCbCrBT601 to RGB in float point format.
  
  ImageC<RGBValueC<float> > UInt16YCbCrBT601ValueImage2ImageRGBValueFloat(const ImageC<UInt16YCbCrBT601ValueC> &dat) {
    ImageC<RGBValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<RGBValueC<float>,UInt16YCbCrBT601ValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2().FloatRGB();
    return ret ;
  }

  //: Convert UInt16YCbCrBT709 to RGB in float point format.
  
  ImageC<RGBValueC<float> > UInt16YCbCrBT709ValueImage2ImageRGBValueFloat(const ImageC<UInt16YCbCrBT709ValueC> &dat) {
    ImageC<RGBValueC<float> > ret(dat.Rectangle());
    for(Array2dIter2C<RGBValueC<float>,UInt16YCbCrBT709ValueC > it(ret,dat);it;it++) 
      it.Data1() = it.Data2().FloatRGB();
    return ret ;
  }

  //: Convert byte RGB to YCbCrBT601.

  ImageC<ByteYCbCrBT601ValueC > ByteRGBImage2ByteYCbCrBT601ValueImage(const ImageC<ByteRGBValueC> &dat) {
    ImageC<ByteYCbCrBT601ValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYCbCrBT601ValueC,ByteRGBValueC> it(ret,dat);it;it++) 
      it.Data1() = ByteYCbCrBT601ValueC(it.Data2());
    return ret ;
  }

  //: Convert byte RGB to YCbCrBT601.

  ImageC<ByteYCbCrBT709ValueC > ByteRGBImage2ByteYCbCrBT709ValueImage(const ImageC<ByteRGBValueC> &dat) {
    ImageC<ByteYCbCrBT709ValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYCbCrBT709ValueC,ByteRGBValueC> it(ret,dat);it;it++) 
      it.Data1() = ByteYCbCrBT709ValueC(it.Data2());
    return ret ;
  }

  
}
