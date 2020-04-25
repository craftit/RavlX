// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/testConvolve.cc"
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Filtering"

#include "Ravl/Image/Convolve2d.hh"
#include "Ravl/Image/ConvolveHorz2d.hh"
#include "Ravl/Image/ConvolveVert2d.hh"
#include "Ravl/Image/ConvolveSeparable2d.hh"
#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/Image/PixelMixer.hh"
#include "Ravl/Image/WarpScale.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/Image/WarpProjective.hh"
#include "Ravl/Image/WarpThinPlateSpline.hh"
#include "Ravl/Image/HistogramEqualise.hh"
#include "Ravl/Image/Matching.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/SpatialDifference.hh"
#include "Ravl/Image/SumRectangles.hh"
#include "Ravl/Image/Rectangle2dIter.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/DCT2d.hh"
#include "Ravl/Image/ImageExtend.hh"
#include "Ravl/Image/ImagePyramid.hh"
#include "Ravl/Random.hh"
#include "Ravl/CollectionIter.hh"
#include "Ravl/config.h"
#include "Ravl/IO.hh"

using namespace RavlImageN;

#define TESTMMX 0

int testConvolve2d();
int testConvolveHorz2d();
int testConvolveVert2d();
int testConvolveSeparable2d();
int testConvolve2dMMX();
int testGaussConvolve();

int testWarpScale();
int testWarpThinPlateSpline();
int testHistogramEqualise();
int testMatching();
int testSpatialDifference();
int testSumRectangles();
int testDCT();
int testImageExtend();
int testImagePyramid();
int testImagePyramidRGB();


#if RAVL_HAVE_TEMPLATEINSTANTIATE
template WarpScaleC<ByteRGBValueC,ByteRGBValueC>;
template WarpAffineC<ByteT,ByteT>;
template WarpProjectiveC<ByteT,ByteT>;
template WarpProjectiveC<ByteRGBValueC,ByteRGBValueC>;
#else
#include "Ravl/EvaluateNumInliers.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Image/ByteRGBMedian.hh"
#include "Ravl/FitHomog2dPoints.hh"
#include "Ravl/Ransac.hh"
#include "Ravl/LevenbergMarquardt.hh"
#include "Ravl/ObservationManager.hh"
#include "Ravl/SArray2d.hh"

static EvaluateNumInliersBodyC dummyvar1(0.0,0.0) ;
static CollectionBodyC<RavlImageN::ByteRGBGreyValueC> dummyvar2 ;
static FitHomog2dPointsBodyC dummyvar3(0.0,0.0) ;
static ObservationManagerC dummyvar4 ; 
static FitToSampleC dummyvar5 ; 
static EvaluateSolutionC dummyvar6 ; 
static RansacC dummyvar7 ( dummyvar4, dummyvar5, dummyvar6) ;
static StateVectorC dummyvar8 (VectorC(1) );
static DListC<ObservationC> dummyvar9 ; 
static LevenbergMarquardtC dummyvar10( dummyvar8, dummyvar9);
static ObservationListManagerBodyC dummyvar11 (dummyvar9);


#endif

int main() {
  int ln;
#if 1
#if !TESTMMX
  if((ln = testConvolve2d()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testConvolveHorz2d()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testConvolveVert2d()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testConvolveSeparable2d()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
#endif
  if((ln = testConvolve2dMMX()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testHistogramEqualise()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testMatching()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testSpatialDifference()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testSumRectangles()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testWarpThinPlateSpline()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testGaussConvolve()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testWarpScale()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testImageExtend()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testImagePyramid()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testImagePyramidRGB()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
#endif
  if((ln = testDCT()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  cerr << "Test passed ok. \n";
  return 0;
}

int testConvolve2d() {
  cerr << "testConvolve2d(), Started... \n";
  ImageC<IntT> kernel(IndexRangeC(-1,1),IndexRangeC(-1,1));
  kernel.Fill(0);
  kernel[0][0] = 2;
  kernel[0][1] = -1;
  kernel[1][0] = -1;
  kernel[0][-1] = 1;
  kernel[-1][0] = 1;
  Convolve2dC<IntT> conv(kernel);
  ImageC<IntT> test(10,10);
  test.Fill(0);
  test[3][5] = 1;
  test[6][3] = 1;
  ImageC<IntT> res;
  conv.Apply(test,res);
  //cout << res;
  IntT sum = 0;
  for(Array2dIterC<IntT> it(res);it;it++)
    sum += *it;
  if(sum != 4) return __LINE__;
  // Check a few values....
  if(res[3][5] != 2) return __LINE__;
  if(res[6][3] != 2) return __LINE__;
  if(res[2][5] != 1) return __LINE__;
  if(res[2][4] != 0) return __LINE__;
  return 0;
}


int testConvolveHorz2d() {
  cerr << "testConvolveHorz2d(), Started... \n";
  Array1dC<IntT> filt(-1,1);
  filt[-1] = 1;
  filt[0] = 2;
  filt[1] = -1;
  ConvolveHorz2dC<IntT> conv(filt);
  ImageC<IntT> test(10,10);
  test.Fill(0);
  test[3][5] = 1;
  test[6][3] = 1;
  //cout << test;
  ImageC<IntT> res;
  conv.Apply(test,res);
  //cout << res;
  IntT sum = 0;
  for(Array2dIterC<IntT> it(res);it;it++)
    sum += *it;
  if(sum != 4) return __LINE__;
  if(res[3][5] != 2) return __LINE__;
  if(res[6][3] != 2) return __LINE__;
  if(res[7][3] != 0) return __LINE__;
  return 0;
}

int testConvolveVert2d() {
  cerr << "testConvolveVert2d(), Started... \n";
  Array1dC<IntT> filt(-1,1);
  filt[-1] = 1;
  filt[0] = 2;
  filt[1] = -1;
  ConvolveVert2dC<IntT> conv(filt);
  ImageC<IntT> test(10,10);
  test.Fill(0);
  test[3][5] = 1;
  test[6][3] = 1;
  //cout << test;
  ImageC<IntT> res;
  conv.Apply(test,res);
  //cout << res;
  IntT sum = 0;
  for(Array2dIterC<IntT> it(res);it;it++)
    sum += *it;
  if(sum != 4) return __LINE__;
  if(res[3][5] != 2) return __LINE__;
  if(res[6][3] != 2) return __LINE__;
  if(res[6][4] != 0) return __LINE__;
  return 0;
}

int testConvolveSeparable2d() {
  cerr << "testConvolveSeparable2d(), Started... \n";
  Array1dC<IntT> filt(-1,1);
  filt[-1] = 1;
  filt[0] = 2;
  filt[1] = -1;
  ConvolveSeparable2dC<IntT> conv(filt,filt);
  ImageC<IntT> test(10,10);
  test.Fill(0);
  test[3][5] = 1;
  test[6][3] = 1;
  //cout << test;
  ImageC<IntT> res;
  conv.Apply(test,res);
  //cout << res;
  IntT sum = 0;
  for(Array2dIterC<IntT> it(res);it;it++)
    sum += *it;
  if(sum != 8) return __LINE__;
  if(res[3][5] != 4) return __LINE__;
  if(res[6][3] != 4) return __LINE__;
  if(res[6][4] != -2) return __LINE__;
  return 0;
}

int testConvolve2dMMX() {
  cerr <<"testConvolve2dMMX(), Started. \n";
  ImageC<short> kernel(IndexRangeC(-2,2),IndexRangeC(-2,2));
  kernel.Fill(0);
  kernel[0][0] = 2;
  kernel[0][1] = -1;
  kernel[1][0] = -1;
  kernel[0][-1] = 1;
  kernel[-1][0] = 1;
  Convolve2dC<short,short,short,short> conv(kernel);
  ImageC<short> test(200,200);
  test.Fill(0);
  test[3][5] = 1;
  test[6][3] = 1;
  ImageC<short> res;
  for(int i = 0;i < 30;i++) {
    conv.Apply(test,res);
    //cout << res;
  }
#if 0
  IntT sum = 0;
  for(Array2dIterC<short> it(res);it;it++)
    sum += *it;
  if(sum != 4) return __LINE__;
  // Check a few values....
  if(res[3][5] != 2) return __LINE__;
  if(res[6][3] != 2) return __LINE__;
  if(res[2][5] != 1) return __LINE__;
  if(res[2][4] != 0) return __LINE__;
  return 0;
#endif
  
  return 0;
}


// Tag this on the end for the moment.

int testWarpScale() {
  cerr << "testWarpScale(), Started... \n";
  typedef RealT PixelT;
  ImageC<PixelT> test(800,800);
  for(int i = 0;i <100;i++) {
    for(Array2dIterC<PixelT> it(test);it;it++)
      *it = i++;
    //cerr << test << "\n";
    ImageRectangleC resRect(0,400,0,400);
    WarpScaleC<PixelT,PixelT> xyz(resRect);
    ImageC<PixelT> res = xyz.Apply(test);
    if(res.Rectangle() != resRect) return __LINE__;
    //cerr << res << "\n";
    if(res[resRect.Origin()] != 0) return __LINE__;
  }
  // Do some more checking here.
  return 0;
}

#if RAVL_HAVE_TEMPLATEINSTANTIATE
template HistogramEqualiseC<RealT>;
template HistogramEqualiseC<ByteT>;
#endif

int testHistogramEqualise() {
  cerr << "testHistogramEqualise() \n";
  // Test equalisation of real images.
  ImageC<RealT> test(10,10);
  RealT v = 0;
  for(Array2dIterC<RealT> it(test);it;it++)
    *it = v++;
  HistogramEqualiseC<RealT> histEqual(0,100);
  ImageC<RealT> result = histEqual.Apply(test);
  //cerr << "Test=" << test << "\n Result=" << result << "\n";
  //cerr << (result-test) << "\n";
  if((result-test).SumOfSqr() > 0.000001) return __LINE__;
  
  // Test equalisation of byte images
  ImageC<ByteT> testb(10,10);
  ByteT vb = 0;
  for(Array2dIterC<ByteT> itb(testb);itb;itb++)
    *itb = vb++;
  HistogramEqualiseC<ByteT> histEqualb(0,100);
  ImageC<ByteT> resultb = histEqualb.Apply(testb);
  //cerr << "Test=" << testb << "\n Result=" << resultb << "\n";
  //if((resultb-testb).SumOfSqr() > 0.000001) return __LINE__;
  
  return 0;
}

int testMatch(int s) {
  Array2dC<ByteT> img1(s,s);
  Array2dC<ByteT> img2(s,s);
  int v = 0;
  for(Array2dIterC<ByteT> it(img1);it;it++)
    *it =  (v++ % 32) + 1;
  v = 0;
  for(Array2dIterC<ByteT> it(img2);it;it++)
    *it = (v++ % 32) + 2;
  Index2dC at(0,0);
  IntT sum;
  sum = RavlImageN::MatchSumAbsDifference(img1,img2,at,sum);
  //cerr << "Sum=" << sum << "\n";
  if(sum != s * s)
    return __LINE__;
  return 0;
}

int testMatching() {
  cerr << "testMatching(). \n";
  int ln;
  for(int i = 3;i < 19;i++) {
    cerr << "Testing " << i << "x" << i << "\n";
    if((ln = testMatch(i)) != 0)
      return ln;
  }
  return 0;
}

int testSpatialDifference() {
  cerr << "testSpatialDifference(). \n";

  Array2dC<ByteT> img1(10,10);
  int v = 0;
  for(Array2dIterC<ByteT> it(img1);it;it++,v++)
    *it = v;
  
  for(int i = 1;i < 4;i++) {
    ImageC<Vector2dC> grad;
    cerr << "Checking " << i << "\n";
    SpatialDifference(i,img1,grad);
    //cerr << "Grad=" << grad << "\n";
#if 0
    for(Array2dIterC<Vector2dC> it(grad);it;it++) {
      if((*it - Vector2dC(10,1)).SumOfSqr() > 0.000000001)
	return __LINE__;
    }
#endif
  }
  
  return 0;
}

int testSumRectangles() {
  cerr << "testSumRectangles(). \n";
  Array2dC<IntT> test(5,5);
  int i = 0;
  for(Array2dIterC<IntT> it(test);it;it++)
    *it = i++;
  IndexRange2dC mask(-1,1,-1,1);
  Array2dC<IntT> result;
#if 1
  SumRectangles(test,mask,result);
  //  cerr << "Result1=" << result << "\n";
  
  if(result[1][1] != 54)
    return __LINE__;
  if(result[1][2] != 63)
    return __LINE__;
  if(result[1][3] != 72)
    return __LINE__;
  if(result[2][1] != 99)
    return __LINE__;
  if(result[2][2] != 108)
    return __LINE__;
  if(result[2][3] != 117)
    return __LINE__;
  if(result[3][1] != 144)
    return __LINE__;
  if(result[3][2] != 153)
    return __LINE__;
  if(result[3][3] != 162)
    return __LINE__;
#endif
  
  Array2dC<IntT> test3(10,12);
  for(Array2dIterC<IntT> it(test3);it;it++)
    *it = RandomInt() % 9;
  //cerr << "Img=" << test3 << "\n";
  
  for(int z = 0;z < 3;z++) {
    for(int i = 1;i < 3;i++) {
      for(int j = 1;j < 3;j++) {
	IndexRange2dC mask3(-i,i,-j,j);
	if(z == 1)
	  mask3.RCol()++;
	if(z == 2)
	  mask3.BRow()++;
	
	cerr << "Testing mask " << mask3 << "\n";
	Array2dC<IntT> result3;
	
	SumRectangles(test3,mask3,result3);
	//cerr << "Result3=" << result3 << "\n";
	for(Rectangle2dIterC itr(test3.Frame(),mask3);itr;itr++) {
	  IntT sum = 0;
	  for(Array2dIterC<IntT> its(test3,itr.Window());its;its++)
	    sum += *its;
	  Index2dC at = itr.Window().Origin() - mask3.Origin();
	  if(sum != result3[at]) {
	    cerr << "Incorrect sum at " << at << " sum=" << sum << "\n";
	    return __LINE__;
	  }
	}
      }
    }
  }
  
  return 0;
}

int testWarpThinPlateSpline() {
  RealT sigma = 4;
  ImageC<ByteT> inImg(10,10);
  inImg.Fill(0);
  Array1dC<Point2dC> orgPnts(5);
  orgPnts[0] = Point2dC(10,10);
  orgPnts[1] = Point2dC(100,200);
  orgPnts[2] = Point2dC(200,200);
  orgPnts[3] = Point2dC(200,100);
  orgPnts[4] = Point2dC(50,50);
  
  Array1dC<Point2dC> newPnts(5);
  newPnts[0] = Point2dC(10,11);
  newPnts[1] = Point2dC(102,201);
  newPnts[2] = Point2dC(204,201);
  newPnts[3] = Point2dC(200,106);
  newPnts[4] = Point2dC(55,55);
  
  WarpThinPlateSplineC<ByteT> warp(sigma);
  
  
  MatrixC w = warp.ComputeW(newPnts,orgPnts);
  //cerr << "w=" << w << "\n";
  for(Array1dIter2C<Point2dC,Point2dC> it(orgPnts,newPnts);it;it++) {
    Point2dC at;
    warp.ComputeWUd(orgPnts,it.Data2(),w,at);
    //cerr << it.Data1() << " " << it.Data2() << " " << at << "\n";
    if(at.EuclidDistance(it.Data1()) > 8)
      return __LINE__;
  }
  return 0;
}

int testGaussConvolve() {
  GaussConvolve2dC<ByteRGBValueC,ByteRGBValueC,RealT,RealRGBValueC> gc(3);
  ImageC<ByteRGBValueC> img(10,10);
  img.Fill(ByteRGBValueC(0,0,0));
  ImageC<ByteRGBValueC> res = gc.Apply(img);
  
  return 0;
}

int testDCT() {
  ImageC<RealT> img(16,16);
  //img.Fill(0);
  for(Array2dIterC<RealT> it(img);it;it++)
   *it = Random1();
  
  img[3][3] = 1;
  img[4][3] = 1;
  img[4][4] = 1;
  img[3][4] = 1;
  ImageC<RealT> res;
  DCT(img,res);
  //cerr << "Res=" << res << "\n";
  
  ImageC<RealT> rimg;
  IDCT(res,rimg);
  
  //cerr << "rImg=" << rimg << " Err=" << (rimg - img).SumOfSqr() << "\n";

  if((rimg - img).SumOfSqr() > 0.000001)
    return __LINE__;
  
  ChanDCTC chandct(img.Rows());
  ImageC<RealT> cimg = chandct.DCT(img);
  cerr << "ChanRes=" << cimg << "\n";
  //cerr << "Error=" << (cimg - res).SumOfSqr()  << "\n";
  if((cimg - res).SumOfSqr() > 0.000001)
    return __LINE__;
  
  for(int i = 0;i < 50000;i++)
    cimg = chandct.DCT(img);
  
#if 1
  VecRadDCTC vrdct(img.Rows(),6);
  ImageC<RealT> cimg2;
  for(int i = 0;i < 50000;i++)
    cimg2 = vrdct.DCT(img);
  cerr << "VecRadRes=" << cimg2 << "\n";
#endif
  
  return 0;
}

int testImageExtend() {
  
  ImageC<IntT> img(3,3);
  int i = 0;
  for(Array2dIterC<IntT> it(img);it;it++,i++)
    *it = i;
  
  ImageC<IntT> result;
  ExtendImageCopy(img,2,result);
  if(result[result.Frame().Origin()] != img[img.Frame().Origin()]) return __LINE__;
  if(result[result.Frame().End()] != img[img.Frame().End()]) return __LINE__;
  
  ExtendImageFill(img,2,result,-1);
  if(result[result.Frame().Origin()] != -1) return __LINE__;
  if(result[result.Frame().End()] != -1) return __LINE__;
  
  ExtendImageMirror(img,2,result);
  if(result[result.Frame().Origin()] != img[img.Frame().End()]) return __LINE__;
  if(result[result.Frame().End()] != img[img.Frame().Origin()]) return __LINE__;
  
  return 0;
}

int testImagePyramid() {
  cerr << "testImagePyramid() \n";
  ImageC<RealT> img(10,10);
  RealT v = 1;
  for(Array2dIterC<RealT> it(img);it;it++)
    *it = v++;
  
  ImagePyramidC<RealT> pyramid(img,3,true);
  if(pyramid.Images().Size() != 3) return __LINE__;
  RealT fscale(0.0),pscale(0.0);
  ImageC<RealT> simg;
  for(RealT scale =0.1;scale < 10;scale += 0.1) {
    pyramid.Find(scale,simg,fscale,pscale);
    if(fscale < pscale) return __LINE__;
    //cerr << "Scale=" << scale << " fscale=" <<fscale << " pscale=" << pscale << "\n";  
  }
#if 0
  cerr << "Images:\n";
  for(CollectionIterC<Tuple3C<RealT,RealT,ImageC<RealT> > > it(pyramid.Images());it;it++) {
    cerr << it->Data3() << "\n";
  }
#endif
  
  // Check termination.
  
  ImagePyramidC<RealT> pyramid2(img,-1,true);
  //cerr << "NoImages=" << pyramid2.Images().Size() << "\n";
  if(pyramid2.Images().Size() != 3) return __LINE__;
  
  ImagePyramidC<RealT> pyramid3(img,-1,true,true);
  //cerr << "NoImages=" << pyramid3.Images().Size() << "\n";
  if(pyramid3.Images().Size() != 3) return __LINE__;
  
  
  return 0;
}

int testImagePyramidRGB() {
  //cerr << "testImagePyramidRGB, Called \n";
  ImageC<ByteRGBValueC> img(100,100);
  ByteT v = 1;
  for(Array2dIterC<ByteRGBValueC> it(img);it;it++,v++) {
    *it = ByteRGBValueC(v,v % 27,v % 91);
  }
  
  ImagePyramidC<ByteRGBValueC,RealRGBValueC> pyramid(img,3,true);
  if(pyramid.Images().Size() != 3) return __LINE__;
  RealT fscale(0.0),pscale(0.0);
  ImageC<ByteRGBValueC> simg;
  for(RealT scale =0.1;scale < 5;scale += 1) {
    pyramid.Find(scale,simg,fscale,pscale);
    StringC sizeStr(scale);
    //RavlN::Save(StringC("@X:") + sizeStr,simg);
    if(fscale < pscale) return __LINE__;
  }
  return 0;
}
