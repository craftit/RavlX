// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/testImageTools.cc"
//! docentry="Ravl.API.Images.Misc"
//! userlevel=Develop
//! author="Charles Galambos"

#include "Ravl/Image/SubSample.hh"
#include "Ravl/Image/PyramidScan.hh"
#include "Ravl/Image/Rectangle2dIter.hh"
#include "Ravl/Image/SummedAreaTable.hh"
#include "Ravl/Image/SummedAreaTable2.hh"
#include "Ravl/Image/PeakDetector.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;
using namespace RavlImageN;

int testRectangle2dIter();
int testSummedAreaTable();
int testSummedAreaTable2();
int testPyramidScan();
int testPeakDetection();
int testSubPixelPeakDetection();
int testSubSample();

int main() {
  int ln;
  if((ln = testRectangle2dIter()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";
    return 1;
  }
  if((ln = testSummedAreaTable()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";
    return 1;
  }
  if((ln = testSummedAreaTable2()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";
    return 1;
  }
  if((ln = testPyramidScan()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";
    return 1;
  }
  if((ln = testPeakDetection()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";
    return 1;
  }
  if((ln = testSubPixelPeakDetection()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";
    return 1;
  }
  if((ln = testSubSample()) != 0) {
    cerr << "Error on line numner '" << ln << "'\n";    
    return 1;
  }
  cerr << "Test passed. \n";
  return 0;
}

int testRectangle2dIter() {
  cerr << "testRectangle2dIter(), Called \n";
  ImageRectangleC rect1(0,6,0,5);
  ImageRectangleC rect2(5,5);
  ImageC<int> data(rect1);
  data.Fill(1);
  cerr << "Area=" << rect2.Area() << "\n";
  int rects = 0;
  for(Rectangle2dIterC it(rect1,rect2);it;it++) {
    if(it.Window().Area() != rect2.Area()) return __LINE__;
    int x = 0;
    for(Array2dIterC<int> itx(data,it.Window());itx;itx++)
      x += *itx;
    //cerr << "x=" << x << "\n";
    if((UIntT) x != it.Window().Area()) return __LINE__;
    rects++;
  }
  if(rects != 6)
    return __LINE__;
  return 0;
}

#if RAVL_HAVE_TEMPLATEINSTANTIATE
template SummedAreaTableC<UIntT>;
#endif

int testSummedAreaTable() {
  cerr << "testSummedAreaTable(), Called \n";
  Array2dC<IntT> img(5,5);
  img.Fill(1);
  SummedAreaTableC<UIntT> tab(img);
  cerr << "Table=" << tab << "\n";
  if(tab.Sum(img.Frame()) != 25) return __LINE__;
  IndexRange2dC srect(img.Frame());
  srect = srect.Erode();
  if(tab.Sum(srect) != 9) return __LINE__;
  
  // Build a more interesting image.
  IntT sum = 1;
  for(Array2dIterC<IntT> it(img);it;it++)
    *it = sum++;
  tab.BuildTable(img);
  //cerr << img << "\n";
  //cerr << tab << "\n";
  IndexRange2dC rec2(0,1,0,1);
  //cerr <<"Sum=" << tab.Sum(rec2) << "\n";
  if(tab.Sum(rec2) != 16) return __LINE__;
  
  IndexRange2dC rec3(0,1,1,2);
  //cerr <<"Sum=" << tab.Sum(rec3) << "\n";
  if(tab.Sum(rec3) != 20) return __LINE__;
  
  IndexRange2dC rec4(1,2,1,2);
  //cerr <<"Sum=" << tab.Sum(rec4) << "\n";
  if(tab.Sum(rec4) != 40) return __LINE__;
  
  
  return 0;
}

int testSummedAreaTable2() {
  cerr << "testSummedAreaTable2(), Called \n";
  Array2dC<IntT> img(5,5);
  img.Fill(2);
  SummedAreaTable2C<UIntT> tab(img);
  cerr << "Table=" << tab << "\n";
  cerr << "Sum=" << tab.Sum(img.Frame()) << "\n";
  if(tab.Sum(img.Frame())[0] != 50) return __LINE__;
  if(tab.Sum(img.Frame())[1] != 100) return __LINE__;
  IndexRange2dC srect(img.Frame());
  srect = srect.Erode();
  if(tab.Sum(srect)[0] != 18) return __LINE__;
  
  // Build a more interesting image.
  IntT sum = 1;
  for(Array2dIterC<IntT> it(img);it;it++)
    *it = sum++;
  tab.BuildTable(img);
  //cerr << img << "\n";
  //cerr << tab << "\n";
  IndexRange2dC rec2(0,1,0,1);
  //cerr <<"Sum=" << tab.Sum(rec2) << "\n";
  if(tab.Sum(rec2)[0] != 16) return __LINE__;
  
  IndexRange2dC rec3(0,1,1,2);
  //cerr <<"Sum=" << tab.Sum(rec3) << "\n";
  if(tab.Sum(rec3)[0] != 20) return __LINE__;
  
  IndexRange2dC rec4(1,2,1,2);
  //cerr <<"Sum=" << tab.Sum(rec4) << "\n";
  if(tab.Sum(rec4)[0] != 40) return __LINE__;
  
  return 0;
}


int testPyramidScan() {
  cerr << "testPyramidScan(), Called \n";
  IndexRange2dC rect(100,200);
  int i = 0;
  int areaSum = 0;
  for(PyramidScanC it(rect,Index2dC(100,100),50,0.5,0.5);it;it++) {
    i++;
    areaSum += (*it).Area();
    //cerr << "Rect=" << *it << "\n";
  }
  cerr << "I=" << i << " Area=" << areaSum << "\n";
  if(i != 24) return __LINE__;
  if(areaSum != 82500) return __LINE__;
  return 0;
}

int testPeakDetection() {
  cerr << "testPeakDetection(), Called \n";
  ImageC<IntT> img(10,10);
  img.Fill(0);
  Index2dC at(5,5);

  // Check 3x3
  if(PeakDetect3(img,at)) return __LINE__;
  img[at] = 1;
  if(!PeakDetect3(img,at)) return __LINE__;
  img[at + Index2dC(1,0)] = 1;
  if(PeakDetect3(img,at)) return __LINE__;  
  img[at + Index2dC(1,0)] = 0;  
  img[at + Index2dC(0,1)] = 1;
  if(PeakDetect3(img,at)) return __LINE__;  
  img[at + Index2dC(0,1)] = 0;
  img[at + Index2dC(-1,0)] = 1;  
  if(PeakDetect3(img,at)) return __LINE__;  
  img[at + Index2dC(-1,0)] = 0;  
  img[at + Index2dC(0,-1)] = 1;
  if(PeakDetect3(img,at)) return __LINE__;  
  img[at + Index2dC(0,-1)] = 0;
  img[at] = 0;
  
  // Check 5x5
  if(PeakDetect5(img,at)) return __LINE__;
  img[at] = 1;
  if(!PeakDetect5(img,at)) return __LINE__;
  img[at + Index2dC(1,0)] = 1;
  if(PeakDetect5(img,at)) return __LINE__;  
  img[at + Index2dC(1,0)] = 0;  
  img[at + Index2dC(0,1)] = 1;
  if(PeakDetect5(img,at)) return __LINE__;  
  img[at + Index2dC(0,1)] = 0;
  img[at + Index2dC(-1,0)] = 1;  
  if(PeakDetect5(img,at)) return __LINE__;  
  img[at + Index2dC(-1,0)] = 0;  
  img[at + Index2dC(0,-1)] = 1;
  if(PeakDetect5(img,at)) return __LINE__;  
  img[at + Index2dC(0,-1)] = 0;
  img[at] = 0;
  
  // Check 7x7
  if(PeakDetect7(img,at)) return __LINE__;
  img[at] = 1;
  if(!PeakDetect7(img,at)) return __LINE__;
  img[at + Index2dC(1,0)] = 1;
  if(PeakDetect7(img,at)) return __LINE__;  
  img[at + Index2dC(1,0)] = 0;  
  img[at + Index2dC(0,1)] = 1;
  if(PeakDetect7(img,at)) return __LINE__;  
  img[at + Index2dC(0,1)] = 0;
  img[at + Index2dC(-1,0)] = 1;  
  if(PeakDetect7(img,at)) return __LINE__;  
  img[at + Index2dC(-1,0)] = 0;  
  img[at + Index2dC(0,-1)] = 1;
  if(PeakDetect7(img,at)) return __LINE__;  
  
  return 0;
}


int testSubPixelPeakDetection() {
  ImageC<RealT> img(3,3);
  img.Fill(0);
  img[1][1] = 1;
  Point2dC at = LocatePeakSubPixel(img,Index2dC(1,1));
  if(Abs(at[0] - 1) > 0.000001) return __LINE__;
  if(Abs(at[1] - 1) > 0.000001) return __LINE__;
  img[1][0] = 0.5;
  img[2][1] = 0.5;
  
  at = LocatePeakSubPixel(img,Index2dC(1,1));
  //cerr << "At=" << at << "\n";
  if(Abs(at[0] - 0.9) > 0.000001) return __LINE__;
  if(Abs(at[1] - 1.1) > 0.000001) return __LINE__;
  return 0;
}


int testSubSample() {
  for(int i = 3;i < 10;i++) {
    ImageC<ByteT> img(i,i);
    cerr << "i = " << i << "\n";
    ImageC<ByteT> out(2,2);
    //ByteT n = 0;
    for(Array2dIterC<ByteT> it(img);it;it++) {
      Index2dC ind = it.Index();
      *it = ind.Row() + ind.Col();
    }
    FilteredSubSample2(img,out);
    FilteredSubSample2(img);
    //cerr << "Out=" << out << "\n";
  }

  ImageC<ByteT> in(6,8);
  in.Fill(0);
  ImageC<ByteT> out2 = SubSample(in);
  in = in.Expand(1);
  out2 = SubSample(in);

  return 0;
}
