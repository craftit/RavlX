// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Buffer/testBuffer2d.cc"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Buffer2d.hh"
#include "Ravl/SBfAcc2d.hh"
#include "Ravl/RBfAcc2d.hh"
#include "Ravl/SingleBuffer.hh"

#include "Ravl/Stream.hh"
#include "Ravl/IntC.hh"

using namespace RavlN;

int TestSingleBuffer();
int TestBuffer2d();

int main()
{
  int ln;
  if((ln = TestSingleBuffer()) != 0) {
    std::cerr << "Test failed at " << ln << "\n";
    return 1;
  }
  if((ln = TestBuffer2d()) != 0) {
    std::cerr << "Test failed at " << ln << "\n";
    return 1;
  }
  std::cerr << "Test passed ok. \n";
  return 0;
}

int TestBuffer2d() {
  std::cerr << "Single Buffer Size=" << sizeof(SingleBufferBodyC<RealT>) - sizeof(RealT) << "\n";
  
  BufferC<int> bf1d (6);
  if(bf1d.Size() != 6) {
    std::cerr << "Size test failed. \n";
    return 1;
  }
  
  IndexRangeC r1(1,3);
  IndexRangeC r2(2,5);
  
  Buffer2dC<int> bf (r1.Size(),r2.Size());
  if(bf.Size1() != (UIntT) r1.Size()) {
    std::cerr << "Size1 test failed. \n";
    return 1;
  }
  if(bf.Size2() != (UIntT) r2.Size()) {
    std::cerr << "Size2 test failed. \n";
    return 1;
  }
  
  //cerr << "Buffer at :" << ((void *) bf.Data().ReferenceElm()) << "\n";
  RangeBufferAccess2dC<int> rba(r2);
  rba.Attach(bf,r1);
  
  if(r1 != rba.Range1()) {
    std::cerr<< "Range 1 incorrect. " << rba.Range1() <<" should be " << r1 << "\n";
    return 1;
  }

  if(r2 != rba.Range2()) {
    std::cerr<< "Range 2 incorrect. " << rba.Range2() <<" should be " << r2 << "\n";
    return 1;
  }
  
  {
    // Build the access structure.
    const SizeT d2Size = rba.Range2().Size();
    int *at = bf.Data().ReferenceElm() - rba.Range2().Min().V();
    for(BufferAccessIterC<BufferAccessC<int> > it(rba);it;it++,at += d2Size)
      *it = at;
  }
  
  if(&(rba[rba.Range1().Min()][rba.Range2().Min()]) != bf.Data().ReferenceElm()) {
    std::cerr << "Buffer setup incorrect. Min@" << ((void *)&(rba[rba.Range1().Min()][rba.Range2().Min()])) << "\n";
    return 1;
  }
  
  BufferAccess2dIterC<int> it(rba,r2);
  if(!it.IsElm()) {
    std::cerr << "Failed to create valid iterator. \n";
    return 1;
  }
  if(&(*it) != &(rba[rba.Range1().Min()][rba.Range2().Min()])) {
    std::cerr << "Inital iterator position incorrect. " << ((void *) &(*it) ) << " should be " << ((void *) &(rba[rba.Range1().Min()][rba.Range2().Min()])) <<  "\n";
    return 1;
  }
  for(;it;it++)
    *it = 0;
  return 0;
}

class TestObjC {
public:
  TestObjC() 
  { val = 123; }
  
  int Value()
  { return val; }
protected:
  int val;
};

int TestSingleBuffer() {
  for(int i = 0;i < 100;i++) {
    SingleBufferC<IntT> buff(i+1);
    //cerr << "UMem=" << ((void *) buff.ReferenceElm()) << "\n";
  }
  for(int i = 0;i < 100;i++) {
    SingleBufferC<SizeT> buff(i+1,16);
    if((((size_t)((void *) buff.ReferenceElm())) & 0xf) != 0)
      return __LINE__;
    //cerr << "AMem=" << ((void *) buff.ReferenceElm()) << "\n";
  }
  SingleBufferC<IntC> buff2(100);
  IntC *at = buff2.ReferenceElm();
  IntC *end = &(at[buff2.Size()]);
  for(;at != end;at++)
    if(*at != 0) return __LINE__;
  
  SingleBufferC<TestObjC> buff3(100);
  TestObjC *at3 = buff3.ReferenceElm();
  TestObjC *end3 = &(at3[buff3.Size()]);
  for(;at3 != end3;at3++)
    if(at3->Value() != 123) return __LINE__;
  return 0;
}


// Check all methods compile.

namespace RavlN {
  template
  class BufferBodyC<IntT>;

  template
  class BufferC<IntT>;

  template
  class SizeBufferAccess2dC<IntT>;

  template
  class RangeBufferAccess2dC<IntT>;

  template
  class Buffer2dBodyC<IntT>;

  template
  class Buffer2dC<IntT>;
}
