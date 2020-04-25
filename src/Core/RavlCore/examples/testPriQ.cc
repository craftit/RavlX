// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Queue/testPriQ.cc"
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Queues"

#include "Ravl/BinStream.hh"
#include "Ravl/PriQueueL.hh"
#include "Ravl/Random.hh"
#include "Ravl/Stream.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/SysLog.hh"

using namespace RavlN;

int testSimple();
int SeqTest();
int InterTest();

int main() {
  std::cerr << "Starting test. \n";
  RAVL_RUN_TEST(testSimple());
  //RAVL_RUN_TEST(SeqTest());
  //RAVL_RUN_TEST(InterTest());
  return 0;
}

int testSimple()
{
  PriQueueLC<int,int> Queue;

  RAVL_TEST_TRUE(Queue.IsEmpty());
  RAVL_TEST_EQUALS(0,Queue.Size());
  Queue.Insert(1,1);
  RAVL_TEST_FALSE(Queue.IsEmpty());
  RAVL_TEST_EQUALS(1,Queue.Size());
  Queue.Insert(1,2);
  RAVL_TEST_EQUALS(2,Queue.Size());
  RAVL_TEST_EQUALS(1,Queue.Top());
  Queue.DelTop();
  RAVL_TEST_EQUALS(1,Queue.Size());
  RAVL_TEST_EQUALS(2,Queue.Top());
  Queue.DelTop();
  RAVL_TEST_TRUE(Queue.IsEmpty());
  RAVL_TEST_EQUALS(0,Queue.Size());
  Queue.Insert(1,1);
  Queue.Insert(1,2);
  Queue.DelTop();
  Queue.Insert(1,3);
  RAVL_TEST_EQUALS(2,Queue.Size());
  //RAVL_TEST_EQUALS(2,Queue.Top());
  //Queue.DelTop();
  //RAVL_TEST_EQUALS(1,Queue.Top());

  return 0;
}

int SeqTest() {
  PriQueueLC<int,int> Queue;
  int i;
  for(i = 2;i < 10000;i++) {
    Queue.Insert(RandomInt(),i+1);
    //Queue.Dump();
  }
  int Last = Queue.TopKey();
  while(Queue.IsElm()) {
    Tuple2C<int,int> &KP =Queue.GetTopPair();
    const int Val = KP.Data2();
    const int Key = KP.Data1();
    if(Key < Last) {
      std::cerr << "\n ERROR !!! " << KP.Data1() << " " << Val << " \n";
      return __LINE__;
    }
    Last = Key;
    //printf("(%d %d) ",KP.Data1(),Val);
  }
  
  return 0;
}  

int InterTest() {
  PriQueueLC<int,int> Queue;
  int i,Last;
  
  for(i = 1;i < 10000;i++)
    Queue.Insert(RandomInt(),i+1);
  std::cerr << "Starting Interleave. \n";
  for(int k = 0;k < 1000;k++) {
    // Remove
    Last = Queue.TopKey();
    for(i = 1;i < 10;i++) {
      Tuple2C<int,int> &KP =Queue.GetTopPair();
      const int Val = KP.Data2();
      const int Key = KP.Data1();
      if(Key < Last) {
	std::cerr << "\n ERROR !!! " << KP.Data1() << " " << Val << " \n";
	return __LINE__;
      }
      Last = Key;
    }
    // Insert.
    for(i = 1;i < 10;i++) {
      Queue.Insert(RandomInt(),i+1);
    }
  }
  std::cerr << "Checking remaining queue. \n";
  Last = Queue.TopKey();
  while(Queue.IsElm()) {
    Tuple2C<int,int> &KP =Queue.GetTopPair();
    const int Val = KP.Data2();
    const int Key = KP.Data1();
    if(Key < Last) {
      std::cerr << "\n ERROR !!! " << KP.Data1() << "  " <<Val << " \n";
      return __LINE__;
    }
    Last = Key;
  }
  
  return 0;
}

namespace RavlN {
  template
  class PriQueueLC<int, UIntT>;
}

