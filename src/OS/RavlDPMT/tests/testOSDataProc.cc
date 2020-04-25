// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/testOSDataProc.cc"
//! docentry="Ravl.API.Core.Data Processing.Threads"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/config.h"
#include "Ravl/DP/ThreadPipe.hh"
#include "Ravl/DP/MTIOConnect.hh"
#include "Ravl/DP/Blackboard.hh"
#include "Ravl/BufStream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DP/PlayControl.hh"
#include "Ravl/DP/ListIO.hh"
#include "Ravl/DP/MethodIO.hh"
#include "Ravl/DP/Governor.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/FixedBuffer.hh"
#include "Ravl/DP/MethodPtrIO.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/DP/PrintIOInfo.hh"

using namespace RavlN;

namespace RavlN {
  template class DPThreadPipeC<IntT, IntT>;
  template class DPMTIOConnectC<IntT>;
}

int testThreadPipe();
int testBlackboard();
int testPlayControl();


int main() {

  PrintIOFormats(std::cerr);

  RAVL_RUN_TEST(testThreadPipe());
  RAVL_RUN_TEST(testBlackboard());
  RAVL_RUN_TEST(testPlayControl());

  std::cerr << "Test passed. \n";
  return 0;
}

int AddOne(const int &value)
{ return value+1; }

class CounterC 
 : public RavlN::RCBodyC
{
public:
  CounterC()
   : m_counter(0)
  {}

  int GetValue()
  {
    return m_counter++;
  }

  typedef RavlN::SmartPtrC<CounterC> RefT;
  int m_counter;
};

int testThreadPipe() {
  std::cerr << "testThreadPipe() Called. \n";
  {
    CounterC::RefT aCounter = new CounterC();
    DPIPortC<int> source = IMethodPtr(aCounter,&CounterC::GetValue);
    DPIPortC<int> input = source >> Process(&AddOne) >>=
              RavlN::DPFixedBufferC<int>(5);
    int total = 0;
    for(int i = 0;i < 20;i++) {
      int value;
      input.Get(value);
      total += value;
    }
    std::cerr << "Counter=" << total << " (Expected 210) \n";
    if(total != 210)
      return __LINE__;
  }
  {
    CounterC::RefT aCounter = new CounterC();
    DPISPortC<int> source = SPort(IMethodPtr(aCounter,&CounterC::GetValue));
    DPIPortC<int> input = source >> Process(&AddOne) >>=
              RavlN::DPFixedBufferC<int>(5);

    int total = 0;
    for(int i = 0;i < 20;i++) {
      int value;
      input.Get(value);
      total += value;
    }
    std::cerr << "Counter=" << total << " (Expected 210) \n";
    if(total != 210)
      return __LINE__;
  }
  return 0;
}

int testBlackboard() {
  std::cerr << "testBlackboard() Called. \n";
  BlackboardC bb(true);
  IntT bval = 1;
  UIntT uintval = 2;
  float floatval = 3;
  RealT realval = 4;
  bb.Put("Hello",bval);
  bb.Put("UIntT",uintval);
  bb.Put("FloatT",floatval);
  bb.Put("RealT",realval);
  IntT ival = 0;
  if(!bb.Get("Hello",ival)) return __LINE__;
  if(ival != (IntT) bval) return __LINE__;
  
  std::cerr << "Saving blackboard. \n";
  BufOStreamC bos;
  {
    BinOStreamC os(bos);
    os << bb;
  }
  bos.os().flush();

  SArray1dC<char> buf = bos.Data();
  std::cerr << "Loading blackboard. " << buf.Size() <<  "\n";
#if 0
  for(int i = 0;i < buf.Size();i++)
    std::cerr << std::hex << ((unsigned) ((ByteT) buf[i]))  << " ";
  std::cerr << std::endl;
  for(int i = 0;i < buf.Size();i++)
    std::cerr << buf[i] << " ";
  std::cerr << std::dec << std::endl;
#endif
  BufIStreamC bis(buf);
  BinIStreamC is(bis);
  BlackboardC bb2;
  is >> bb2;
  std::cerr << "Done.. \n";
  ival = 0;
  if(!bb2.Get("Hello",ival)) return __LINE__;
  if(ival != (IntT) bval) return __LINE__;
  
  return 0;
}

int testPlayControl() {
  std::cerr << "testPlayControl(), Called. \n";
  DListC<UIntT> list;
  UIntT i;
  UIntT max = 200;
  for(i = 0;i < max;i++)
    list.InsLast(i);
  
  DPISListC<UIntT> ip(list);
  DPIPlayControlC<UIntT> playControl(ip);

  playControl.Stop();
  
  DListC<UIntT> olist;
  DPEventC event =  playControl >> DPGovernorC<UIntT >(0.01) >>= OMethod(olist,&DListC<UIntT>::InsLast);
  
  playControl.PlayFwd();
  
  std::cerr << "Waiting for complete. \n";
  while(playControl.Tell() != (max-1)) {
    //std::cerr << " *********************************************************** " << playControl.Tell() << ". \n";
    Sleep(0.01);
  }
  playControl.Stop();
  std::cerr << "Checking list. \n";
  i = 0;
  for(DLIterC<UIntT> it(olist);it;it++,i++)
    if(*it != i) return __LINE__;
  std::cerr << "testPlayControl(), Done. \n";
  return 0;
}
