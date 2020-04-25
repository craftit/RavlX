// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Radek Marik, modified by Charles Galambos"
//! date="1/10/1991"
//! file="Ravl/Image/Processing/Corners/CornerDetectorHarris.cc"

#include "Ravl/Image/CornerDetectorHarris.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array2dSqr31Iter2.hh"
#include "Ravl/Image/PeakDetector.hh"
#include "Ravl/Image/Gradient.hh"
#include "Ravl/MeanVariance.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"
#include "Ravl/Image/SumRectangles.hh"
#include "Ravl/GenerateSignal1d.hh"


#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  ///////////////////////////////////
  // Constructor.
  
  CornerDetectorHarrisBodyC::CornerDetectorHarrisBodyC(int nTheshold,int nW,bool nUseTopHat)
    : w(nW),
      threshold(nTheshold),
      useTopHat(nUseTopHat)
  {
    if(!useTopHat) {
      // Setup gaussian filter.
      Array1dC<IntT> mask = GenerateBinomial((IntT) 1,w,false,true);
      filter = ConvolveSeparable2dC<TFVectorC<IntT,3>,TFVectorC<IntT,3>,IntT,TFVectorC<IntT,3> >(mask,mask);
      maskSum = (RealT) Sqr(mask.Sum());
    } else {
      IntT half = (w - 1)/2;
      IndexRange2dC win(-half,half,-half,half);
      maskSum = win.Area();
    }
    
    ONDEBUG(cerr << "Mask size=" << w << " Sum=" << maskSum << "\n");
    //ONDEBUG(cerr << "Mask=" << mask << "\n");
  }
  
  DListC<CornerC> CornerDetectorHarrisBodyC::Apply(const ImageC<ByteT> &img) {
    ImageC<IntT> var;
    var = CornerHarris(img);
    DListC<CornerC> lst;
    Peak(var,img,lst);
    return lst;
  }
  
  // Haris corner detector.
  
  ImageC<IntT> CornerDetectorHarrisBodyC::CornerHarris(const ImageC<ByteT> &img) {
    ImageRectangleC workRect(img.Rectangle().Shrink(1));
    if(!vals.Frame().Contains(workRect)) {
      vals = ImageC<TFVectorC<IntT,3> >(workRect);
      fvals = ImageC<TFVectorC<IntT,3> >(workRect);
      var = ImageC<IntT>(workRect);               /* cornerness image */
      var.Fill(0); // Ensure the borders are zero.
    }
    
    ImagGrad(img,vals);
    ImageRectangleC subWR(workRect.Shrink(w+1));
    if(useTopHat) {
      // Use top hat filter.
      IntT half = (w - 1)/2;
      IndexRange2dC win(-half,half,-half,half);
      SumRectangles(vals,win,fvals);
    } else {
      // Use gaussian filter.
      filter(vals,fvals);
    }
    for(Array2dIter2C<IntT,TFVectorC<IntT,3> > it(var,fvals,subWR);it;it++) {
      RealT ixixs = (RealT) it.Data2()[0];
      RealT iyiys = (RealT) it.Data2()[1]; 
      RealT ixiys = (RealT) it.Data2()[2];
      /* Evaluating the cornerness measure */
      RealT num = ixixs+iyiys;
      if(num != 0) {
	it.Data1() = Round((ixixs*iyiys-ixiys*ixiys)/(num * maskSum));
      } else
	it.Data1() = 0;
      //cerr << "Val=" << val << "\n";
    }
    return var;
  }
  
  void CornerDetectorHarrisBodyC::ImagGrad(const ImageC<ByteT> &img,ImageC<TFVectorC<IntT,3> > &val) {
    for(Array2dSqr31Iter2C<ByteT,TFVectorC<IntT,3> > it(img,val);it;it++) {
      /* Calculation of the gradients in x and y direction */
      int ix = ((int) it.DataBL1() + it.DataML1()*2 + it.DataTL1() - it.DataBR1() - it.DataMR1()*2 - it.DataTR1()) >> 1;
      int iy = ((int) it.DataTL1() + it.DataTM1()*2 + it.DataTR1() - it.DataBL1() - it.DataBM1()*2 - it.DataBR1()) >> 1;
      it.Data2()[0]=ix*ix;
      it.Data2()[1]=iy*iy;
      it.Data2()[2]=ix*iy;
    }
  }
  
  
  int CornerDetectorHarrisBodyC::Peak(ImageC<IntT> &result,const ImageC<ByteT> &in,DListC<CornerC> &cornLst) {
    IndexRange2dC rect(in.Frame().Shrink(4));
    int n = 0;
    for(Array2dIterC<IntT> it(result,rect);it;it++) {
      if(*it < threshold)
	continue;
      Index2dC at = it.Index();
      if(PeakDetect7(result,at)) {
	Point2dC pat = LocatePeakSubPixel(result,at,0.25);
	cornLst.InsLast(CornerC(pat,SobelGradient(in,at),in[at]));
	n++;
      }
    }
    return n; 
  }  
}
