// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Buffer/testSlice.cc"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Slice1d.hh"
#include "Ravl/Buffer.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int testBasic();
int testRange();

namespace RavlN {
  template
  class Slice1dC<UIntT>;

  template
  class Slice1dIterC<UIntT>;

  template
  class Slice1dIter2C<UIntT, RealT>;

  template
  class Slice1dIter3C<UIntT, RealT, ByteT>;
}

int main() {
  int ln;
  if((ln = testBasic()) != 0) {
    std::cerr << "Test failed at line << " << ln << "\n";
    return 1;
  }
  if((ln = testRange()) != 0) {
    std::cerr << "Test failed at line << " << ln << "\n";
    return 1;
  }
  return 0;
}

int testBasic() {
  BufferC<UIntT> dataBuff(100);
  Slice1dC<UIntT> s1(10);
  Slice1dC<UIntT> s2(dataBuff,10,0,10);
  for(int i = 0;i < 10;i++)
    s1[i] = i;
  for(Slice1dIter2C<UIntT,UIntT> it(s2,s1);it;it++)
    it.Data1() = it.Data2();
  for(int i = 0;i < 10;i++) {
    if(s2[i] != (UIntT) i) {
      std::cerr << "Slice test failed. \n";
      return __LINE__;
    }
  }
  return 0;
}

int testRange() {
  IndexRangeC rng(3,5);
  Slice1dC<int> slice(rng);
  int i = 0;
  for(Slice1dIterC<int> it(slice);it;it++)
    *it = i++;
  if(slice[3] != 0) return __LINE__;
  if(slice[4] != 1) return __LINE__;
  if(slice[5] != 2) return __LINE__;
  
  return 0;
}

