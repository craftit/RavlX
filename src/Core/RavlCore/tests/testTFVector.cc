// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/testTFVector.cc"
//! docentry="Ravl.API.Core.Tuples"
//! userlevel=Develop
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/TFVector.hh"
#include "Ravl/Pair.hh"
#include "Ravl/FIndex.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int testTFVector();
int testPair();

namespace RavlN {
  template
  class TFVectorC<ByteT, 1>;

  template
  class TFVectorC<IntT, 2>;

  template
  class TFVectorC<RealT, 3>;

  template
  class PairC<IntT>;

  template
  class FIndexC<3>;
}

int main(int nargs,char **argv) {
  int ln;
  if((ln = testTFVector()) != 0) {
    std::cerr << "Error line :" << ln << "\n";
    return 1;
  }
  if((ln = testPair()) != 0) {
    std::cerr << "Error line :" << ln << "\n";
    return 1;
  }
  std::cerr <<"Test passed ok. \n";
  return 0;
}

int testPair() {
  PairC<IntT> apair(1,2);
  if(apair[0] != 1) return __LINE__;
  if(apair[1] != 2) return __LINE__;
  if(apair.A() != 1) return __LINE__;
  if(apair.B() != 2) return __LINE__;
  apair.Swap();
  if(apair.A() != 2) return __LINE__;
  if(apair.B() != 1) return __LINE__;
  return 0;
}

int testTFVector() {
  TFVectorC<IntT,2> test1;
  test1[0] = 1;
  test1[1] = 2;  
  TFVectorC<IntT,2> test1a;
  test1a = test1;
  if(test1a[0] != 1) return __LINE__;
  if(test1a[1] != 2) return __LINE__;
  TFVectorC<IntT,2> test2 = test1 * 2;
  if(test2[0] != 2) return __LINE__;
  if(test2[1] != 4) return __LINE__;
  test2 *= 2;
  if(test2[0] != 4) return __LINE__;
  if(test2[1] != 8) return __LINE__;
  if(test2.SumOfSqr() != 80) return __LINE__;
  TFVectorC<IntT,2> test3 = test1 * 4;
  if(test3 == test1) return __LINE__;
  test3 /= 4;
  if(test3 != test1) return __LINE__;
  
  return 0;
}

