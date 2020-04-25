// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tracking/ForegroundSep.cc"

#include "Ravl/Image/ForegroundSep.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/WarpProjective.hh"
#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Image/Erode.hh"
#include "Ravl/Image/Dilate.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImgIO.hh"
#include "Ravl/Image/RealRGBValue.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  ForegroundSepC::ForegroundSepC(const ImageC<ByteRGBValueC>& nmosaic, RealT nfgThreshold, bool nprogressive) 
    : mosaic(nmosaic),
      fgThreshold(Sqr(nfgThreshold)*3), // saves computation later
      progressive(nprogressive)
{ 
  SetOpening(1); 
  SetFilter(7);
}
  
  
  //: Set binary mask to exclude regions from tracker
  bool ForegroundSepC::SetMask(const StringC& fileName) {
    if (!Load(fileName, mask)) {
      std::cerr << "Failed to load file: " << fileName << std::endl;
      return false;
    }
    return true;
  }
  
  
  //: Set morphological opening structuring element
  void ForegroundSepC::SetOpening(IntT Width) {
    IndexRangeC centred(-Width,Width);
    kernel = ImageC<bool>(centred,centred);
    kernel.Fill(true);
    switch (Width) {
    case 0: case 1: 
      break; 
    case 2:
      // 5x5 kernel for morphological operations, with this shape:
      //  ***
      // *****
      // *****
      // *****
      //  ***
      kernel[-2][-2] = kernel[-2][2] = kernel[2][-2] = kernel[2][2] = false;
      break;
    default: 
      std::cerr<<"Opening size  of "<<Width<<" not supported."<<std::endl;
      SetOpening(0);
    }
    if (!progressive) { // delete alternate rows for interlaced version
      for (IndexC r(-Width+1); r<=Width; r+=2)
	for (IntT c(-Width); c<=Width; ++c)
	  kernel[r][c] = kernel[r][c] = false; 
    }
  }
  

//: Set width of Gaussian low-pass filter
void ForegroundSepC::SetFilter(IntT Width) { 
  if (progressive) {
    lpf = GaussConvolve2dC<ByteRGBValueC, ByteRGBValueC, RealT, RealRGBValueC>(Width);
  }
  else { // interlaced (field-based)
    if (Width%2 == 0) {
      ++Width;
      std::cerr << "Even-sized filters not allowed; increasing size to " << Width << std::endl;
    }
    Array1dC<RealT> hkernel = GenerateBinomial(1.0, Width, true, true);
    // create vertical kernel with every other coeff = 0
    Array1dC<RealT> vcoeff = GenerateBinomial(1.0, (Width+1)/2, true, false);
    Array1dC<RealT> vkernel(-Width/2, Width/2);
    vkernel.Fill(0.0);
    for (UIntT i(0); i<vcoeff.Size(); ++i)
      vkernel[vkernel.IMin() + 2*i] = vcoeff[i];
    lpf = ConvolveSeparable2dC<ByteRGBValueC,ByteRGBValueC,RealT,RealRGBValueC>(vkernel,hkernel);
  }
  filterWidth = Width; 
}


//: Computes foreground image for corresponding input image and homography
ImageC<ByteRGBValueC> ForegroundSepC::Apply(const ImageC<ByteRGBValueC>& img,  Projection2dC& homog) {
    // warp mosaic into image space
    ImageC<ByteRGBValueC> warped_img(img.Rectangle());
    WarpProjectiveC<ByteRGBValueC,ByteRGBValueC> pwarp(img.Rectangle(),homog);
    pwarp.Apply(mosaic,warped_img);
    //Save("@X:mosaic", mosaic); Save("@X:image", img); Save("@X:warp", warped_img);
    
    // smooth both images to suppress artefacts
    ImageC<ByteRGBValueC> fg;
    if (filterWidth > 1) {
      fg = lpf.Apply(img);
      warped_img = lpf.Apply(warped_img);
    }
    else fg = img.Copy();
    // subtract and threshold
    ImageC<bool> outmask(img.Rectangle());
    for (Array2dIter3C<ByteRGBValueC,ByteRGBValueC,bool>
	   it(fg,warped_img,outmask,fg.Rectangle()); it; it++) {
      IntT diff2 = ((RGBValueC<IntT>)it.Data1() - (RGBValueC<IntT>)it.Data2()).SumOfSqr();
      it.Data3() = (diff2 >= fgThreshold);
    }
    
    // exclude pixels from graphics mask (if used)
    if (mask.Rectangle().Contains(outmask.Rectangle())) {
      // (will be true if mask has been used)
      for (Array2dIter2C<bool,bool> it(mask,outmask,outmask.Rectangle());
	   it; it++) {
	it.Data2() &= it.Data1();
      }
    }
    
    // erode and dilate binary mask
    if (kernel.Size() > 1) {
      ImageC<bool> result;
      BinaryErode(outmask, kernel, result);
      BinaryDilate(result, kernel, outmask);
    }

    // blank out background pixels
    ByteRGBValueC black(0,0,0);
    for(Array2dIter2C<ByteRGBValueC,bool> it(fg,outmask,fg.Rectangle());
	it.IsElm(); it.Next()) {
      if ( !it.Data2() ) it.Data1() = black;
    }
    return fg;
  }
}
