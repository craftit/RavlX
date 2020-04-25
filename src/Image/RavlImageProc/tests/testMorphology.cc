// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Morphology/testMorphology.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/Dilate.hh"
#include "Ravl/Image/Erode.hh"
#include "Ravl/Stream.hh"

using namespace RavlImageN;

int testMorphology();
int testStressMorphology();

int main(int nargs,char **argv)
{
  int ln;
  if((ln = testMorphology()) != 0) {
    cerr << "Test failed at " << ln << "\n";
    return 1;
  }
  if((ln = testStressMorphology()) != 0) {
    cerr << "Test failed at " << ln << "\n";
    return 1;
  }
  
  cerr << "Test passed. \n";
  return 0;
}


int testMorphology() {
  ImageC<UIntT> kernel(IndexRangeC(-1,1),
		       IndexRangeC(-1,1));
  kernel.Fill(0);
  
  kernel[0][0] = 1;
  kernel[0][1] = 1;
  kernel[1][0] = 1;
  kernel[0][-1] = 1;
  kernel[-1][0] = 1;
  
  ImageC<UIntT> test(8,8);
  test.Fill(0);
  test[3][3] = 1;
  test[4][3] = 1;
  //cerr << test;
  
  ImageC<UIntT> res(test.Rectangle());
  res.Fill(0);
  BinaryDilate(test,kernel,res);
  if(!res[3][3]) return __LINE__;
  if(!res[4][3]) return __LINE__;
  if(!res[3][2]) return __LINE__;
  if(!res[3][4]) return __LINE__;
  if(!res[2][3]) return __LINE__;
  if(!res[4][4]) return __LINE__;
  //cerr << res;
  ImageC<UIntT> res2(test.Rectangle());
  res2.Fill(0);
  BinaryErode(res,kernel,res2);
  //cerr << res2;
  if(!res2[3][3]) return __LINE__;
  if(!res2[4][3]) return __LINE__;
  if(res2[3][2]) return __LINE__;
  if(res2[3][4]) return __LINE__;
  if(res2[2][3]) return __LINE__;
  if(res2[4][4]) return __LINE__;
  return 0;
}

int testStressMorphology() {
  
  ImageC<UIntT> kernel(IndexRangeC(-1,1),
		       IndexRangeC(-1,1));
  kernel.Fill(1);
  
  // Check nothing nasty happens on degenerate images.
  for(int i = 0;i < 5;i++) {
    for(int j = 0;j < 5;j++) {
      ImageC<UIntT> test(i,j);
      test.Fill(1);
      ImageC<UIntT> res2(test.Rectangle());
      BinaryDilate(test,kernel,res2);
    }
  }

  
  // Check nothing nasty happens on degenerate images.
  for(int i = 0;i < 5;i++) {
    for(int j = 0;j < 5;j++) {
      ImageC<UIntT> test(i,j);
      test.Fill(1);
      ImageC<UIntT> res2(test.Rectangle());
      BinaryErode(test,kernel,res2);
    }
  }
  
  return 0;
}
