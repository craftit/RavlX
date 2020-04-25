// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tracking/testTracking.cc"

#include "Ravl/config.h"
#include "Ravl/Image/MatchNormalisedCorrelation.hh"
#include "Ravl/Image/GaussConvolve2d.hh"
#include "Ravl/Random.hh"

using namespace RavlImageN;

#if RAVL_COMPILER_MIPSPRO
#include "Ravl/Image/Edgel.hh"
#include "Ravl/SArray1dIter.hh"
static SArray1dC<EdgelC>     sgi_bug_fix1 ;
static SArray1dIterC<EdgelC> sgi_bug_fix2 ; 

namespace {
  void sgi_bug_fix_func (void) 
  {
    Array2dC<double> sgi_bug_fix3 ; 
    IndexRange2dC    sgi_bug_fix4 ;
    DrawFrame ( sgi_bug_fix3, 0.0, sgi_bug_fix4 ) ;
  };
};
#endif 


int testNormalisedCorrelation();

int main(int nargs,char **argv) {
  int ln;
  if((ln = testNormalisedCorrelation()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  std::cout << "Test passed ok. \n";
  return 0;
}

int testNormalisedCorrelation() {
  std::cerr << "testNormalisedCorrelation(), Called. \n";
  ImageC<ByteT> img(200,200);
  for(Array2dIterC<ByteT> it(img);it;it++)
    *it = (RandomInt() % 200);
  
  GaussConvolve2dC<ByteT> filt(11);
  img = filt.Apply(img);
  
  IndexRange2dC targFrame(Index2dC(50,50),8);
  ImageC<ByteT> targ(ImageC<ByteT>(img,targFrame).Copy());
  targ.ShiftIndexes1(50);
  targ.ShiftIndexes2(50);
  //cerr << "Targ=" << targ.Frame() << "\n";
  
  MatchNormalisedCorrelationC match(img);
  
#if 1
  // Add some noise...
  for(Array2dIterC<ByteT> it(targ);it;it++)
    *it += (IntT) (RandomInt() % 5) - 2;
#endif
  
  RealT score;
  Index2dC at;
  match.Search(targ,img.Frame(),score,at);
  std::cerr << "Targ1, At=" << at << " Score=" << score << "\n";
  if(score < 0.8)
    return __LINE__;
  if((at - Index2dC(50,50)).SumOfSqr() > 1)
    return __LINE__;
  
  ImageC<ByteT> targ2(8,8);
  for(Array2dIterC<ByteT> it(targ2);it;it++)
    *it = (RandomInt() % 200);
  match.Search(targ2,img.Frame(),score,at);
  std::cerr << "Targ2, At=" << at << " Score=" << score << "\n";
  if(score > 0.7)
    return __LINE__;
  
  return 0;
}
