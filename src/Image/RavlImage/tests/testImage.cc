// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/testImage.cc"
//! docentry="Ravl.API.Images"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/String.hh"
#include "Ravl/Resource.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Image/RGBcYUV.hh"
#include "Ravl/Image/Deinterlace.hh"
#include "Ravl/Image/Font.hh"
#include "Ravl/Image/DrawLine.hh"
#include "Ravl/Image/DrawFrame.hh"
#include "Ravl/Image/DrawCircle.hh"
#include "Ravl/Image/DrawEllipse.hh"
#include "Ravl/Image/Reflect.hh"
#include "Ravl/Image/RGBcYUV.hh"
#include "Ravl/Image/ByteYUVValue.hh"
#include "Ravl/Image/BilinearInterpolation.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/Image/YCbCrBT601Value.hh"
#include "Ravl/Image/ByteYCbCrBT601Value.hh"
#include "Ravl/Image/UInt16YCbCrBT601Value.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/IO.hh"

using namespace RavlImageN;

int TestBasic();
int TestIO();
int TestColorCnv();
int TestInterlace();
int TestFont();
int TestDraw();
int TestBilinear();
int TestRotate();
int TestRGB2YUV();
int TestRound();
int TestYCbCrBT601();

namespace RavlImageN {
  template class ImageC<int>; // Make sure all functions are compiled.
}

int main()
{
#ifndef TEST_ROOT
 #ifndef PROJECT_OUT
  #define TEST_ROOT ""
 #else
  #define TEST_ROOT PROJECT_OUT
 #endif
#endif

  SetResourceRoot(TEST_ROOT);

  std::cerr << "Testing basic image ops. \n";
  int lineno;
#if 1
  if((lineno = TestBasic()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestIO()) != 0) {
    std::cerr << "Image io test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestColorCnv()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestInterlace()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestFont()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestDraw()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestBilinear()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestRotate()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
#endif
  if((lineno = TestRGB2YUV()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
     return 1;
  }
  if((lineno = TestRound()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
    return 1;
  }
  if((lineno = TestYCbCrBT601()) != 0) {
    std::cerr << "Image test failed : " << lineno << "\n";
    return 1;    
  }
  std::cerr << "Test passed. \n";
  return 0;
}

////////////////////////////////
// Check iterators, and image copying

int TestBasic()
{
  std::cerr << "Starting basic tests... \n";
  // Create test image.
  
  ImageRectangleC rect(5,10,6,9);
  std::cerr << "Size1:" << rect.Range1().Size() << " Size2:" << rect.Range2().Size() << "\n";
  ImageC<int> testImg(rect);
  
  int i = 0;
  
  Array2dIterC<int> it(testImg);
  if(it.Index() != rect.Origin()) {
    std::cerr << "Origin check failed. \n";
    return __LINE__;
  }
  
  for(;it.IsElm();it.Next()) 
    it.Data() = i++;

  if(rect.Area() != (UIntT) i) {
    std::cerr << "Iterator check failed. Area mis-match. Count:" << i << " should be " << rect.Area() <<  "\n";
    return __LINE__;
  }
  if(testImg[10][9] != (i-1)) {
    std::cerr<< "Inital spot value check failed. " << testImg[10][9]  << " Expected:" << (i-1)<< "\n";
    std::cerr << testImg;
    return __LINE__;
  }
  // Make copy...
  
  ImageC<int> testInImg = testImg.Copy();

  if(testInImg[10][9] != (i-1)) {
    std::cerr<< "Spot value check 2 failed. " << testInImg[10][9]  << " Expected:" << (i-1)<< "\n";
    std::cerr << testInImg;
    return __LINE__;
  }
  if(testImg[10][9] != (i-1)) {
    std::cerr<< "Spot value check 3 failed. " << testImg[10][9]  << " Expected:" << (i-1)<< "\n";
    std::cerr << testImg;
    return __LINE__;
  }
  std::cerr <<"Copy " << testInImg<<"\n";
  // check image.
  
  if(testInImg[6][7] != testImg[6][7]) {
    std::cerr<< "Spot value check failed. \n";
    return __LINE__;
  }

  if(testInImg[5][6] != 0 || testImg[5][6] != 0) {
    std::cerr<< "Spot value check failed. \n";
    return __LINE__;
  }
  
  if(testInImg.Rectangle() != testImg.Rectangle()) {
    std::cerr<< "Rectangle size mismatch. \n";
    return __LINE__;
  }
  
  for(Array2dIter2C<int,int> it2(testInImg,testImg);it2;it2++) {
    if(it2.Data1() != it2.Data2()) {
      std::cerr<< "Image contents mismatch at " <<it2.Index() << ":" << it2.Data1() << " " << it2.Data2() << "\n";
      return __LINE__;
    }
  }
  
  testImg[10][9] = 0;
  if(testInImg[10][9] != (i-1)) {
    std::cerr<< "Spot value check 4 failed. " << testInImg[10][9]  << " Expected:" << (i-1)<< "\n";
    std::cerr << testImg;
    return __LINE__;
  }

  return 0;
}

////////////////////////////////
// Check image io opreators.

int TestIO()
{
  std::cerr << "Starting IO test... \n";
  FilenameC testFn("/tmp/testImage.zyx");
  testFn = testFn.MkTemp();
  std::cerr << "Using file '" << testFn << "'\n";
  // Create test image.
  
  ImageRectangleC rect(5,10,4,11);
  ImageC<int> testImg(rect);
  
  int i = 0;
  Array2dIterC<int> it(testImg);
  if(it.Index() != rect.Origin()) {
    std::cerr << "Origin check failed. \n";
    return __LINE__;
  }
  
  for(;it.IsElm();it.Next()) 
    it.Data() = i++;
  
  if(testImg.Rectangle().Area() != ((UIntT) i)) {
    std::cerr << "Area test failed! \n";
    return __LINE__;
  }
  
  // Write out test image.
  {
    OStreamC out(testFn);
    out << testImg;
    if(!out) {
      std::cerr << "Image write failed! \n";
      return __LINE__;
    }
  }
  
  ImageC<int> testInImg;
  
  // Read in test image.
  {
    IStreamC in(testFn); 
    if(!in) {
      std::cerr << "Image read failed! \n";
      return __LINE__;
    }
    in >> testInImg;
  }
  
  if(testInImg.Rectangle() != testImg.Rectangle()) {
    std::cerr<< "Rectangle size mismatch. \n";
    return __LINE__;
  }
  
  for(Array2dIter2C<int,int> it2(testInImg,testImg);it2.IsElm();it2.Next()) {
    if(it2.Data1() != it2.Data2()) {
      std::cerr<< "Image contents mismatch at " <<it2.Index() << ":" << it2.Data1() << " " << it2.Data2() << "\n";
      return __LINE__;
    }
  }
  
  if(testFn.Exists())
    testFn.Remove();
  
  return 0;
}

int TestColorCnv() {
  RealYUVValueC yuv(20,30,40);
  std::cerr << "yuv=" << yuv << "\n";
  RealRGBValueC rgb(yuv);
  std::cerr << "rgb=" << rgb << "\n";
  RealYUVValueC yuv2(rgb);
  std::cerr << "yuv2=" << yuv2 << "\n";
  RealT diff = (yuv - yuv2).SumOfSqr();
  if(diff > 0.002) return __LINE__;
  return 0;
}

int TestInterlace() {
  std::cerr << "TestInterlace(), Called \n";
  ImageC<IntT> img(10,10);
  img.Fill(0); 
  img[0][3] = 0; img[1][3] = 1; img[2][3] = 2; img[3][3] = 3; img[4][3] = 4;
  img[5][3] = 5; img[6][3] = 6; img[7][3] = 7; img[8][3] = 8; img[9][3] = 9;
  ImageC<IntT> res = Interlace(img);
  if(res.RowPtr(0) != img.RowPtr(0)) return __LINE__;
  if(res.RowPtr(1) != img.RowPtr(5)) return __LINE__;
  if(res.RowPtr(2) != img.RowPtr(1)) return __LINE__;
  if(res.RowPtr(3) != img.RowPtr(6)) return __LINE__;
  if(res.RowPtr(4) != img.RowPtr(2)) return __LINE__;
  if(res.RowPtr(5) != img.RowPtr(7)) return __LINE__;
  if(res.RowPtr(6) != img.RowPtr(3)) return __LINE__;
  if(res.RowPtr(7) != img.RowPtr(8)) return __LINE__;
  if(res.RowPtr(8) != img.RowPtr(4)) return __LINE__;
  if(res.RowPtr(9) != img.RowPtr(9)) return __LINE__;
  if(res[2][3] != 1 || res[3][3] != 6) return __LINE__;
  ImageC<IntT> img2 = DeInterlace(res);  
  // see if we are back to where we started
  for (Array2dIter2C<IntT,IntT> i(img,img2); i; ++i) {
    if (i.Data1() != i.Data2()) {cout<<i.Index()<<"; "<<i.Data1()<<" "<<i.Data2();return __LINE__;}
  }
  return 0;
}

int TestFont() {
  std::cerr << "TestFont(), Called. \n";
  //FontC fnt = LoadPSF1("default8x16.psf");
  FontC fnt(true);
  if(!fnt.IsValid()) return __LINE__;
  ImageC<ByteT> img(100,100);
  img.Fill(0);
  DrawText(fnt,(ByteT) 255,Index2dC(50,10),"Hello",img);
  //Save("@X",img);
  return 0;
}

int TestDraw() {
  // Do some quick tests on the draw functions.
  ImageC<ByteT> img(100,100);
  img.Fill(0);
  Index2dC from(10,10);
  Index2dC to(90,90);
  Index2dC out1(200,20);
  Index2dC out2(200,40);
  DrawLine(img,(ByteT) 255,out1,out2);
  for (Array2dIterC<ByteT>i(img); i; ++i) if (*i != 0) return __LINE__;
  DrawLine(img,(ByteT) 255,from,to);
  if(img[50][50] != 255) return __LINE__;
  if(img[from] != 255) return __LINE__;
  if(img[to] != 255) return __LINE__;
  DrawCircle(img,(ByteT) 254,Index2dC(50,50),30);
  DrawFrame(img,(ByteT) 255,5,img.Frame());
  //Save("@X",img);
  if(img[0][0] != 255) return __LINE__;
  if(img[99][99] != 255) return __LINE__;
  if(img[15][10] != 0) return __LINE__;
  Ellipse2dC ellipse(Point2dC(50,50),40,20,0);
  DrawEllipse(img,(ByteT) 255,ellipse);
  DrawFrame(img,(ByteT) 128,img.Frame());
  if(img[0][0] != 128) return __LINE__;
  if(img[99][99] != 128) return __LINE__;
  if(img[0][99] != 128) return __LINE__;
  if(img[99][0] != 128) return __LINE__;
  //Save("@X",img);
  return 0;
}

int TestBilinear() {
  ImageC<RealT> img(4,4);
  img.Fill(0);
  img[1][1] = 1.0;
  
  RealT value;
  for(int i = 0;i < 3;i++) {
    for(int j = 0;j < 3;j++) {
      BilinearInterpolation(img,Point2dC((RealT) i,(RealT) j),value);
      //cerr << "Value=" << value << "\n";
      if(Abs(img[i][j] - value) > 0.001) return __LINE__;
    }
  }
  
  return 0;
}

int TestRotate() {
  ImageC<IntT> img(3,3);
  int i = 0;
  for(Array2dIterC<IntT> it(img);it;it++)
    *it = i++;
  //cerr << "Img=" << img << "\n";
  
  ImageC<IntT> img90a = img.Rotate90(Index2dC(0,0));
  //cerr << "Img90(0,0)=" << img90a << "\n";
  
  ImageC<IntT> img90 = img.Rotate90(Index2dC(1,1));
  if(img90[Index2dC(1,1)] != img[Index2dC(1,1)])
    return __LINE__;
  //cerr << "Img90(2,2)=" << img90 << "\n";
  
  ImageC<IntT> img180 = img.Rotate180(Index2dC(1,1));
  if(img180[Index2dC(1,1)] != img[Index2dC(1,1)])
    return __LINE__;
  //cerr << "Img180=" << img180 << "\n";
  
  ImageC<IntT> img270 = img.Rotate270(Index2dC(1,1));
  if(img270[Index2dC(1,1)] != img[Index2dC(1,1)])
    return __LINE__;
  //cerr << "Img270=" << img270 << "\n";
  
  
  return 0;
}

int TestRGB2YUV() {
  
  UIntT diff = 0;
  for(IntT r = 0;r < 256;r += 4) {
    for(IntT g = 0;g < 256;g += 4) {
      for(IntT b = 0;b < 256;b += 4) {
        // Convert to YUV.
        RealRGBValueC rrgb(r,g,b);
        RealYUVValueC ryuv(rrgb);
        ryuv.LimitYUV(0,255,-128,127);
        ByteYUVValueC yuv(ryuv);
        
        ByteRGBValueC rgb;
        
        {
#if 1
          // Direct conversion.
          ByteYUV2RGB(yuv.Y(),yuv.U(),yuv.V(),rgb);
#else
          // Convert via real.
          RealYUVValueC cv_ryuv(yuv);
          RealRGBValueC cv_rrgb(ryuv);
          cv_rrgb.Limit(0,255);
          rgb = ByteRGBValueC(cv_rrgb);
#endif
        }
        
        RealRGBValueC rrgb2(rgb);
        RealYUVValueC ryuv2(rrgb2);
        ryuv2.LimitYUV(0,255,-128,127);
        ByteYUVValueC yuv2(ryuv2);
        
        IntT val = Abs((IntT) yuv2.Y() - (IntT) yuv.Y());
        diff += val;
        if(val > 1) {
          std::cerr << "\n RYUV=" << ryuv << "\n";
          std::cerr << "YUV1=" << yuv << "\n";
          std::cerr << "YUV2=" << yuv2 << "\n";
          std::cerr << " Y=" << val << " "; 
        }
        
        val = Abs((IntT) yuv2.U() - (IntT) yuv.U());
        diff += val;
        if(val > 1) std::cerr << " U=" << val << " "; 
        
        val = Abs((IntT) yuv2.V() - (IntT) yuv.V());
        if(val > 1) {
          std::cerr << "\n RYUV=" << ryuv << "\n";
          std::cerr << "YUV1=" << yuv << "\n";
          std::cerr << "YUV2=" << yuv2 << "\n";
          std::cerr << " V=" << val << "\n"; 
        }
        diff += val;
      }
    }
  }
  
  // std::cout << "DIff=" << diff << "\n";
  if(diff > 3400)
    return __LINE__;
  return 0;
}


int TestRound() {
  ImageC<ByteT> b(ImageRectangleC(1,5,1,5));
  for (Array2dIterC<ByteT>i(b); i; ++i) *i = i.Index()[0]*5+i.Index()[1];
  b[2][2] = 255;
  b[3][3] = 0;
  ImageC<RealT> r = ByteImageCT2DoubleImageCT(b);
  r += 0.3;
  r[2][2] = 1000.0;
  r[3][3] = -6.8;
  r[4][4] -= 0.6;
  ImageC<ByteT> b1 = DoubleImageCT2ByteImageCT(r);
  for (Array2dIter2C<ByteT,ByteT> i(b,b1); i; ++i)
    if (i.Data1() != i.Data2()) {
      std::cerr << "pixel pos: " << i.Index()
           << "; in: "  << (int)i.Data1() 
           << ", real: "<< r[i.Index()]
           << ", out: " << (int)i.Data2() << std::endl;
      return __LINE__;
    }
  r = ImageC<RealT>(r, r.Frame().Shrink(1));
  b1 = DoubleImageCT2ByteImageCT(r);
  for (Array2dIter2C<ByteT,ByteT> i(b,b1,b1.Frame()); i; ++i)
    if (i.Data1() != i.Data2()) { 
      std::cerr << "pixel pos: " << i.Index() 
           << "; in: "  << (int)i.Data1()
           << ", real: "<< r[i.Index()]
           << ", out: " << (int)i.Data2() << std::endl;
      return __LINE__;
    } 
  return 0;
}

int TestYCbCrBT601() {
  
  static RGBValueC<float> rgbValues[] = { RGBValueC<float>(0.9,0,0),
                                          RGBValueC<float>(0,0.9,0),
                                          RGBValueC<float>(0,0,0.9),
                                          RGBValueC<float>(0.9,0.9,0.9),
                                          RGBValueC<float>(0,0.9,0.9),
                                          RGBValueC<float>(0.9,0,0.9),
                                          RGBValueC<float>(0.9,0.9,0),
                                          RGBValueC<float>(0.5,0.5,0.5),
                                          RGBValueC<float>(0.2,0.5,0.8),
                                          RGBValueC<float>(0.8,0.5,0.2),
                                          RGBValueC<float>(0.5,0.8,0.2),
                                          RGBValueC<float>(0.2,0.8,0.5)
  };
  
  for(int i = 0;i < 9;i++) {
    const RGBValueC<float> &rgbValue1 = rgbValues[i];
    RGBValueC<float> rgbValue2;
    YCbCrBT601ValueC<float> yCbCr;
    
    // Convert Floating RGB -> yCbCr
    RGBFloat2YCbCrBT601Float(rgbValue1,yCbCr);

    // Check conversion too and from UInt16 
    UInt16YCbCrBT601ValueC yCbCrI16(yCbCr);
    RGBValueC<float> rgbValueFromI16 = yCbCrI16.FloatRGB();
    float diffA = (rgbValueFromI16 - rgbValue1).SumOfAbs();
    if(diffA > 0.0001) {
      std::cerr << "Failed to reconstruct from 16 bit value. " << rgbValue1 << " -> " << rgbValueFromI16 << " Diff=" << diffA << "\n";
      return __LINE__;
    }
    
    // Convert Floating yCbCr -> RGB
    YCbCrBT601Float2RGBFloat(yCbCr,rgbValue2);
    std::cerr << "FF RGB=" << rgbValue1 << " yCbCr=" << yCbCr << "\n";
    if((rgbValue1-rgbValue2).SumOfAbs() > 0.000001) return __LINE__;
    
    // Convert yCbCr float to byte.
    ByteYCbCrBT601ValueC ycbcr8(yCbCr);
    
    // Convert byte to float.
    YCbCrBT601ValueC<float> yCbCr2 = ycbcr8.FloatYCbCr();
    
    // Check we can go between 8 bit and float ok ?
    std::cerr << "BF yCbCr8=" << ycbcr8 << " yCbCr2=" << yCbCr2 << " Diff=" <<(yCbCr2 - yCbCr).SumOfAbs() << "\n";
    if((yCbCr2 - yCbCr).SumOfAbs() > 0.01) return __LINE__;
    
    //UInt16YCbCrBT601ValueC ycbcr16(ycbcr8);
    //RGBValueC<float> rgbValue3(ycbcr16);
    
    // Check we can convert to float rgb from byte YCbCr
    RGBValueC<float> rgbValue3(ycbcr8.FloatRGB());
    std::cerr << "FB RGB1=" << rgbValue1 << " ycbcr8=" << ycbcr8 << " RGB=" << rgbValue3<< " Diff=" << (rgbValue1-rgbValue3).SumOfAbs() << "\n";
    //" ycbcr16=" << ycbcr16 << 
    if((rgbValue1-rgbValue3).SumOfAbs() > 0.016) return __LINE__;
    
    // Test conversion to byte rgb.
    
    ByteRGBValueC byteRGB((ByteT)ClipRange(rgbValue1[0]*256.0,0.0,255.0),
                          (ByteT)ClipRange(rgbValue1[1]*256.0,0.0,255.0),
                          (ByteT)ClipRange(rgbValue1[2]*256.0,0.0,255.0));
    
    ByteYCbCrBT601ValueC byteYCbCr(byteRGB);
    yCbCrI16 = byteYCbCr;
    
    ByteRGBValueC rgb8fromYCbCr8 = byteYCbCr.ByteRGB();
    
    int diff =
      Abs((int) byteRGB[0] - (int) rgb8fromYCbCr8[0]) +
      Abs((int) byteRGB[1] - (int) rgb8fromYCbCr8[1]) +
      Abs((int) byteRGB[2] - (int) rgb8fromYCbCr8[2]);
    std::cerr << "BB byteRGB=" << byteRGB << " YCbCrI8=" << byteYCbCr << " rgb8fromYCbCr8=" << rgb8fromYCbCr8 << " diff=" << diff <<"\n";
    if(diff > 8) return __LINE__;

    ByteRGBValueC rgb8fromYCbCrI16 = yCbCrI16.ByteRGB();
    int diff2=
      Abs((int) byteRGB[0] - (int) rgb8fromYCbCrI16[0]) +
      Abs((int) byteRGB[1] - (int) rgb8fromYCbCrI16[1]) +
      Abs((int) byteRGB[2] - (int) rgb8fromYCbCrI16[2]);
    std::cerr << "BB byteRGB=" << byteRGB << " YCbCrI16=" << yCbCrI16 << "(" << (yCbCrI16/256)<< ") rgb8fromYCbCrI16=" << rgb8fromYCbCrI16 << " diff=" << diff2 <<"\n";
    if(diff2 > 8) return __LINE__;

    
  }
  return 0;
}
