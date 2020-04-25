// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Buffer/testBuffer3d.cc"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Stream.hh"
#include "Ravl/Buffer3d.hh"
#include "Ravl/SBfAcc3d.hh"
#include "Ravl/RBfAcc3d.hh"
#include "Ravl/BfAcc3Iter.hh"
#include "Ravl/BfAcc3Iter2.hh"
#include "Ravl/BfAcc3Iter3.hh"

using namespace RavlN;

int TestRangeBuffer();

int main()
{
  int ln;
  if((ln = TestRangeBuffer()) != 0) {
    std::cerr << "Buffer test failed on line :" << ln << "\n";
    return 1;
  }
  std::cerr << "Test passed. \n";
  return 0;
}

int TestRangeBuffer() {

#if 0
  IndexRangeC r1(1,3);
  IndexRangeC r2(2,5);
  IndexRangeC r3(3,7);
#else
  IndexRangeC r1(1,2);
  IndexRangeC r2(2,3);
  IndexRangeC r3(3,4);
#endif
  Buffer3dC<int> bf (r1.Size(),r2.Size(),r3.Size());
  if(bf.Size1() != (UIntT) r1.Size()) return __LINE__;
  if(bf.Size2() != (UIntT) r2.Size()) return __LINE__;
  if(bf.Size3() != (UIntT) r3.Size()) return __LINE__;
  
  //cerr << "Buffer at :" << ((void *) bf.Data().ReferenceElm()) << "\n";
  
  RangeBufferAccess3dC<int> rba(r2,r3);
  rba.Attach(bf,r1);
  
  if(r1 != rba.Range1()) {
    std::cerr<< "Range 1 incorrect. " << rba.Range1() <<" should be " << r1 << "\n";
    return __LINE__;
  }
  if(r2 != rba.Range2()) {
    std::cerr<< "Range 2 incorrect. " << rba.Range2() <<" should be " << r2 << "\n";
    return __LINE__;
  }

  if(r3 != rba.Range3()) {
    std::cerr<< "Range 2 incorrect. " << rba.Range2() <<" should be " << r2 << "\n";
    return __LINE__;
  }
  {    
    std::cerr << "AccBuffer @ " << bf.DataIndex().ReferenceElm() << "\n";
    std::cerr << "DataBuffer @ " << bf.Data().ReferenceElm() << "\n";
    const SizeT d3Size = rba.Range3().Size();
    const SizeT d2Size = rba.Range2().Size();
    BufferAccessC<int> *acc2 = bf.DataIndex().ReferenceElm() - rba.Range2().Min().V();
    int *atData = bf.Data().ReferenceElm() - rba.Range3().Min().V();
    
    for(BufferAccessIterC<BufferAccessC<BufferAccessC<int> > > it(rba,rba.Range1());it;it++,acc2 += d2Size) {
      *it = acc2 ;
      
      std::cerr << "Acc:" << ((void *) acc2) << " (" << ((void *) &acc2[rba.Range2().Min().V()]) << ") " << ((void *) it->ReferenceElm()) << "\n";
      for(BufferAccessIterC<BufferAccessC<int> > it2(*it,rba.Range2());it2;it2++,atData += d3Size) {
	*it2 = atData;
	cerr << "Data:" << ((void *) atData) << "\n";
      }
    }
  }
  
  if(&(rba[rba.Range1().Min()].ReferenceElm()[rba.Range2().Min().V()]) != bf.DataIndex().ReferenceElm()) {
    std::cerr << "Buffer index setup incorrect. Min @" << ((void *) &(rba[rba.Range1().Min()].ReferenceElm()[rba.Range2().Min().V()])) << "\n";
    std::cerr << "Ref @" << ((void *) bf.DataIndex().ReferenceElm()) << "\n";
    return __LINE__;
  }
  
  if(&(rba[rba.Range1().Min()][rba.Range2().Min()][rba.Range3().Min()]) != bf.Data().ReferenceElm()) {
    std::cerr << "Buffer setup incorrect. Min @" << ((void *)&(rba[rba.Range1().Min()][rba.Range2().Min()][rba.Range3().Min()])) << "\n";
    std::cerr << "Ref @" << ((void *) bf.Data().ReferenceElm()) << "\n";
    return __LINE__;
  }
  
  BufferAccess3dIterC<int> it(rba,r2,r3);
  if(!it.IsElm()) {
    std::cerr << "Failed to create valid iterator. \n";
    return __LINE__;
  }
  if(&(*it) != &(rba[rba.Range1().Min()][rba.Range2().Min()][rba.Range3().Min()])) {
    std::cerr << "Inital iterator position incorrect. " << ((void *) &(*it) ) << " should be " << ((void *) &(rba[rba.Range1().Min()][rba.Range2().Min()][rba.Range3().Min()])) <<  "\n";
    return __LINE__;
  }
  for(;it;it++)
    *it = 0;
  return 0;
}

namespace RavlN {
  template
  class Buffer3dBodyC<IntT>;

  template
  class Buffer3dC<IntT>;

  template
  class BufferAccess3dIterC<IntT>;

  template
  class BufferAccess3dIter2C<IntT, RealT>;

  template
  class BufferAccess3dIter3C<IntT, RealT, ByteT>;
}
