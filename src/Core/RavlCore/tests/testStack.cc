// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
// $Id$
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Misc/testStack.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Stacks"
//! userlevel=Develop

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Ravl/Stack.hh"
#include "Ravl/StackIter.hh"

using namespace RavlN;

const int CheckVal = 0x12345678;

class Ref {
public:
  void IsOk(void) const { assert(Check == CheckVal && Counter > 0); }
  Ref(const Ref &Oth) 
    : Counter(Oth.Counter) 
  {
    Oth.IsOk();
    Counter++;  
    Check = CheckVal;
  }
  Ref(int &Ptr) : Counter(Ptr) { Counter++; Check = CheckVal; }
  ~Ref() { IsOk(); Counter--; Check = 0xf5f5f5f5; }
  Ref &operator=(const Ref &Oth) { 
    Oth.IsOk(); 
    Counter--; 
    Counter = Oth.Counter; 
    Counter++; 
    return *this;
  }
  
  int &Counter;
  int Check;
};

const int MaxCounters = 500;
const int MaxStacks = 5;
const int TestSize = 200000;

int main() {
  printf("Starting Stack Test. \n");
  int Counters[MaxCounters];
  int i,j,k;
  int MaxLen = 0;
  {
    StackC<Ref> Stk[MaxStacks];
    for(i = 0;i < MaxCounters;i++)
      Counters[i] = 0;
    for(i = 0;i < TestSize;i++) {
      switch(rand() % 7) 
	{
	case 0: // Push.
	case 4:
	  Stk[rand() % MaxStacks].Push(Ref(Counters[rand() % MaxCounters]));
	  break;
	case 1: // Pop.
	case 5:
	  j = rand() % MaxStacks;
	  if(!Stk[j].IsEmpty()) {
	    Stk[j].Top().IsOk();
	    Stk[j].Pop();
	  }
	  break;
	case 6:
	case 2: // Move stuff around.
	  j = rand() % MaxStacks;
	  k = rand() % MaxStacks;
	  if(!Stk[k].IsEmpty()) {
	    Stk[j].Push(Stk[k].Pop());
	  }
	  break;
	case 3: // Check its all ok.
	  {
	    j = rand() % MaxStacks;
	    int Len = 0;
	    for(StackIterC<Ref> It(Stk[j]);It.IsElm();It.Next()) {
	      It.Data().IsOk();
	      Len++;
	    }
	    if(Len > MaxLen)
	      MaxLen = Len;
	    if(!Stk[j].IsEmpty())
	      Stk[j].Top().IsOk();
	  }
	  break;
	}
    }
    // Check all the numbers are consistant.
    int Total = 0;
    for(j = 0;j < MaxStacks;j++) {
      for(StackIterC<Ref> It(Stk[j]);It.IsElm();It.Next())
	Total++;
    }
    for(i = 0;i < MaxCounters;i++)
      Total -= Counters[i];
    assert(Total == 0); // Do the numbers add up ?
    // Empty some stacks.
    for(j = 1;j < MaxStacks;j++)
      Stk[j].Empty();
  } // Final stack should go out of scope here.
  for(i = 0;i < MaxCounters;i++)
    assert(Counters[i] == 0);
  printf("Test completed ok. (Max depth found:%d) \n",MaxLen);
  return 0;
}

namespace RavlN {
  template
  class StackC<IntT>;

  template
  class StackIterC<IntT>;
}
