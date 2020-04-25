// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Misc/testBlkStack.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Stacks"
//! userlevel=Develop

#include "Ravl/BlkStack.hh"
#include "Ravl/BlkStackIter.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Stream.hh"

#include <stdlib.h>

using namespace RavlN;

const int MaxCounters = 500;
const int MaxStacks = 5;
const int TestSize = 2000000;

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int RandomTest();
int BasicTest();

int main() {
  int ln;
  if((ln =BasicTest()) != 0) {
    std::cerr << "Basic test failed at " << ln << "\n";
    return 1;
  }  
  if((ln = RandomTest()) != 0) {
    std::cerr << "Random test failed at " << ln << "\n";
    return 1;
  }
  std::cerr << "BlkStack test passed. \n";
  return 0;
}

int BasicTest() {
  BlkStackC<int> stk;
  if(!stk.IsEmpty()) return __LINE__;
  stk.Push(1);
  if(stk.IsEmpty()) return __LINE__;
  if(stk.Pop() != 1) return __LINE__;
  if(!stk.IsEmpty()) return __LINE__;
  stk.Push(1);
  stk.Push(2);
  stk.Empty();
  if(!stk.IsEmpty()) return __LINE__;
  std::cerr << "Basic test passed. \n";
  return 0;
}

int RandomTest() {
  std::cerr << "Starting random stack test. \n";
  int Counters[MaxCounters];
  int i,j,k;
  int MaxLen = 0;
  {
    BlkStackC<int> Stk[MaxStacks];
    for(i = 0;i < MaxCounters;i++)
      Counters[i] = 0;
    for(i = 0;i < TestSize;i++) {
      switch(rand() % 7) 
	{
	case 0: // Push.
	case 4:
	  {
	    int s = rand() % MaxStacks;
	    ONDEBUG(cerr << " stk[" << s << "].Push(" << i << ")\n");
	    Stk[s].Push(i);
	  }
	  break;
	case 1: // Pop.
	case 5:
	  j = rand() % MaxStacks;
	  if(!Stk[j].IsEmpty()) {
	    ONDEBUG(cerr << " stk[" << j << "].Pop()\n");
	    Stk[j].Pop();
	  }
	  break;
	case 6:
	case 2: // Move stuff around.
	  j = rand() % MaxStacks;
	  k = rand() % MaxStacks;
	  if(!Stk[k].IsEmpty()) {
	    int val = Stk[k].Pop();
	    ONDEBUG(cerr << " stk[" << k << "].Pop() = " << val <<"\n");
	    ONDEBUG(cerr << " stk[" << j << "].Push(" << val <<")\n");
	    Stk[j].Push(val);
	  }
	  break;
	case 3: // Check its all ok.
	  {
	    j = rand() % MaxStacks;
	    int Len = 0;
	    for(BlkStackIterC<int> It(Stk[j]);It.IsElm();It.Next())
	      Len++;
	    if(Len > MaxLen)
	      MaxLen = Len;
	  }
	  break;
	}
    }
    // Empty some stacks.
    for(j = 1;j < MaxStacks;j++)
      Stk[j].Empty();
  } // Final stack should go out of scope here.
  for(i = 0;i < MaxCounters;i++)
    if(Counters[i] != 0) return __LINE__;
  std::cerr << "Test completed ok. (Max depth found:" << MaxLen << ") \n";
  return 0;
}

namespace RavlN {
  template
  class BlkStackC<int>;

  template
  class BlkStackIterC<int>;
}
