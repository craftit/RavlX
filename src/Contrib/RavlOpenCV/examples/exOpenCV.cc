// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/OpenCVConvert.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/IO.hh"

#include "opencv2/opencv.hpp"
#include "Ravl/OS/Date.hh"

//! lib=RavlOpenCV

using namespace RavlN;
using namespace RavlImageN;
using namespace cv;
using namespace std;

int main()
{
  // set up a RAVL  image
  ImageC<RealRGBValueC> src(100,200);
  src.Fill(RealRGBValueC(200,100,150));
  for (IndexC i=20; i<=80; ++i) src[i][i] = RealRGBValueC(0,200,250);

  // convert it to an OpenCV Mat
  IplImage* im1 = 0;
  std::cout << "RAVL 2 CV: " << RavlImage2IplImage(src, im1) << std::endl;

  Mat mat1=cvarrToMat(im1);
  Mat mat2;

  // filter it
  GaussianBlur(mat1, mat2, Size(9,9), 3);

  // convert it back to a RAVL image and display it
  IplImage im2 = mat2;
  ImageC<ByteRGBValueC> out;
  std::cout << "CV 2 RAVL: " << IplImage2RavlImage(&im2, out) << std::endl;
  RavlN::Save("@X", out);
  cvReleaseImage(&im1);
  RavlN::Sleep(10);
  return 0;
}
