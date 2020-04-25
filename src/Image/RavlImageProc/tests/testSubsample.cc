// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/String.hh"
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/Image/WarpScale2d.hh"
#include "Ravl/Array2dIter2.hh"
//! lib=RavlImageProc

#include <stdio.h>

using namespace RavlN; 
using namespace RavlImageN;

int TestTransform1() {
  //create image
  ImageC<float> img(5, 5);
  img.Fill(0.);

  img[2][2] = 1;


  Affine2dC tr(RavlN::Vector2dC(1, 1), 0, RavlN::Vector2dC(0, 0));
  //cout << "transform:" << tr << endl;

  WarpAffineC<float, float, float, PixelMixerAssignC<float, float>, SampleNearestC<float, float> >
                       warp(ImageRectangleC(5, 5), tr);

  ImageC<float> res;
  warp.Apply(img, res);

  for(SizeT r = 0; r < 5; r++) {
    for(SizeT c = 0; c < 5; c++) {
      //printf("%3g ", res[r][c]);
      if(res[r][c] != img[r][c]) {
        cerr << "Error at " << r << "  " << c << endl;
        return 1;
      }
    }
    //printf("\n");
  }
  return 0;
}

int TestTransform1b() {
  //create image
  ImageC<float> img(5, 5);
  img.Fill(0.);

  img[2][2] = 1;


  Affine2dC tr(RavlN::Vector2dC(1, 1), 0, RavlN::Vector2dC(0, 0));
  cout << "transform:" << tr << endl;

  WarpAffineC<float, float, float, PixelMixerAssignC<float, float>, SampleBilinearC<float, float> >
                       warp(ImageRectangleC(5, 5), tr);

  ImageC<float> res;
  warp.Apply(img, res);

  for(SizeT r = 0; r < 5; r++) {
    for(SizeT c = 0; c < 5; c++) {
      printf("%3g ", res[r][c]);
      if(res[r][c] != img[r][c]) {
        cerr << "Error at " << r << "  " << c << endl;
        return 1;
      }
    }
    printf("\n");
  }
  return 0;
}

int TestTransform2() {
  //create image
  ImageC<float> img(16, 16);
  img.Fill(0.);

  for(SizeT r = 4; r < 8; r++) {
    for(SizeT c = 4; c < 8; c++) {
      img[r][c] = 1;
    }
  }
  for(SizeT r = 0; r < 16; r++) {
    for(SizeT c = 0; c < 16; c++) {
      //printf("%3g ", img[r][c]);
    }
    //printf("\n");
  }

  Affine2dC tr(RavlN::Vector2dC(4, 4), 0, RavlN::Vector2dC(0, 0));
  //cout << "transform:" << tr << endl;

  WarpAffineC<float, float, float, PixelMixerAssignC<float, float>, SampleNearestC<float, float> >
                       warp(ImageRectangleC(4, 4), tr);

  ImageC<float> res;
  warp.Apply(img, res);

  for(SizeT r = 0; r < 4; r++) {
    for(SizeT c = 0; c < 4; c++) {
      //printf("%3g ", res[r][c]);
      if(res[r][c] != 0 && r != 1 && c != 1) {
        cerr << "Error at " << r << "  " << c << endl;
        return 1;
      }
    }
    //printf("\n");
  }
  if(res[1][1] != 1) {
    cerr << "Error at 1   1\n";
    return 1;
  }
  return 0;
}

int TestTransform3() {
  //create image
  ImageC<float> img(16, 16);
  img.Fill(0.);

  for(SizeT r = 4; r < 8; r++) {
    for(SizeT c = 4; c < 8; c++) {
      img[r][c] = 1;
    }
  }
  for(SizeT r = 0; r < 16; r++) {
    for(SizeT c = 0; c < 16; c++) {
      //printf("%3g ", img[r][c]);
    }
    //printf("\n");
  }

  ImageC<float> res(4, 4);
  WarpScaleBilinear(img, Vector2dC(4,4), res);

  for(SizeT r = 0; r < 4; r++) {
    for(SizeT c = 0; c < 4; c++) {
      //printf("%3g ", res[r][c]);
      if(res[r][c] != 0 && r != 1 && c != 1) {
        cerr << "Error at " << r << "  " << c << endl;
        return 1;
      }
    }
    //printf("\n");
  }
  if(res[1][1] != 1) {
    cerr << "Error at 1   1\n";
    return 1;
  }

  return 0;
}

int TestTransform4() {
  printf("TestTransform4\n");
  //create image
  ImageC<float> img(16, 16);
  img.Fill(0.);

  for(SizeT r = 4; r < 8; r++) {
    for(SizeT c = 4; c < 8; c++) {
      img[r][c] = 1;
    }
  }
  for(SizeT r = 0; r < 16; r++) {
    for(SizeT c = 0; c < 16; c++) {
      printf("%3g ", img[r][c]);
    }
    printf("\n");
  }

  Vector2dC scale(1.1,1.1);

  ImageC<float> res1(10, 10);
  WarpScaleBilinear(img, scale, res1);

  Affine2dC tr(scale, 0, RavlN::Vector2dC(0, 0));
  cout << "transform:" << tr << endl;

  WarpAffineC<float, float, float, PixelMixerAssignC<float, float>, SampleBilinearC<float, float> >
                       warp(ImageRectangleC(10, 10), tr);
  warp.SetMidPixelCorrection(false);

  ImageC<float> res2;
  warp.Apply(img, res2);

  for(SizeT r = 0; r < 10; r++) {
    for(SizeT c = 0; c < 10; c++) {
      printf("%6g ", res1[r][c]);
    }
    printf("\n");
  }

  printf("\n");
  for(SizeT r = 0; r < 10; r++) {
    for(SizeT c = 0; c < 10; c++) {
      printf("%6g ", res2[r][c]);
    }
    printf("\n");
  }

  printf("\n");
  for(SizeT r = 0; r < 10; r++) {
    for(SizeT c = 0; c < 10; c++) {
      printf("%6g ", res1[r][c] - res2[r][c]);
      /*if(res[r][c] != 0 && r != 1 && c != 1) {
        cerr << "Error at " << r << "  " << c << endl;
        return 1;
      }*/
    }
    printf("\n");
  }
  /*if(res[1][1] != 1) {
    cerr << "Error at 1   1\n";
    return 1;
  }*/

  return 0;
}

int Test0() {
  printf("Preparing array\n");
  //create image
  const int imgSize = 1000;
  ImageC<float> img1(imgSize, imgSize);
  for(int r = 0; r < imgSize; r++)
    for(int c = 0; c < imgSize; c++) {
      img1[r][c] = c;
    }

  ImageC<float> img2(imgSize, imgSize);
  for(int r = 0; r < imgSize; r++)
    for(int c = 0; c < imgSize; c++) {
      img2[r][c] = imgSize - c;
    }

  Vector2dC scale(1.1, 2.12);
  ImageC<float> res1;
  WarpSubsample(img1, scale, res1);

  ImageC<float> res2;
  WarpSubsample(img2, scale, res2);

  cout << "Fr1:" << res1.Frame() << endl;
  cout << "Fr2:" << res2.Frame() << endl;
  float sum = res1[0][0] + res2[0][0];
  cout << "sum:" << res1[0][0] + res2[0][0] << endl;

  for(Array2dIter2C<float, float> it(res1, res2);it;it++) {
    float s1 = it.Data1() + it.Data2();
    float dif = s1 - sum;
    if(RavlN::Abs(dif) > (s1 + sum) * 1e-6) {
      cerr << "err:" << dif << "   " << s1 << "   " << it.Index() << endl;
      return __LINE__;
    }
  }

  return 0;
}

int Test1() {
  printf("Preparing array\n");
  //create image
  const int imgSize = 1000;
  ImageC<float> img1(imgSize, imgSize);
  for(int r = 0; r < imgSize; r++)
    for(int c = 0; c < imgSize; c++) {
      img1[r][c] = r;
    }

  ImageC<float> img2(imgSize, imgSize);
  for(int r = 0; r < imgSize; r++)
    for(int c = 0; c < imgSize; c++) {
      img2[r][c] = imgSize - r;
    }

  Vector2dC scale(1.1, 2.12);
  ImageC<float> res1;
  WarpSubsample(img1, scale, res1);

  ImageC<float> res2;
  WarpSubsample(img2, scale, res2);

  cout << "Fr1:" << res1.Frame() << endl;
  cout << "Fr2:" << res2.Frame() << endl;
  float sum = res1[0][0] + res2[0][0];
  cout << "sum:" << res1[0][0] + res2[0][0] << endl;

  for(Array2dIter2C<float, float> it(res1, res2);it;it++) {
    float s1 = it.Data1() + it.Data2();
    float dif = s1 - sum;
    if(RavlN::Abs(dif) > (s1 + sum) * 1e-6) {
      cerr << "err:" << dif << "   " << s1 << "   " << it.Index() << endl;
      return __LINE__;
    }
  }

  return 0;
}



int main(int argc, char **argv)
{
  if(TestTransform1() > 0) {
    cerr << "Error in WarpAffineC test 1\n";
    return 1;
  }

  if(TestTransform1b() > 0) {
    cerr << "Error in WarpAffineC test 1b\n";
    return 1;
  }

  if(TestTransform2() > 0) {
    cerr << "Error in WarpAffineC test 2\n";
    return 1;
  }

  if(TestTransform3() > 0) {
    cerr << "Error in WarpAffineC test 3\n";
    return 1;
  }

  if(TestTransform4() > 0) {
    cerr << "Error in WarpAffineC test 3\n";
    return 1;
  }

  if(Test0() > 0) {
    cerr << "Error in test 0\n";
    return 1;
  }

  if(Test1() > 0) {
    cerr << "Error in test 1\n";
    return 1;
  }

  cerr << "All ok\n";

  return 0;
}


