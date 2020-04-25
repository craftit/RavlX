// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/testDataProc.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing" 
//! userlevel=Develop

#include "Ravl/DP/IFailOver.hh"
#include "Ravl/DP/IOTap.hh"
#include "Ravl/DP/Multiplex.hh"
#include "Ravl/DP/SplitO.hh"
#include "Ravl/DP/FileIO.hh"
#include "Ravl/DP/Compose.hh"
#include "Ravl/DP/FuncP2Proc.hh"
#include "Ravl/DP/ContainerIO.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/DP/Func2Proc.hh"
#include "Ravl/DP/FuncP2Proc.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/DP/MethodIO.hh"
#include "Ravl/DP/MethodPtrIO.hh"
#include "Ravl/DP/Method2Proc21.hh"
#include "Ravl/DP/MethodPtr2Proc.hh"
#include "Ravl/DP/Func2Proc21.hh"
#include "Ravl/DP/Method2Proc31.hh"
#include "Ravl/DP/Func2Proc31.hh"
#include "Ravl/DP/StreamOp21.hh"
#include "Ravl/DP/StreamOp31.hh"
#include "Ravl/DP/OffsetScale.hh"
#include "Ravl/DP/RunningAverage.hh"
#include "Ravl/DP/SampleStream.hh"
#include "Ravl/DP/CacheIStream.hh"
#include "Ravl/DP/SPortAttach.hh"
#include "Ravl/DP/ListIO.hh"
#include "Ravl/DP/MemIO.hh"
#include "Ravl/config.h"

using namespace RavlN;

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int conv(const int &val) {
  return val;
}

namespace RavlN {
  template class DPFunc2ProcC<int, int, conv>;
  template class DPIOPortBodyC<int, int>;
  template class DPIOPortC<int, int>;
}

int testSimple(); 
int testMultiplex();
int testContainerIO();
int testCompose(); 
int testMethodIO(); 
int testMethodPtrIO(); 
int testFunc2Proc();
int testSPort();
int testSampleStream();
int testIStreamCache();
int testMemIO(); 
int testMethodPtr2Proc();

int main(int nargs,char **argv) {
  int ln;
  if((ln = testSimple()) != 0) {
    cerr << "Error in testSimple(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testSimple() ok\n");
  if((ln = testMultiplex()) != 0) {
    cerr << "Error in testMultiplex(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testMultiplex() ok\n");
  if((ln = testCompose()) != 0) {
    cerr << "Error in testMultiplex(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testCompose() ok\n");
  if((ln = testFunc2Proc()) != 0) {
    cerr << "Error in testFunc2Proc(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testFunc2Proc() ok\n");
  if((ln = testSPort()) != 0) {
    cerr << "Error in testFunc2Proc(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testSPort() ok\n");
  if((ln = testSampleStream()) != 0) {
    cerr << "Error in testFunc2Proc(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testSampleStream() ok\n");
  if((ln = testIStreamCache()) != 0) {
    cerr << "Error in testIStreamCache(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testIStreamCache() ok\n");
  if((ln = testMemIO()) != 0) {
    cerr << "Error in testMemIO(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testMemIO() ok\n");
  if((ln = testMethodPtrIO()) != 0) {
    cerr << "Error in testMethodPtrIO(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testMethodPtrIO() ok\n");
  if((ln = testMethodPtr2Proc()) != 0) {
    cerr << "Error in testMethodPtrIO(), Line:" << ln << "\n";
    return 1;
  }
  ONDEBUG(cerr << "testMethodPtr2Proc() ok\n");
  cerr << "Test passed. \n";
  return 0;
}


RealT MultiplyBy2(const RealT &val)
{ return val * 2; }

int testSimple() {  
  StrOStreamC ostr;
  StrIStreamC src("1 2 3 4 ");
  DPIFileC<RealT>(src) >>  Process(MultiplyBy2) >> DPOFileC<RealT>(ostr);
  StringC result = ostr.String();
  //cerr << "Result=" << result << "\n";
  if(result != "2\n4\n6\n8\n") return __LINE__;
  return 0;
}


int testContainerIO() {
  DListC<IntT> lst1;
  DListC<IntT> lst2;
  lst1.InsLast(1);
  lst1.InsLast(2);
  lst1.InsLast(3);
  DPIContainer(lst1) >> DPOContainer(lst2);
  if(lst2.Size() != 3) return __LINE__;
  if(lst2.First() != lst1.First()) return __LINE__;
  if(lst2.Last() != lst1.Last()) return __LINE__;
  return 0;
}

IntT Trans1(const IntT &v1)
{ return v1 + 1; }

IntT Trans2(const IntT &v1)
{ return v1 + 2; }

int testCompose() {  
  DListC<IntT> lst1;
  DListC<IntT> lst2;
  lst1.InsLast(1);
  lst1.InsLast(2);
  lst1.InsLast(3);
  
  DPIContainer(lst1) >> Process(Trans1) >> Process(Trans2) >> DPOContainer(lst2);
  
  if(lst2.Size() != 3) return __LINE__;
  if(lst2.First() != lst1.First() + 3) return __LINE__;
  if(lst2.Last() != lst1.Last() + 3) return __LINE__;
  
  Process(Trans1) >> Process(Trans2);
  return 0;
}

int testMultiplex() {  
#if 0
  StrOStreamC ostr;
  StrIStreamC src("1 2 3 4");
  DPIFileC<RealT>(src) >>  DPMultiplex(1,Process(MultiplyBy2)) >> DPOFileC<RealT>(ostr);
  StringC result = ostr.String();
  if(result != "2\n4\n6\n8\n") return __LINE__;
#endif
  return 0;
}

class TestC {
public:
  TestC()
    : i(0)
  {}
  
  int Seq()
  { return i++; }

  int Method(const int &var)
  { return var + 1; }

  int Method21(const double &,const bool &)
  { return i++; }

  int Method31(const double &,const bool &,const char &)
  { return i++; }
  
  bool PutInt(const int &v)
  { i = v; return true; }
  
  int i;
};

int Func21(const double &x,const bool &)
{ return (int) x + 1; }

int Func31(const double &x,const bool &,const char &)
{ return (int) x + 1; }

TestC tc;

int testFunc2Proc()
{
  DPFunc2ProcC<int,int,conv> func;
  DPIStreamOp21C<double,bool,int> mop2(Process(tc,&TestC::Method21));
  DPIStreamOp31C<double,bool,char,int> mop3(Process(tc,&TestC::Method31));
  DPIStreamOp21C<double,bool,int> fop2(Process(&Func21));
  DPIStreamOp31C<double,bool,char,int> fop3(Process(&Func31));
  return 0;
}

int testSPort() {  
  DPIPortC<int> ip = IMethod(tc,&TestC::Seq);
  DPISPortAttachC<int> sip(ip,true);
  DPISPortC<int> x(sip);
  if(!x.IsValid()) return __LINE__;
  DPIPortC<int> pip(sip);
  DPSeekCtrlC asc(x);
  if(!asc.IsValid()) return __LINE__;
  DPSeekCtrlC asc3(pip);
  if(!asc3.IsValid()) return __LINE__;
  DPSeekCtrlC asc2(ip);
  if(asc2.IsValid()) return __LINE__;
  return 0;
}

int testSampleStream() {
  DListC<IntT> lst1;
  DListC<IntT> lst2;
  for(int i = 0;i < 100;i++)
    lst1.InsLast(i);
  
  cerr << "Testing sample stream. \n";
  DPIContainer(lst1) >> DPISampleStreamC<IntT>(5) >> DPOContainer(lst2);
  //cerr << "SampleStream:" << lst2 << "\n";
  if(lst2.Size() != 20) return __LINE__;
  return 0;
}

int testVirtualConstructor() {
  
  return 0;
}

int testIStreamCache() {
  cerr << "testIStreamCache(). \n";
  CacheIStreamC<IntT> cacheIStream(5);
  DListC<IntT> data;
  for(int i = 0;i < 10;i++)
    data.InsLast(i);
  DPISListC<IntT> listIn(data);
  DPIPortC<IntT> inPort = listIn >> cacheIStream;
  for(int i = 0;i < 8;i++) {
    IntT v;
    if(!inPort.Get(v))  return __LINE__;
    if(v != i) return __LINE__;
  }
    
  return 0;
}

int testMemIO() {
  //cerr << "Testing MemIO. \n";
  for(IntT i = 0;i < 10;i++) {
    StringC buff;
    if(!MemSave(buff,i))
      return __LINE__;
    IntT x = -1;
    if(!MemLoad(buff,x))
      return __LINE__;
    if(i != x)
      return __LINE__;
  }
  return 0;
}

int testMethodPtrIO() {
  TestC seq;
  DPIPortC<int> ip = IMethodPtr(&seq,&TestC::Seq);
  if(ip.Get() != 0) return __LINE__;
  if(ip.Get() != 1) return __LINE__;
  
  DPOPortC<int> op = OMethodPtr(&seq,&TestC::PutInt);
  op.Put(0);
  if(seq.i != 0) return __LINE__;
  op.Put(1);
  if(seq.i != 1) return __LINE__;
  
  return 0;
}

int testMethodPtr2Proc() {
  TestC seq1;
  TestC seq2;
  DPIPortC<int> sourcep = IMethodPtr(&seq1,&TestC::Seq);;
  DPIPortC<int> ip = sourcep >> ProcessPtr(&seq2,&TestC::Method);
  int val = 0;
  if(!ip.Get(val)) return __LINE__;
  if(val != 1) return __LINE__;
  return 0;
}

#if RAVL_COMPILER_MIPSPRO
DPIPortBodyC<bool> dummy_variable ;
DPIPortBodyC<char> dummy_variable1 ;
#endif

