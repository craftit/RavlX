// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////
//! lib=RavlExtImgIO
//! file="Ravl/Image/ExternalImageIO/Core/exExtImgIO.cc"
//! docentry="Ravl.API.Images.IO.Formats"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/DP/MemIO.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Image/ImageConv.hh"

#define DODEBUG 0
// No need to define ONDEBUG as that is declared in Ravl/DP/MemIO.hh

using namespace RavlImageN;

template<typename PixelT>
int testImgMemIO(const StringC &formatName,bool isLossy) {
  ImageC<PixelT> img(100,100);
  PixelT val = 0;
  for(Array2dIterC<PixelT> it(img);it;it++) {
    *it = (val++)  % (int)(1 << 12);
  }

  SArray1dC<char> buffer;
  if(!MemSave(buffer,img,formatName,DODEBUG)) {
    std::cerr << "Failed to save " << formatName << " image. \n";
    return __LINE__;
  }

  ImageC<PixelT> img2;
  if(!MemLoad(buffer,img2,formatName,DODEBUG)) {
    std::cerr << "Failed to load " << formatName << " image. \n";
    return __LINE__;
  }

  for(Array2dIter2C<PixelT,PixelT> it(img,img2);it;it++) {
    if(isLossy) {
      if(Abs((int) it.Data1() - (int) it.Data2()) > 8) {
        if ( DODEBUG )
        {
          Save("@X:in",img);
          Save("@X:out",img2);
        }
        std::cerr << "Images differ for format " << formatName << " v1:" << (int) it.Data1() << " v2:" << (int) it.Data2() << " @ " << it.Index() << " \n";
        return __LINE__;
      }
    } else {
      if(it.Data1() != it.Data2()) {
        if ( DODEBUG )
        {
          Save("@X:in",img);
          Save("@X:out",img2);
        }
        std::cerr << "Images differ for format " << formatName << " v1:" << (int) it.Data1() << " v2:" << (int) it.Data2() << " @ " << it.Index() << " \n";
        return __LINE__;
      }
    }
  }

  return 0;
}


int test16bitPNG() {

  ImageC<UInt16RGBValueC> im1(50,50);
  UInt16RGBValueC p(0,0,0);
  for (Array2dIterC<UInt16RGBValueC> i(im1); i; ++i) {
    *i = p;
    p.Red()++;
  }
  Save("/tmp/tmp.png", im1);
  ImageC<RealRGBValueC> im2;
  Load("/tmp/tmp.png", im2);
  ImageC<UInt16RGBValueC> im3 = RealRGBImageCT2UInt16RGBImageCT(im2);
  for (Array2dIterC<UInt16RGBValueC> i(im1-im3); i; ++i) 
    if (*i != UInt16RGBValueC(0,0,0)) {
      std::cout << i.Index() << " " << *i << std::endl;
      return __LINE__;
    }

//   ImageC<ByteRGBValueC> im4;
//   Load("/tmp/tmp.png", im4);
//   Save("@X", im4);

  return 0;
}





int main(int argc,char **argv) {  
  int ln;
  if((ln = testImgMemIO<ByteT>("",false)) != 0) {
    std::cerr << "Test failed on line:" << ln << "\n";
    return 1;
  }
  if((ln = testImgMemIO<ByteT>("jpeg",true)) != 0) {
    std::cerr << "Test failed on line:" << ln << "\n";
    return 1;
  }
  if((ln = testImgMemIO<ByteT>("png",false)) != 0) {
    std::cerr << "Test failed on line:" << ln << "\n";
    return 1;
  }
  if((ln = testImgMemIO<UInt16T>("png",false)) != 0) {
    std::cerr << "Test failed on line:" << ln << "\n";
    return 1;
  }
  if((ln = test16bitPNG()) != 0) {
    std::cerr << "Test failed on line:" << ln << "\n";
    return 1;
  }
  std::cout << "Test passed ok. \n";
  return 0;
}


