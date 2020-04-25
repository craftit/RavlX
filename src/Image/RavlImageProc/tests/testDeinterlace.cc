// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/DeinterlaceStream.hh"
#include "Ravl/Image/Averaging2d.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/DP/ContainerIO.hh"
#include "Ravl/DList.hh"
#include "Ravl/IO.hh"
#include "Ravl/Moments2d2.hh"
//! lib=RavlImageProc


using namespace RavlN;
using namespace RavlImageN;

static void Inc(RealT &x)
{ ++x; }

static void Inc(ByteT &x)
{ ++x; }

static void Inc(RealRGBValueC &x)
{ ++x.Red(); ++x.Green(); ++x.Blue(); }

static void Inc(ByteRGBValueC &x)
{ ++x.Red(); ++x.Green(); ++x.Blue(); }

static RealT YValue(RealT x)
{ return x; }

static ByteT YValue(ByteT x)
{ return x; }

static RealT YValue(RealRGBValueC x)
{ return x.Y(); }

static ByteT YValue(ByteRGBValueC x)
{ return x.Y(); }

template<class PixelT> int testDeinterlace() {
  ImageC<PixelT> imgR(20,10);
  PixelT fill; SetZero(fill);
  imgR.Fill(fill);
  ImageC<PixelT> imgRSub(imgR, IndexRange2dC(7,14,3,7));
  Inc(fill);
  imgRSub.Fill(fill);
  Averaging2dC<PixelT,PixelT> filt(2,1);
  imgR = filt.Apply(imgR);
  DListC<ImageC<PixelT> > imlist;
  imlist += imgR;
  DPIPortC<ImageC<PixelT> > port(DPIContainer(imlist));
  DeinterlaceStreamC<PixelT> stream(port);
  stream.NoRescale();
  stream.FieldAlign();
  SArray1dC<Point2dC> centroid(2);
  for (IntT i(0); stream.Get(imgR); ++i){
    Moments2d2C mom;
    for (Array2dIterC<PixelT> j(imgR); j; ++j)  mom.AddPixel(j.Index(), YValue(*j));
    centroid[i] = Point2dC(mom.CentroidX(),mom.CentroidY());
  }
  std::cout << centroid << std::endl;
  if (centroid[0].EuclideanDistance(centroid[1]) > 1.0e-10)  return __LINE__;
  return 0;
}


int main() {
  int ln;
  if((ln = testDeinterlace<RealT>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  //if((ln = testDeinterlace<ByteT>()) != 0) {
  //  std::cerr << "Test failed on line " << ln << "\n";
  //  return 1;
  //}
  if((ln = testDeinterlace<RealRGBValueC>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  //if((ln = testDeinterlace<ByteRGBValueC>()) != 0) {
  //  std::cerr << "Test failed on line " << ln << "\n";
  //  return 1;
  //}
  std::cerr << "Test passed ok. \n";
  return 0;
}

