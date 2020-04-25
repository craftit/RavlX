// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Queue/testFixedQueue.cc"
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Queues"

#include "Ravl/FixedQueue.hh"
#include "Ravl/Stream.hh"
#include "Ravl/UnitTest.hh"

using namespace RavlN;

int TestQueue();
int TestQueueIter();
int TestQueueRevIter();

int main() 
{
  int ln;
  if((ln = TestQueue()) != 0) {
    std::cerr << "Queue test failed at:" << ln << "\n";
    return 1;
  }
  if((ln = TestQueueIter()) != 0) {
    std::cerr << "Queue iter test failed at:" << ln << "\n";
    return 1;
  }
  if((ln = TestQueueRevIter()) != 0) {
    std::cerr << "Queue reverse iter test failed at:" << ln << "\n";
    return 1;
  }
  
  std::cerr << "Test passed ok.\n";
  return 0;
}

int TestQueue()
{
  std::cerr << "Starting TestQueue() \n";
  {
    // Check zero size queue
    FixedQueueC<int> emptyQ;
    RAVL_TEST_FALSE(emptyQ.IsSpace());
    RAVL_TEST_TRUE(emptyQ.IsEmpty());
    RAVL_TEST_TRUE(emptyQ.Size() == 0);
    RAVL_TEST_TRUE(emptyQ.MaxSize() == 0);

    // Check we can resize ok from it.
    emptyQ.Resize(5);
    RAVL_TEST_TRUE(emptyQ.Size() == 0);
    RAVL_TEST_TRUE(emptyQ.MaxSize() == 5);
    RAVL_TEST_TRUE(emptyQ.IsSpace());
    RAVL_TEST_TRUE(emptyQ.IsEmpty());
  }

  FixedQueueC<int> q(5);
  for(int z = 0;z < 12;z++) {
    if(!q.IsEmpty()) {
      std::cerr << "IsEmpty test 1 failed. \n";
      return __LINE__;
    }
    if(!q.IsSpace()) {
      std::cerr << "IsSpace test 1 failed. \n";
      return __LINE__;
    }
    q.InsLast(1);
    if(q.IsEmpty()) {
      std::cerr << "IsEmpty test 1a failed. \n";
      return __LINE__;
    }
    if(q.Size() != 1) {
      std::cerr << "Size test 1 failed. \n";
      return __LINE__;
    }
    if(q[0] != 1) {
      std::cerr << "Indexed access failed. \n";
      return __LINE__;      
    }
    if(q.GetFirst() != 1) {
      std::cerr << "Content test 1 failed. \n";
      return __LINE__;
    }
    
    // Fill the queue.
    for(int i = 0;i < 5;i++) {
      if(!q.IsSpace()) {
	cerr << "IsSpace test 3 failed. \n";
	return __LINE__;
      }
      q.InsLast(i);
    }
    if(q.Size() != 5) {
      std::cerr << "Size test 2 failed. (Size() == " << q.Size() << ")\n";
      return __LINE__;
    }
    
    // Check it out.
    if(q.IsEmpty()) {
      std::cerr << "IsEmpty test 2 failed. \n";
      return __LINE__;
    }  
    if(q.IsSpace()) {
      std::cerr << "IsSpace test 2 failed. \n";
      return __LINE__;
    }
    if(q[0] != 0) return __LINE__;
    if(q[1] != 1) return __LINE__;
    if(q[2] != 2) return __LINE__;
    if(q[3] != 3) return __LINE__;
    if(q[4] != 4) return __LINE__;
    
    if(q.IndexBackward(0) != 4) return __LINE__;
    if(q.IndexBackward(1) != 3) return __LINE__;
    if(q.IndexBackward(2) != 2) return __LINE__;
    if(q.IndexBackward(3) != 1) return __LINE__;
    if(q.IndexBackward(4) != 0) return __LINE__;

    for(int j = 0;j < 5;j++) {
      if(q.IsEmpty()) {
	cerr << "IsEmpty test 3 failed. \n";
	return __LINE__;
      }
      if(q.GetFirst() != j) {
	cerr << "Content test 2 failed. \n";
	return __LINE__;
      }
    }
  }

  for(int j = 2;j < 20;j++) {
    FixedQueueC<int> fq(j);
    int k = 0;
    while(fq.IsSpace()) {
      fq.InsLast(k);
      k++;
    }
    int z = 0;
    while(!fq.IsEmpty()) {
      if(fq.GetFirst() != z) {
        return __LINE__;
      }
      z++;
    }
    if(z != k)
      return __LINE__;
  }

  {
    FixedQueueC<int> aq(4);
    aq.InsLast(0);
    aq.InsLast(1);
    aq.InsLast(2);
    aq.InsLast(3);
    aq.GetFirst();
    aq.Resize(8);
    RAVL_TEST_EQUALS(aq.Size(),3);
    RAVL_TEST_EQUALS(aq.MaxSize(),8);
    for(int i = 0;i < aq.Size();i++)
      aq[i] = i+1;
  }


  return 0;
}

int TestQueueIter() {
  std::cerr << "Starting TestQueueIter() \n";
  FixedQueueC<int> q(5);
  for(int f = 1;f < 6;f++) {
    for(int z = 0;z < 6;z++) {
      // Shit the queue bountry on.
      for(int k = 0;k < z;k++) {
	q.InsLast(1);
	q.GetFirst();
      }
      for(int i = 0;i < f;i++) 
	q.InsLast(i);
      FixedQueueIterC<int> it(q);
      int cc = 0;
      for(int j = 0;it;j++,it++) {
	if(j >= f) {
	  std::cerr << "operator bool(), test failed. j=" << j << " q.Size()=" << q.Size()<< "\n";
	  return __LINE__;
	}
	if(*it != j) {
	  std::cerr << "operator bool(), iter content test failed. \n";
	  return __LINE__;
	}
	cc++;
      }
      if(cc != f) {
	cerr << "Iteration failed :" << cc << " of "  << f << "elements covered. \n";
	return __LINE__;
      }
      // Empty queue again.
      for(int i = 0;i < f;i++) {
	if(q.GetFirst() != i) {
	  std::cerr << "Content test failed. \n";
	  return __LINE__;
	}
      }
    }
  }
  return 0;
}

int TestQueueRevIter() {
  std::cerr << "Starting TestQueueRevIter() \n";
  FixedQueueC<int> q(5);
#if 0
  q.InsLast(1);
  q.InsLast(2);
  for(FixedQueueRevIterC<int> it(q);it;it++)
    std::cerr << " " << *it;
  std::cerr << "\n";
#endif
#if 1
  for(int f = 1;f < 6;f++) {
    for(int z = 0;z < 6;z++) {
      // Shit the queue bountry on.
      for(int k = 0;k < z;k++) {
	q.InsLast(1);
	q.GetFirst();
      }
      for(int i = 0;i < f;i++) 
	q.InsLast(i);
      FixedQueueRevIterC<int> it(q);
      int cc = 0;
      //cerr << " z=" << z << "\n";
      for(int j = f-1;it;j--,it++) {
	//cerr << " j=" << j << "\n";
	if(j < 0) {
	  std::cerr << "operator bool(), test failed. j=" << j << " q.Size()=" << q.Size()<< "\n";
	  return __LINE__;
	}
	if(*it != j) {
	  std::cerr << "operator bool(), iter content test failed. " << *it << " " << j << "\n";
	  return __LINE__;
	}
	cc++;
      }
      if(cc != f) {
	cerr << "Iteration failed :" << cc << " of "  << f << " elements covered. \n";
	return __LINE__;
      }
      // Empty queue again.
      for(int i = 0;i < f;i++) {
	if(q.GetFirst() != i) {
	  std::cerr << "Content test failed. \n";
	  return __LINE__;
	}
      }
    }
  }
#endif
  return 0;
}

namespace RavlN {
  template
  class FixedQueueC<int>;
}
