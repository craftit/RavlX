// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/OpenCVConvert.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/RealRGBValue.hh"
//! lib=RavlOpenCV


using namespace RavlN;
using namespace RavlImageN;

int TestOpenCVConv () {
  ImageC<ByteT>         r1;  
  ImageC<RealT>         r2;
  ImageC<ByteRGBValueC> r3;
  ImageC<RealRGBValueC> r4;
  IplImage* ocvim = 0;

  const UIntT nRows=8, nCols=6;
  const Index2dC p(3,5);  // unique pixel in i/p image
  #define Q 1,2
  const Index2dC q(Q);  // unique pixel in o/p image


  // ByteT source
  ImageC<ByteT>         im1(nRows,nCols);  
  im1.ShiftArray(p-q);
  im1.Fill(13);
  im1[p]=4;
  if (!RavlImage2IplImage(im1, ocvim))  return __LINE__;
  if (cvGet2D(ocvim,Q).val[0] != 4)  return __LINE__; 
  
  if (!IplImage2RavlImage(ocvim, r1))  return __LINE__;
  if (r1[q] != 4)  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r2))  return __LINE__;
  if (r2[q] != 4.0)  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r3))  return __LINE__;
  if (r3[q] != ByteRGBValueC(4,4,4))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r4))  return __LINE__;
  if (r4[q] != RealRGBValueC(4.0,4.0,4.0))  return __LINE__;
  cvReleaseImage(&ocvim);


  // RealT source
  ImageC<RealT>         im2(nRows,nCols);
  im2.ShiftArray(p-q);
  im2.Fill(3.14159);
  im2[p]=4.3;
  if (!RavlImage2IplImage(im2, ocvim))  return __LINE__;
  if (cvGet2D(ocvim,Q).val[0] != 4.3)  return __LINE__; 

  if (!IplImage2RavlImage(ocvim, r1))  return __LINE__;
  if (r1[q] != 4)  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r2))  return __LINE__;
  if (r2[q] != 4.3)  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r3))  return __LINE__;
  if (r3[q] != ByteRGBValueC(4,4,4))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r4))  return __LINE__;
  if (r4[q] != RealRGBValueC(4.3,4.3,4.3))  return __LINE__; // value is truncated for some reason
  cvReleaseImage(&ocvim);


  // ByteRGBValueC source
  ImageC<ByteRGBValueC> im3(nRows,nCols);
  im3.ShiftArray(p-q);
  im3.Fill(ByteRGBValueC(4,15,126));
  im3[p]=ByteRGBValueC(23,40,2);
  for (IndexC i=im3.TRow(); i<=im3.BRow(); ++i) im3[i][im3.LCol()].Red()=i;
  if (!RavlImage2IplImage(im3, ocvim))  return __LINE__;
  if (cvGet2D(ocvim,Q).val[0] != 2)  return __LINE__; 

  if (!IplImage2RavlImage(ocvim, r1))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r2))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r3))  return __LINE__;
  if (r3[q] != ByteRGBValueC(23,40,2))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r4))  return __LINE__;
  if (r4[q] != RealRGBValueC(23,40,2))  return __LINE__;
  cvReleaseImage(&ocvim);


  // RealRGBValueC source
  ImageC<RealRGBValueC> im4(nRows,nCols);
  im4.ShiftArray(p-q);
  im4.Fill(RealRGBValueC(6.3,46,0.02));
  im4[p]=RealRGBValueC(1.3,13.7,0.6);
  if (!RavlImage2IplImage(im4, ocvim))  return __LINE__;

  if (!IplImage2RavlImage(ocvim, r1))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r2))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r3))  return __LINE__;
  if (r3[q] != ByteRGBValueC(1,13,0))  return __LINE__;
  if (!IplImage2RavlImage(ocvim, r4))  return __LINE__;
  if (r4[q] != RealRGBValueC(1.3,13.7,0.6))  return __LINE__;
  cvReleaseImage(&ocvim);

  return 0;
}

int main()
{

  std::cerr << "Testing OpenCV image conversion \n";
  int lineno;
  if((lineno = TestOpenCVConv()) != 0) {
    std::cerr << "Image conversion test failed : " << lineno << "\n";
     return 1;
  }
  std::cerr << "Test passed. \n";
  return 0;
}


  /*  
  std::cout << src->nSize << std::endl; // size of this struct
  std::cout << src->nChannels << std::endl; // 3 for colour
  std::cout << src->depth << std::endl; // bits per channel - 8 for byte
  std::cout << src->dataOrder << std::endl; // 0 for interleaved
  std::cout << src->origin << std::endl; // 0 for top left corner; 1 for bottom left
  std::cout << src->width << std::endl;
  std::cout << src->height << std::endl;
  std::cout << src->imageSize << std::endl; // # of bytes (I think)
  std::cout << src->widthStep << std::endl; // row length in bytes (I think)
  */

