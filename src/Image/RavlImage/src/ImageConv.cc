// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/ImageConv.cc"

#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/RealYUVValue.hh"
#include "Ravl/Image/RGBcYUV.hh"
#include "Ravl/VectorUtils.hh"

// Some type conversions for standard images.

namespace RavlImageN {

  // Byte to double image.

  ImageC<RealT> ByteImageCT2DoubleImageCT(const ImageC<ByteT> &dat) {
    ImageC<RealT> ret(dat.Rectangle());
    for(Array2dIter2C<RealT,ByteT> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = (RealT) it.Data2();
    return ret;
  }

  // Byte to int image.

  ImageC<IntT> ByteImageCT2IntImageCT(const ImageC<ByteT> &dat) {
    ImageC<IntT> ret(dat.Rectangle());
    for(Array2dIter2C<IntT,ByteT> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = (IntT) it.Data2();
    return ret;
  }

  // Int to double image.

  ImageC<RealT> IntImageCT2DoubleImageCT(const ImageC<IntT> &dat)   {
    ImageC<RealT> ret(dat.Rectangle());
    for(Array2dIter2C<RealT,IntT> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = (RealT) it.Data2();
    return ret;
  }


  // Byte grey level to colour image.

  ImageC<ByteRGBValueC> ByteImageCT2ByteRGBImageCT(const ImageC<ByteT> &dat) {
    ImageC<ByteRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteRGBValueC,ByteT> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = ByteRGBValueC(it.Data2(),it.Data2(),it.Data2());
    return ret;
  }

  // Real grey level to colour image.

  ImageC<RealRGBValueC> RealImageCT2RealRGBImageCT(const ImageC<RealT> &dat) {
    ImageC<RealRGBValueC> ret(dat.Rectangle());
    for(Array2dIter2C<RealRGBValueC,RealT> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = RealRGBValueC(it.Data2(),it.Data2(),it.Data2());
    return ret;
  }


  // Byte grey level to byte YUV colour image.

  ImageC<ByteYUVValueC> ByteImageCT2ByteYUVImageCT(const ImageC<ByteT> &dat) {
    ImageC<ByteYUVValueC> ret(dat.Rectangle());
    for(Array2dIter2C<ByteYUVValueC,ByteT> it(ret,dat);it.IsElm();it.Next())
      it.Data1() = ByteYUVValueC(it.Data2(),0,0);
    return ret;
  }

  // Double -> Byte (clipped to fit)
  // This will clip, then round the double value (NOT floor!) to fit in a byte value 0 to 255.
  ImageC<ByteT> DoubleImageCT2ByteImageCT(const ImageC< RealT> &dat)
  {
    ImageC< ByteT> ret(dat.Rectangle());
    if (dat.IsContinuous()) RavlBaseVectorN::Real2ByteD(ret.Row(ret.TRow()), dat.Row(dat.TRow()), dat.Size());
    else {
      for (IndexC r=dat.TRow(); r<=dat.BRow(); ++r)
        RavlBaseVectorN::Real2ByteD(ret.Row(r), dat.Row(r), dat.Cols());
    }
    return ret;
  }


}
