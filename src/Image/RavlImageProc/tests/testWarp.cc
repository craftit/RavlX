// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/Image/WarpAffine.hh"
#include "Ravl/Image/WarpProjective.hh"
#include "Ravl/Image/WarpScale2d.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/IO.hh"
//! lib=RavlImageProc

using namespace RavlN;
using namespace RavlImageN;

int testWarpAffine();
int testWarpProjective();
int testWarpScale();


int main () {
  int ln;
  if((ln = testWarpAffine()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  if((ln = testWarpProjective()) != 0) {
    cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  return 0;
}

int testWarpAffine() {
  ImageC<RealT> im(ImageRectangleC(Index2dC(0,0), 8), 0);
  im[1][2] = 1;
  ImageC<RealT> out;
  Affine2dC M;

  //unit transformation; origin @ t.l. of pixel (default)
  M  = Affine2dC(Vector2dC(1,1),0,Vector2dC(0,0)); 
  WarpAffineC<RealT> warp(im.Frame().Expand(-1), M);
  out = warp.Apply(im);
  for (Array2dIter2C<RealT,RealT>i(out,im,out.Frame()); i; ++i) if (Abs(i.Data1()-i.Data2()) > 1.0e-12)  return __LINE__;

  //unit transformation; origin @ centre of pixel
  warp.SetMidPixelCorrection(false);
  out = warp.Apply(im);
  for (Array2dIter2C<RealT,RealT>i(out,im,out.Frame()); i; ++i) if (Abs(i.Data1()-i.Data2()) > 1.0e-12)  return __LINE__;

  // rotate by +pi/2; origin @ t.l. of pixel
  M  = Affine2dC(Vector2dC(1,1),RavlConstN::pi/2,Vector2dC(0,0)); //rotate by pi/2
  warp = WarpAffineC<RealT>(im.Frame().Expand(-1), M);
  warp.SetMidPixelCorrection(true);
  out = warp.Apply(im);
  if (Abs(out[2][-2] - 1) > 1.0e-12)  return __LINE__;

  // rotate by +pi/2; origin @ centre of pixel
  warp = WarpAffineC<RealT>(im.Frame().Expand(-1), M);
  warp.SetMidPixelCorrection(false);
  out = warp.Apply(im);
  if (Abs(out[2][-1] - 1) > 1.0e-12)  return __LINE__;

  // 4 consecutive rotations by pi/2 should end up back where we started; origin @ t.l. of pixel
  warp = WarpAffineC<RealT>(im.Frame().Expand(-1), M);
  out = im.Copy();
  for (UIntT i=1; i<=4; ++i)  out = warp.Apply(out);
  if (Abs(out[1][2] - 1) > 1.0e-12)  return __LINE__;

  return 0;
}


int testWarpProjective() {
  ImageC<RealT> im(ImageRectangleC(Index2dC(0,0), 4), 0);
  RealT x = 0;
  for (Array2dIterC<RealT>i(im); i; ++i, ++x) *i = x;
  ImageRectangleC checkRange(im.Frame());
  checkRange.BRow() -= 1;
  checkRange.RCol() -= 1;

  ImageC<RealT> out;
  Projection2dC M;

  //unit transformation; origin @ t.l. of pixel (default)
  M  = Projection2dC(Affine2dC(Vector2dC(1,1),0,Vector2dC(0,0))); 
  WarpProjectiveC<RealT> warp(im.Frame(), M);
  out = warp.Apply(im);
  for (Array2dIter2C<RealT,RealT>i(out,im,checkRange); i; ++i)
    if (Abs(i.Data1()-i.Data2()) > 1.0e-12)  return __LINE__;

  //unit transformation; origin @ centre of pixel
  warp.SetMidPixelCorrection(false);
  out = warp.Apply(im);
  for (Array2dIter2C<RealT,RealT>i(out,im,checkRange); i; ++i) 
    if (Abs(i.Data1()-i.Data2()) > 1.0e-12)  return __LINE__;

  // ***Ideally this test should be the same as for affine warp.***  
  // ***But currently there is an interface inconsistency in direction of rotation.***
  // rotate by -pi/2; origin @ t.l. of pixel
  M  = Projection2dC(Affine2dC(Vector2dC(1,1),-RavlConstN::pi/2,Vector2dC(0,0)),10,100);
  warp = WarpProjectiveC<RealT>(im.Frame().Expand(-1), M);
  out = warp.Apply(im);
  //cout <<im<<endl<<out<<endl; 
  if (Abs(out[2][-2] - 51) > 1.0e-12)  return __LINE__;

  // rotate by -pi/2; origin @ centre of pixel
  warp = WarpProjectiveC<RealT>(im.Frame().Expand(-1), M);
  warp.SetMidPixelCorrection(false);
  out = warp.Apply(im);
  //cout <<out<<endl; 
  if (Abs(out[2][-1] - 51) > 1.0e-12)  return __LINE__;

  // 4 consecutive rotations by pi/2 should end up back where we started; origin @ t.l. of pixel
  warp = WarpProjectiveC<RealT>(im.Frame().Expand(-1), M);
  out = im.Copy();
  for (UIntT i=1; i<=4; ++i)  out = warp.Apply(out);
  if (Abs(out[1][2] - 51) > 1.0e-12)  return __LINE__;

  // 2 consecutive rotations by pi/4 should preserve values if image is const gradient, & origin is centre of pixel
  M  = Projection2dC(Affine2dC(Vector2dC(1,1),-RavlConstN::pi/4,Vector2dC(0,0)),10,100);
  warp = WarpProjectiveC<RealT>(im.Frame().Expand(-1), M);
  warp.SetMidPixelCorrection(false);
  out = im.Copy();
  for (UIntT i=1; i<=2; ++i)  out = warp.Apply(out);
  if (Abs(out[2][0] - 42) > 1.0e-12)  return __LINE__;

  // Test what happens if no o/p frame is specified
  M  = Projection2dC(Affine2dC(Vector2dC(1,1),0,Vector2dC(1.3,4.4)),10,100);
  warp = WarpProjectiveC<RealT>(M);
  warp.SetMidPixelCorrection(false);
  out = warp.Apply(im.Copy());
  if (out.Frame().IsEmpty())  return __LINE__;

  return 0;
}
