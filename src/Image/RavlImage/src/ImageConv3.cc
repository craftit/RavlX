// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/ImageConv3.cc"

#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/ByteRGBMedian.hh"
#include "Ravl/Image/RealRGBAverage.hh"
#include "Ravl/Image/RealHSVValue.hh"

namespace RavlImageN {

  
  // Real RGB rolling average -> RGB

  ImageC<ByteRGBValueC>  RealRGBAverageImageC2ByteRGBImageCT(const ImageC<RealRGBAverageC> &dat)   { 
    ImageC<ByteRGBValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteRGBValueC,RealRGBAverageC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = it.Data2();
    return ret;
  }

  // Byte RGB median -> RGB

  ImageC<ByteRGBValueC>  ByteRGBMedianImageC2ByteRGBImageCT(const ImageC<ByteRGBMedianC> &dat)   { 
    ImageC<ByteRGBValueC> ret(dat.Rectangle()); 
    for(Array2dIter2C<ByteRGBValueC,ByteRGBMedianC> it(ret,dat);it.IsElm();it.Next()) 
      it.Data1() = it.Data2();
    return ret;
  }
  

 
  // Real RGB to Real HSV 
  ImageC<RealHSVValueC> RealRGBImageCT2RealHSVImageCT(const ImageC<RealRGBValueC> & dat) {
    ImageC<RealHSVValueC> ret(dat.Rectangle() ) ;
    for(Array2dIter2C<RealHSVValueC,RealRGBValueC> it(ret,dat);it.IsElm();it.Next()) {
      RealHSVValueC v( it.Data2() ) ;
      it.Data1() = v ;}
    return ret ;
  }
  
  // Real HSV to Real RGB 
  ImageC<RealRGBValueC> RealHSVImageCT2RealRGBImageCT(const ImageC<RealHSVValueC> & dat ) {
    ImageC<RealRGBValueC> ret ( dat.Rectangle() ) ;
    for(Array2dIter2C<RealRGBValueC,RealHSVValueC> it(ret,dat);it.IsElm();it.Next()) {
      it.Data1() = (it.Data2().RealRGBValue() ) ;}
      return ret ;
  }

}
