// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Edges/EdgeDeriche.cc"

#include "Ravl/Image/EdgeDeriche.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Math.hh"
#include "Ravl/Stream.hh"

#if RAVL_USE_PARALLEL
#include "Ravl/Threads/LaunchThread.hh"
#endif

#define DODEBUG 0

#if DODEBUG
#include "Ravl/DP/FileFormatIO.hh"
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlImageN {

  //: Constructor.
  
  EdgeDericheBodyC::EdgeDericheBodyC(RealT nOmega,RealT nAlpha)
    : omega(nOmega), 
      alpha(nAlpha)
  { 
    Init(); 
    ONDEBUG(cerr << "EdgeDericheBodyC::EdgeDericheBodyC(), Omega " << omega << " Alpha:" << alpha << " \n");
  }
  
  //: Calculate the edge magnitude only
  
  ImageC<RealT> EdgeDericheBodyC::EdgeMagnitude(const ImageC<RealT> &inImg) const {
    ImageC<RealT> dx(inImg.Rectangle());
    ImageC<RealT> dy(inImg.Rectangle());
    if(!Apply(inImg,dx,dy))
      return ImageC<RealT>();
    ImageC<RealT> ret(inImg.Rectangle());
    for(Array2dIter3C<RealT,RealT,RealT> it(ret,dx,dy);it;it++)
      it.Data1() = Hypot(it.Data2(),it.Data3());    
    return ret;
  }
    

  //: Compute filter paramiters.
  
  bool EdgeDericheBodyC::Init() {
    cst0 = alpha * alpha + omega * omega;
    cst  = cst0 * cst0 / (4 * alpha * omega);
    cst1 = -cst * alpha / cst0;
    cst2 = -cst * omega / cst0;
    ad1  = -2 * Cos(omega) * Exp(-alpha);
    ad2  = Exp(-2 * alpha);
    an1  = cst2;
    an2  = - cst2 * Cos(omega) * Exp(-alpha)
      + cst1 * Sin(omega) * Exp(-alpha);
    an3  = an2 - cst2 * ad1;
    an4  = -cst2 * ad2;
    an11 = cst * Sin(omega) * Exp(-alpha);
    return true;
  }

  
  ///// XDer ///////////////////////////////////////////////////////////////
  
  bool EdgeDericheBodyC::YDer(ImageC<RealT> &im,ImageC<RealT> &derv) const {
    
    ImageC<RealT> a2(im.Rectangle());
    ImageC<RealT> a3(im.Rectangle());
    ImageC<RealT> a4(derv);
    
#if RAVL_USE_PARALLEL  
    EdgeDericheC me(*this);
    LaunchThreadC oth = LaunchThread(me,&EdgeDericheC::YDer1p,im,a2);
    YDer1n(im,a3); 
    oth.Done().Wait();  
#else
    YDer1p(im,a2);
    YDer1n(im,a3); 
#endif
  
    for(Array2dIter2C<RealT,RealT> it(a2,a3);
	it.IsElm();it.Next())
      it.Data1() += it.Data2();
    
#if RAVL_USE_PARALLEL  
    oth = LaunchThread(me,&EdgeDericheC::YDer2p,im,a2,a3);
    YDer2n(im,a2,a4);
    oth.Done().Wait();
#else
    YDer2p(im,a2,a3);
    YDer2n(im,a2,a4); 
#endif
    
    // stock results of 1-st passage
    
    for(Array2dIter2C<RealT,RealT> itx(a4,a3);itx;itx++)
      itx.Data1() += itx.Data2();
    return true;
  }
  
  bool EdgeDericheBodyC::YDer1p(ImageC<RealT> &im,ImageC<RealT> &a2) const{
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC i= i_min; i <= i_max; ++i) {
      a2[i][j_min+0] =  an1 * im[i][j_min+0];
      a2[i][j_min+1] =  an1 * im[i][j_min+1]
	+ an2 * im[i][j_min+0]
	- ad1 * a2[i][j_min+0];
      for (IndexC j = j_min+2; j <= j_max; ++j)
	a2[i][j] =  an1 * im[i][j]   + an2 * im[i][j-1]
	  - ad1 * a2[i][j-1] - ad2 * a2[i][j-2];
    }
    return true;
  }
  
  bool EdgeDericheBodyC::YDer1n(ImageC<RealT> &im,ImageC<RealT> &a3) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC i = i_min; i <= i_max; ++i) {
      a3[i][j_max] = 0;
      a3[i][j_max-1] = an3 * im[i][j_max];
      for (IndexC j = j_max-2; j >= j_min; --j)
	a3[i][j] =   an3 * im[i][j+1] + an4 * im[i][j+2]
	  - ad1 * a3[i][j+1] - ad2 * a3[i][j+2];
    }
    return true;
  }
  
  bool EdgeDericheBodyC::YDer2p(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a3) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC j = j_min; j <= j_max; ++j) {
      a3[i_min+0][j] = 0.0;
      a3[i_min+1][j] = an11 * a2[i_min+0][j];
      for (IndexC i = i_min+2; i <= i_max; ++i)
	a3[i][j] = an11 * a2[i-1][j] - ad1 * a3[i-1][j] - ad2 * a3[i-2][j];
    }
    return true;
  }
  
  bool EdgeDericheBodyC::YDer2n(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a4) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC j = j_min; j <= j_max; ++j) {
      a4[i_max][j] = 0.0;
      a4[i_max-1][j] = -an11 * a2[i_max][j];
      for (IndexC i = i_max-2; i >= i_min+0; --i)
	a4[i][j] = - an11 * a2[i+1][j] - ad1 * a4[i+1][j] - ad2 * a4[i+2][j];
    }
    return true;
  }
  
  ///// XDer ///////////////////////////////////////////////////////////////
  
  bool EdgeDericheBodyC::XDer(ImageC<RealT> &im,ImageC<RealT> &derv) const {
    ImageC<RealT> a2(im.Rectangle());
    ImageC<RealT> a3(im.Rectangle());
    ImageC<RealT> a4(derv);
    
#if RAVL_USE_PARALLEL
    LaunchThreadC oth = LaunchThread(EdgeDericheC(*this),&EdgeDericheC::XDer1p,im,a2);
    XDer1n(im,a3); 
    oth.Done().Wait();  
#else
    XDer1p(im,a2);
    XDer1n(im,a3); 
#endif
    
    for(Array2dIter2C<RealT,RealT> it(a2,a3);
	it.IsElm();it.Next())
      it.Data1() += it.Data2();
    
#if RAVL_USE_PARALLEL  
    oth = LaunchThread(EdgeDericheC(*this),&EdgeDericheC::XDer2p,im,a2,a3);
    XDer2n(im,a2,a4);
    oth.Done().Wait();
#else
    XDer2p(im,a2,a3);
    XDer2n(im,a2,a4); 
#endif
    
    for(Array2dIter2C<RealT,RealT> itx(a4,a3);itx;itx++)
      itx.Data1() += itx.Data2();  
    return true;
  }
  
  bool EdgeDericheBodyC::XDer1p(ImageC<RealT> &im,ImageC<RealT> &a2) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC i= i_min; i <= i_max; ++i) {
      a2[i][j_min+0] = 0.0;
      a2[i][j_min+1] = an11 * im[i][j_min+0];
      for (IndexC j = j_min+2; j <= j_max; ++j)
	a2[i][j] = an11 * im[i][j-1] - ad1 * a2[i][j-1] - ad2 * a2[i][j-2];
    }    
    return true;
  }
  
  bool EdgeDericheBodyC::XDer1n(ImageC<RealT> &im,ImageC<RealT> &a3) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC i = i_min; i <= i_max; ++i) {
      a3[i][j_max] = 0.0;
      a3[i][j_max-1] = -an11 * im[i][j_max];
      for (IndexC j = j_max-2; j >= j_min; --j)
	a3[i][j] = -an11 * im[i][j+1] - ad1 * a3[i][j+1] - ad2 * a3[i][j+2];
    }
    return true;
  }
  
  bool EdgeDericheBodyC::XDer2p(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a3) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC j = j_min; j <= j_max; ++j) {
      a3[i_min+0][j] = an1 * a2[i_min+0][j];
      a3[i_min+1][j] =   an1 * a2[i_min+1][j] + an2 * a2[i_min+0][j]
	- ad1 * a3[i_min+0][j];
      for (IndexC i = i_min+2; i <= i_max; ++i)
	a3[i][j] =   an1 * a2[i][j] + an2 * a2[i-1][j]
	  - ad1 * a3[i-1][j] - ad2 * a3[i-2][j];
    }
    return true;
  }
  
  bool EdgeDericheBodyC::XDer2n(ImageC<RealT> &im,ImageC<RealT> &a2,ImageC<RealT> &a4) const {
    const IndexC i_min = im.TRow();
    const IndexC i_max = im.BRow();
    const IndexC j_min = im.LCol();
    const IndexC j_max = im.RCol();
    for (IndexC j = j_min; j <= j_max; ++j) {
      a4[i_max][j]   = 0.0;
      a4[i_max-1][j] = an3 * a2[i_max][j];
      for (IndexC i = i_max-2; i >= i_min; --i)
	a4[i][j] =   an3 * a2[i+1][j] + an4 * a2[i+2][j]
	  - ad1 * a4[i+1][j] - ad2 * a4[i+2][j];
    }
    return true;
  }
  
  //: Apply operator.
  
  bool EdgeDericheBodyC::Apply(const ImageC<RealT> &img,ImageC<RealT> &dx,ImageC<RealT> &dy) const {
    if(img.Cols() < 2 || img.Rows() < 2) {
      cerr << "EdgeDericheBodyC::Apply(), Image too small. " << img.Cols() << " " << img.Rows() << endl;
      return false;
    }
    
    ImageC<RealT> im(img);
    // Set up images to receive results.
    if(dx.IsEmpty())
      dx = ImageC<RealT>(img.Rectangle());
    if(dy.IsEmpty())
      dy = ImageC<RealT>(img.Rectangle());
    
    // Do some processing.
#if RAVL_USE_PARALLEL
    LaunchThreadC oth = LaunchThread(EdgeDericheC(*this),&EdgeDericheC::YDer,im,dx);
    XDer(im,dy);
    oth.Done().Wait();
#else
    YDer(im,dx);
    XDer(im,dy);
#endif
    //ONDEBUG(Save("@X:XDer",a2));  
    //Tuple2C< >(a4,a1);
    return true;
  }

  //: Apply sobol operator to 'img', put result in 'out'
  
  bool EdgeDericheBodyC::Apply(const ImageC<RealT> &inImg,ImageC<TFVectorC<RealT,2> > &out) const{
    if(out.IsEmpty())
      out = ImageC<TFVectorC<RealT,2> >(inImg.Rectangle());
    ImageC<RealT> dx(out.Rectangle());
    ImageC<RealT> dy(out.Rectangle());
    if(!Apply(inImg,dx,dy))
      return false;
    TFVectorC<RealT,2> x;
    for(Array2dIter3C<TFVectorC<RealT,2>,RealT,RealT> it(out,dx,dy);it;it++) {
      x[0] = it.Data2();
      x[1] = it.Data3();
      it.Data1() = x;
    }
    return true;
  }
  
}
