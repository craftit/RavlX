// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/SqrComposition.cc"

#include "Ravl/Image/SqrComposition.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Math.hh"
#include "Ravl/SArray1d.hh"

namespace RavlImageN {
  
  using namespace RavlN;

  //: Calculate the maginute image.
  
  bool SqrCompositionC::Apply(const ImageC<TFVectorC<RealT,2> > &inImg,ImageC<RealT> &out) const {
    if(out.IsEmpty())
      out = ImageC<RealT>(inImg.Rectangle());
    for(Array2dIter2C<RealT,TFVectorC<RealT,2> > it(out,inImg);it;it++) {
      it.Data1() = (it.Data2()[0] != 0.0 || it.Data2()[1] != 0.0)
	? Sqrt(Sqr(it.Data2()[0]) + Sqr(it.Data2()[1]))
	: RealT(0.0);
    }
    return true;
  }
  
  bool SqrCompositionC::Apply(const ImageC<RealT> &img1,const ImageC<RealT>  &img2,ImageC<RealT> &res) const {
    ImageRectangleC rect(img1.Rectangle());
    rect.ClipBy(img2.Rectangle());
    RavlAssert(rect.Area() > 0); // There should be something to process.
    if(res.IsEmpty())
      res = ImageC<RealT>(rect);    
    for(Array2dIter3C<RealT,RealT,RealT> it(res,img1,img2);
	it.IsElm();it.Next()) {
      it.Data1() = (it.Data2() != 0.0 || it.Data3() != 0.0)
	? Sqrt(Sqr(it.Data2()) + Sqr(it.Data3()))
	: RealT(0.0);
    } 
    return true;
  }
  
  void SqrCompositionC::DoSqrComp(ImageC<RealT> &res,ImageC<RealT> &img1,ImageC<RealT> &img2,IntT startRow,IntT endRow) const {
    
    ImageRectangleC rect(res.Rectangle());
    rect.Origin().Row() = startRow;
    rect.End().Row() = endRow;
    for(Array2dIter3C<RealT,RealT,RealT> it(res,
					    img1,
					    img2,rect);
	it.IsElm();it.Next())  {
      it.Data1() = (it.Data2() != 0.0 || it.Data3() != 0.0)
	? Sqrt(Sqr(it.Data2()) + Sqr(it.Data3()))
	: RealT(0.0);
    }
  }
  
}
