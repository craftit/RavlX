// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Calls.cc"

#include "Ravl/Calls.hh"

namespace RavlN
{
#if RAVL_HAVE_TEMPLATEINSTANTIATE
  template CallFunc0BodyC<bool>; 
#endif
  
  //: Invoke event.
  
  //void TriggerFunc0BodyC::Invoke()
  
};

#if 0
#include "Ravl/Stream.hh"
#include "Ravl/Calls.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/CallMethodRefs.hh"

using namespace RavlN;

int test0() 
{ return 1; }

int test1(int &x) 
{ return x; }

char test2(int &x,char &b) 
{ return b; }

bool test3(int &x,char &b,float &n) 
{ return true; }

float test4(int &x,char &b,float &n,double &d) 
{ return n; }

double test5(int &x,char &b,float &n,double &d,bool &a) 
{ return d; }

class ATestClassC {
 public:
  
  int test1(int &x) 
    { return x; }
  
  char test2(int &x,char &b) 
    { return b; } 
  
  bool test3(int &x,char &b,float &n) 
    { return true; }
  
  float test4(int &x,char &b,float &n,double &d) 
    { return n; }
  
  double test5(int &x,char &b,float &n,double &d,bool &a) 
    { return n; }
  
  void testVoid(int &x,char &b,float &n,double &d,bool &a) 
    {}
  
};

int TestCalls();

int main() {
  int error;
  if((error = TestCalls()) != 0) {
    std::cerr << "TestCalls(), Failed at line :" << error << "\n";
    return 1;
  }
  std::cerr << "Test passed ok. \n";
  return 0;
}

int TestCalls()
{
  // If this lot compiles, then we should be ok.
  
  CallFunc0C<int> func = Trigger(&test0);
  if(func.Call() != 1) return __LINE__;
  CallFunc1C<int&,int> t1 = Trigger(&test1,1);
  if(t1.Call() != 1) return __LINE__;
  char let = 1;

#if 0
  CallFunc2C<int&,char&,char> t2 = Trigger(&test2,2,let);
  if(t2.Call() != let) return __LINE__;
  float f = 0.1f;
  CallFunc3C<int&,char&,float&,bool> t3 = Trigger(&test3,2,let,f);
  if(!t3.Call()) return __LINE__;
  double d = 0.3f;
  CallFunc4C<int&,char&,float&,double&,float> t4 = Trigger(&test4,2,let,f,d);
  if(t4.Call() != f) 
    return __LINE__;
  bool t = true;
  CallFunc5C<int&,char&,float&,double&,bool&,double> t5 = Trigger(&test5,2,let,f,d,t);
  if(t5.Call() != d) return __LINE__;

  ATestClassC ac;

  TriggerC ct1 = Trigger(ac,&ATestClassC::test1,1);
  TriggerC ct2 = Trigger(ac,&ATestClassC::test2,2,let);
  TriggerC ct3 = Trigger(ac,&ATestClassC::test3,2,let,f);
  TriggerC ct4 = Trigger(ac,&ATestClassC::test4,2,let,f,d);
  TriggerC ct5 = Trigger(ac,&ATestClassC::test5,2,let,f,d,t);  
#if !defined(__sgi__)
  TriggerC ctv = Trigger(ac,&ATestClassC::testVoid,2,let,f,d,t);
#endif
  
  TriggerC rct1 = TriggerR(ac,&ATestClassC::test1,1);
  TriggerC rct2 = TriggerR(ac,&ATestClassC::test2,2,let);
  TriggerC rct3 = TriggerR(ac,&ATestClassC::test3,2,let,f);
  TriggerC rct4 = TriggerR(ac,&ATestClassC::test4,2,let,f,d);
  TriggerC rct5 = TriggerR(ac,&ATestClassC::test5,2,let,f,d,t);  
#if !defined(__sgi__)
  TriggerC rctv = TriggerR(ac,&ATestClassC::testVoid,2,let,f,d,t);
#endif
#endif
  return 0;
}

template class CallFunc0C<int>;
template class CallFunc1C<int,ByteT>;
template class CallFunc2C<int,ByteT,RealT>;
template class CallFunc3C<int,ByteT,RealT,UIntT>;
template class CallFunc4C<int,ByteT,RealT,UIntT,float>;
template class CallFunc5C<int,ByteT,RealT,UIntT,float,bool>;

template class CallFunc0BodyC<int>;
template class CallFunc1BodyC<int,ByteT>;
template class CallFunc2BodyC<int,ByteT,RealT>;
template class CallFunc3BodyC<int,ByteT,RealT,UIntT>;
template class CallFunc4BodyC<int,ByteT,RealT,UIntT,float>;
template class CallFunc5BodyC<int,ByteT,RealT,UIntT,float,bool>;

class TestC {
public:
  TestC()
  {}
};

template class CallMethod0BodyC<TestC,int>;
template class CallMethod0C<TestC,int>;

template class CallMethod1C<TestC,int,ByteT>;
template class CallMethod2C<TestC,int,ByteT,RealT>;
template class CallMethod3C<TestC,int,ByteT,RealT,UIntT>;
template class CallMethod4C<TestC,int,ByteT,RealT,UIntT,float>;
template class CallMethod5C<TestC,int,ByteT,RealT,UIntT,float,bool>;

template class CallMethod1BodyC<TestC,int,ByteT>;
template class CallMethod2BodyC<TestC,int,ByteT,RealT>;
template class CallMethod3BodyC<TestC,int,ByteT,RealT,UIntT>;
template class CallMethod4BodyC<TestC,int,ByteT,RealT,UIntT,float>;
template class CallMethod5BodyC<TestC,int,ByteT,RealT,UIntT,float,bool>;

#if 0

template class CallMethod0C<TestC &,int>;
template class CallMethod1C<TestC &,int,ByteT>;
template class CallMethod2C<TestC &,int,ByteT,RealT>;
template class CallMethod3C<TestC &,int,ByteT,RealT,UIntT>;
template class CallMethod4C<TestC &,int,ByteT,RealT,UIntT,float>;
template class CallMethod5C<TestC &,int,ByteT,RealT,UIntT,float,bool>;

template class CallMethod0BodyC<TestC &,int>;
template class CallMethod1BodyC<TestC &,int,ByteT>;
template class CallMethod2BodyC<TestC &,int,ByteT,RealT>;
template class CallMethod3BodyC<TestC &,int,ByteT,RealT,UIntT>;
template class CallMethod4BodyC<TestC &,int,ByteT,RealT,UIntT,float>;
template class CallMethod5BodyC<TestC &,int,ByteT,RealT,UIntT,float,bool>;
#endif

#endif
