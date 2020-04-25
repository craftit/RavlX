// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/testRavlCore.cc"
//! docentry="Ravl.API.Core"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/EntryPnt.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/DList.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/StringList.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Option.hh"
#include "Ravl/FunctionRegister.hh"
#include "Ravl/Cache.hh"
#include "Ravl/String.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Random.hh"
#include "Ravl/IndexRange2dSet.hh"
#include "Ravl/RealRange1dSet.hh"
#include "Ravl/Base64.hh"
#include "Ravl/Random.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/SmartLayerPtr.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/RCWrapIO.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/XMLTree.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/BufStream.hh"
#include "Ravl/IndexRangeSet.hh"
#include "Ravl/SysLog.hh"

#include <string.h>

#if RAVL_HAVE_ANSICPPHEADERS
#include <iostream>
#include <typeinfo>
#else
#include <iostream.h>
#include <typeinfo.h>
#endif

using namespace RavlN;
#if RAVL_HAVE_STDNAMESPACE
using namespace std;
#endif

int testArray1();
int testArray2();
int testDList();
int testTypeName();
int testStrList();
int testIntrDList();
int testOption();
int testFunctionRegister();
int testCache();
int testStringArrayIO();
int testSArrayIO();
int testIndexRange2dSet();
int testRealRange1dSet();
int testBase64();
int testObjIO();
int testVectorIO();
int testSmartPtr();
int testStreamParse();
int testXMLTree();
int testIndexRangeSet();
int testBinStream();

int testRavlCore(int argc,char **argv) {
#if 0
  RAVL_RUN_TEST(testArray1());
  RAVL_RUN_TEST(testArray2());
  RAVL_RUN_TEST(testDList());
  RAVL_RUN_TEST(testTypeName());
  RAVL_RUN_TEST(testStrList());
  RAVL_RUN_TEST(testIntrDList());
  RAVL_RUN_TEST(testOption());
  RAVL_RUN_TEST(testFunctionRegister());
  RAVL_RUN_TEST(testCache());
  RAVL_RUN_TEST(testStringArrayIO());
  RAVL_RUN_TEST(testSArrayIO());
  RAVL_RUN_TEST(testIndexRange2dSet());
  RAVL_RUN_TEST(testRealRange1dSet());
  RAVL_RUN_TEST(testBase64());
  RAVL_RUN_TEST(testObjIO());
  RAVL_RUN_TEST(testVectorIO());
  RAVL_RUN_TEST(testSmartPtr());
  RAVL_RUN_TEST(testStreamParse());
  RAVL_RUN_TEST(testXMLTree());
  RAVL_RUN_TEST(testIndexRangeSet());
#endif
  RAVL_RUN_TEST(testBinStream());
  std::cout << "Test passed. \n";
  return 0;
}

int testArray1() {
  Array1dC<int> test(10);
  Array1dC<int> test2(10);
  test[0] = 1;
  test[9] = 0;
  int count = 0;
  for(Array1dIterC<int> it(test);it;it++) {
    *it = 1;
    count++;
  }
  if(count != 10) return __LINE__;
  for(Array1dIter2C<int,int> it(test,test2);it;it++) {
    it.Data1() = it.Data2();
    count++;
  }
  if(count != 20) return __LINE__;

#if 0
  DListC<Array1dC<int> > listOfArray;
  DListC<Array1dC<int> > list2 = listOfArray.DeepCopy();
#endif
  return 0;
}

int testArray2() {
  Array2dC<int> test(10,10);
  Array2dC<int> test2(10,10);
  test[0][1] = 1;
  test[9][3] = 0;
  int count = 0;
  for(Array2dIterC<int> it(test);it;it++) {
    *it = 1;
    count++;
  }
  if(count != 100) return __LINE__;
  for(Array2dIter2C<int,int> it(test,test2);it;it++) {
    it.Data1() = it.Data2();
    count++;
  }
  return 0;
}

static bool IntLessOrEqual(const int &v1,const int &v2)
{ return v1 <= v2; }

int testDList() {
  DListC<IntT> alist;
  if(!alist.IsEmpty()) return __LINE__;
  alist.InsFirst(1);
  if(alist.Size() != 1) return __LINE__;
  if(alist.IsEmpty()) return __LINE__;
  alist.InsLast(-1);
  if(alist.Size() != 2) return __LINE__;
  alist.InsFirst(2);
  if(alist.Size() != 3) return __LINE__;
  IntT sum = 0;
  DLIterC<IntT> it(alist);
  for(;it;it++)
    sum += *it;  
  if(sum != 2) return __LINE__;
  it.First();
  if(*it != 2) return __LINE__;
  it.DelMoveNext();
  if(*it != 1) return __LINE__;
  if(alist.Size() != 2) return __LINE__;
  it.Del();
  if(it) return __LINE__; 
  if(alist.Size() != 1) return __LINE__;
  alist.Empty();
  if(alist.Size() != 0) return __LINE__;
  if(!alist.IsEmpty()) return __LINE__;
  
  // Check the sort function.
  alist.InsFirst(3); 
  alist.InsFirst(1);
  alist.InsFirst(7);
  alist.InsFirst(4);
  alist.InsFirst(2);
  alist.InsFirst(5);
  alist.InsFirst(6);
  alist.MergeSort(IntLessOrEqual);
  // Check it.
  if(alist.Size() != 7) return __LINE__;
  int last = 0;
  for(it.First();it;it++) {
    if(last > *it) return __LINE__;
    last = *it;
  }
  
  return 0;
}

int testTypeName() {
#if RAVL_HAVE_RTTI
  const char *nm = TypeName(typeid(IntT));
  if(strcmp(nm,"IntT") != 0)  return __LINE__;
  nm = TypeName(typeid(RealT));
  if(strcmp(nm,"RealT") != 0)  return __LINE__;
  if(RTypeInfo("RealT") != typeid(RealT)) return __LINE__;
#endif
  return 0;
}


int testStrList()
{
  StringListC alist("one two 3");
  if(alist.Size() != 3) return __LINE__;
  if(alist.First() != "one") return __LINE__;
  if(alist.Last() != "3") return __LINE__;
  alist.PopFirst();
  if(alist.First() != "two") return __LINE__;
  return 0;
}

int testIntrDList() {
  IntrDListC<DLinkC> lst;
  if(!lst.IsEmpty()) return __LINE__;
  DLinkC lnk1,lnk2,lnk3;
  lst.InsFirst(lnk1);
  if(lst.IsEmpty()) return __LINE__;
  if(lst.Size() != 1) return __LINE__;
  lst.InsFirst(lnk2);
  if(lst.Size() != 2) return __LINE__;
  if(&lst.First() != &lnk2) return __LINE__;
  if(&lst.Last() != &lnk1) return __LINE__;
  lst.InsLast(lnk3);
  if(&lst.Last() != &lnk3) return __LINE__;
  // What about the iterator ?
  IntrDLIterC<DLinkC> it(lst);
  if(!it.IsValid()) return __LINE__;
  if(!it) return __LINE__;
  if(&it.Data() != &lnk2)  return __LINE__;
  it++;
  if(!it) return __LINE__;
  if(&it.Data() != &lnk1)  return __LINE__;
  it.Next();
  if(!it) return __LINE__;
  if(&it.Data() != &lnk3)  return __LINE__;
  it.Next();
  if(it) return __LINE__;
  it--;
  if(!it) return __LINE__;
  if(&it.Data() != &lst.Last()) return __LINE__;
  return 0;
}

int testOption() {
  // Do a quick check that OptionC is doing something sensible.
  int nargs = 15;
  char const *argv[] = { "arg0","-d","0.3","-i","4","-k","1","2","-o1","t1 t2","-o2","t3 t4","-o3","t5 t6","-b",0 };
  OptionC opt(nargs,(char **) argv);
  RealT vd= opt.Real("d",0,"Read in a real. ");
  RealT vd2= opt.Real("d2",0.4,"Read in a real. ");
  IntT vi= opt.Int("i",1,"Read in a int. ");
  Index2dC ind = opt.Index2d("k",0,1,"Index 2");
  StringC s1 = opt.String("o1","","Option 1");
  StringC s2 = opt.String("o2","","Option 2");
  StringC s3 = opt.String("o3","","Option 3");
  bool b = opt.Boolean("b", "Boolean test");
  opt.Check();
  if(vd != 0.3) return __LINE__;
  if(vi != 4) return __LINE__;
  if(vd2 != 0.4) return __LINE__;
  if(!opt.IsOnCommandLine("d")) return __LINE__;
  if(opt.IsOnCommandLine("x")) return __LINE__;
  //cout << " '" << s1 << "' '" << s2 << "' '" << s3 << "'\n";
  if(s1 != "t1 t2") return __LINE__;
  if(s2 != "t3 t4") return __LINE__;
  if(s3 != "t5 t6") return __LINE__;
  RAVL_TEST_EQUALS(ind,Index2dC(1,2));
  if(!b) return __LINE__;
  StrOStreamC str;
  str <<opt;
  if (str.String() != "arg0 -d 0.3 -i 4 -k 1 2 -o1 t1\\ t2 -o2 t3\\ t4 -o3 t5\\ t6 -b ")
    return __LINE__;
  return 0;
}

int testFunctionRegister() {
  
  int (*afunc)(int);
  
  if(!LookupFunctionByName("NullFunc(int)",afunc)) return __LINE__;
  if(afunc(2) != 2) return __LINE__;
  return 0;
}

int testCache() {
  CacheC<IntT,IntT> cache(5);
  for(int i = 0;i < 10;i++)
    cache.Insert(i,i);
  if(cache.Size() != 5)
    return __LINE__;
  for(CacheIterC<IntT,IntT> it(cache);it;it++) {
    if(it.Key() < 5) return __LINE__;
    if(it.Key() != it.Data()) return __LINE__;
  }  
  IntT v;
  for(int i = 5;i <= 9;i++) {
    if(!cache.Lookup(i,v)) return __LINE__;
    if(v != i) return __LINE__;
  }
    
  return 0;
}

int testStringArrayIO() {
 
  // Test SArray1dC...
  SArray1dC<StringC> str(3);
  str[0] = StringC("Hello1");
  str[1] = StringC("Hello2");
  str[2] = StringC("Hello3");
  
  StrOStreamC outs;
  outs << str;
  StringC data = outs.String();
  StrIStreamC ins(data);
  SArray1dC<StringC> tstr;
  ins >> tstr;
  if(str.Size() != tstr.Size()) return __LINE__;
  if(str[0] != tstr[0]) return __LINE__;
  if(str[1] != tstr[1]) return __LINE__;
  if(str[2] != tstr[2]) return __LINE__;

  // Test Array1dC...
  
  Array1dC<StringC> tarr(tstr);
  StrOStreamC xout;
  xout << tarr;
  
  StrIStreamC ins2(xout.String());
  //cerr << "Data='" << data << "'\n";
  Array1dC<StringC> xstr;
  ins2 >> xstr;
  if(str.Size() != (UIntT) xstr.Range().Size()) return __LINE__;
  if(str[0] != xstr[0]) return __LINE__;
  if(str[1] != xstr[1]) return __LINE__;
  if(str[2] != xstr[2]) return __LINE__;
  
  return 0;
}

int testSArrayIO() {
  for(int s = 0;s < 500;s++) {
    UIntT i;
    SArray1dC<ByteT> bytePad(s);
    for(i = 0;i < bytePad.Size();i++)
      bytePad[i] = (ByteT) i;
    SArray1dC<RealT> values(s);
    for(i = 0;i < values.Size();i++)
      values[i] = Random1();
    
    StrOStreamC outs;
    BinOStreamC bos(outs);
    bos << bytePad << values;
    SArray1dC<RealT> newValues;
    StringC str = outs.String();
    StrIStreamC ins(str);
    BinIStreamC bis(ins);
    SArray1dC<ByteT> tmp;
    bis >> tmp >> newValues;
    if(newValues.Size() != values.Size())
      return __LINE__;
    for(i = 0;i < values.Size();i++)
      if(Abs(newValues[i] - values[i]) > 0.000000001) return __LINE__;
  }
  return 0;
}


int testIndexRange2dSet() {
  IndexRange2dC rect1(0,1,0,1);
  IndexRange2dC rect2(1,2,1,2);
  //cout << "R1:" << rect1 << " Area:" << rect1.Area() << "\n";
  //cout << "R2:" << rect2 << " Area:" << rect2.Area() << "\n";
  
  IndexRange2dSetC t1 = IndexRange2dSetC::Subtract(rect1,rect2);
  if(t1.Area() != 3) {
    std::cerr << "Failed test 1. " << t1.Area() << " Elems:" << t1.Size() << "\n";
    std::cerr << t1 << "\n";
    return __LINE__;
  }

  IndexRange2dSetC t2 = IndexRange2dSetC::Subtract(rect2,rect1);
  if(t2.Area() != 3) {
    std::cerr << "Failed test 2. \n";
    return __LINE__;
  }

  IndexRange2dC rect3(0,1,2,3);
  
  IndexRange2dSetC t3 = IndexRange2dSetC::Subtract(rect2,rect3);
  if(t3.Area() != 3) {
    std::cerr << "Failed test 3. " << t3.Area() << " Elems:" << t3.Size() << "\n";
    std::cerr << t3 << "\n";
    return __LINE__;
  }
  
  IndexRange2dSetC t4 = IndexRange2dSetC::Subtract(rect3,rect2);
  if(t4.Area() != 3) {
    std::cerr << "Failed test 4. " << t4.Area() << " Elems:" << t4.Size() << "\n";
    std::cerr << t4 << "\n";
    return __LINE__;
  }
  return 0;
}

int testRealRange1dSet() {
  RealRangeC range1(0,1);
  RealRangeC range2(2,3);
  RealRangeC range3(0,3);
  RealRangeC range4(1,2);

  //cout << "R1:" << rect1 << " Area:" << rect1.Area() << "\n";
  //cout << "R2:" << rect2 << " Area:" << rect2.Area() << "\n";

  RealRange1dSetC t1 = RealRange1dSetC::Add(range1,range2);
  if(t1.Length() != 2) {
    return __LINE__;
  }

  RealRange1dSetC t2 = RealRange1dSetC::Subtract(range3,range4);
  if(t2.Length() != (range3.Size() - range4.Size()))
    return __LINE__;

  return 0;
}

int testBase64() {
  for(int i = 0;i <100;i++) {
    SArray1dC<char> data(RandomInt() % 400);
    for(SArray1dIterC<char> it(data);it;it++)
      *it = (char) (RandomInt() % 256);
    StringC enc = Base64C::Encode(data);
    enc = " \n  " + enc + "\n";
    //cerr << "Encoded string=" << enc << "\n";
    SArray1dC<char> data2 = Base64C::Decode(enc);
    if(data2.Size() != data.Size()) return __LINE__;
    //cerr << "1= " << data << "\n";
    //cerr << "2= " << data2 << "\n";
    for(SArray1dIter2C<char,char> it(data,data2);it;it++) {
      //cerr  << (int) it.Data1() << " != " << (int) it.Data2() << " \n";
      if(it.Data1() != it.Data2())  {
      	std::cerr << "Mismatch at " << it.Index() << " :  " << (int) it.Data1() << " != " << (int) it.Data2() << " \n";
	      return __LINE__;
      }
    }
  }
  
  // Check some corner cases.
  StringC testStr("");
  SArray1dC<char> dat = Base64C::Decode(testStr);
  if(dat.Size() != 0) return __LINE__;
  
  testStr = "====";
  dat = Base64C::Decode(testStr);
  if(dat.Size() != 1) return __LINE__;
  
  // Check we throw an exception on invalid strings.
  StringC sourceStr = "ABCD";
  for(int i = 1;i < 4;i++) {
    try {
      // Pass an invalid strings
      StringC enc;
      for(int k = 0;k < i;k++)
        enc += sourceStr[k];
      dat = Base64C::Decode(enc);
      return __LINE__;
    } catch(...) {
    }
  }

  // Throw rubish at the decoder.
  // Is should either return data, or throw an exception.
  UIntT datCount = 0,excCount = 0;
  for(int i = 0;i < 10000;i++) {
    StringC edata(RandomInt() % 2048,"");
    for(UIntT k = 0;k < edata.Size();k++)
      edata[k] = RandomInt() % 256;
    try {
      SArray1dC<char> dat = Base64C::Decode(edata);
      datCount++;
    } catch(...) {
      excCount++;
    }
  }
  //cerr << "Data count=" << datCount << " Exception count=" << excCount << "\n";
  return 0;
}


int testObjIO() {
  int x = 2;
  int *val1 = &x;
  int *val2 = &x;
  
  DListC<int> alist;
  alist.InsLast(1);
  alist.InsLast(2);
  alist.InsLast(3);
  DListC<int> blist(alist);
  // std::cerr << "&x=" << ((void *)&x) << " &val=" << ((void *) &val) << "\n";
  StrOStreamC os;
  BinOStreamC bos(os);
  
  RavlN::SmartOwnerPtrC<RCLayerBodyC> ownerPtr = new RCLayerBodyC();
  RavlN::SmartCallbackPtrC<RCLayerBodyC> callbackPtr = ownerPtr.BodyPtr();

  bos << ObjIO(val1) << ObjIO(val2) << ObjIO(alist) << ObjIO(blist);

  // IO of Owner/Callback pointers not supported yet.
  //<< ObjIO(ownerPtr) << ObjIO(callbackPtr);
  
  StringC data = os.String();
  //OStreamC file("test.abs");
  //file.write(data.chars(),data.Size());
  
  StrIStreamC is(data);
  BinIStreamC bis(is);
  
  int *valx = 0;
  int *valy = 0;

  DListC<int> list1;
  DListC<int> list2;

  RavlN::SmartOwnerPtrC<RCLayerBodyC> ownerPtrRL;
  RavlN::SmartCallbackPtrC<RCLayerBodyC> callbackPtrRL;

  bis >> ObjIO(valx) >> ObjIO(valy) >> ObjIO(list1) >> ObjIO(list2);

  // IO of Owner/Callback pointers not supported yet.
  //>> ObjIO(ownerPtrRL) >> ObjIO(callbackPtrRL);

  if(valx != valy) return __LINE__;
  
  //cerr << "List1=" << list1.Hash() << " List2=" << list2.Hash() << "\n";
  if(list1.VoidPtr() != list2.VoidPtr()) return __LINE__;
  IntT sum = 0;
  for(DLIterC<int> it(list1);it;it++)
    sum += *it;
  if(sum != 6) return __LINE__;



  return 0;
}

int testVectorIO() {
  TFVectorC<int,3> value;
  value.Fill(0);
  std::cout << value;
  return 0;
}

/***************************************************************************************************/

class TestDerivedC
  : public RCBodyC
{
public:
  TestDerivedC()
    : m_i(0)
  {}

  TestDerivedC(BinIStreamC &strm)
    : m_i(0)
  { strm >> m_i; }
  
  TestDerivedC(int i)
    : m_i(i)
  {}
  
  bool Save(BinOStreamC &strm) const
  { strm << m_i; return true; }
  
  int m_i;
};

class TestDerived2C
  : public RCBodyVC
{
public:
  TestDerived2C(BinIStreamC &strm)
    : RCBodyVC(strm),
     m_i(0)
  { strm >> m_i; }

  TestDerived2C(std::istream &strm)
    : RCBodyVC(strm),
      m_i(0)
  { strm >> m_i; }
  
  TestDerived2C(int i)
    : m_i(i)
  {}
  
  bool Save(BinOStreamC &strm) const {
    if(!RCBodyVC::Save(strm))
      return false;
    strm << m_i;
    return true;
  }
  
  int m_i;
};

#if 0
BinOStreamC &operator<<(BinOStreamC &strm,const TestDerivedC &obj) {
  strm << obj.m_i;
  return strm;
}

BinIStreamC &operator>>(BinIStreamC &strm,TestDerivedC &obj) {
  strm >> obj.m_i;
  return strm;
}
#endif

RAVL_INITVIRTUALCONSTRUCTOR_NAMED(TestDerived2C,"TestDerived2C");

int testSmartPtr() {
  
  {
    StrOStreamC outs;
    BinOStreamC bos(outs);
    SmartPtrC<TestDerivedC> sptr = new TestDerivedC(2);
    bos << sptr;
    
    SmartPtrC<TestDerived2C> sptrb;
    bos << sptrb;
    
    StrIStreamC ins(outs.String());
    BinIStreamC bis(ins);
    SmartPtrC<TestDerivedC> lptr;
    bis >> lptr;
    if(!lptr.IsValid()) return __LINE__;
    if(lptr->m_i != 2) return __LINE__;
    bis >> lptr;
    if(sptrb.IsValid()) return __LINE__;
  }  
  {
    StrOStreamC outs;
    BinOStreamC bos(outs);
    SmartPtrC<TestDerived2C> sptr2 = new TestDerived2C(3);
    bos << sptr2;
    
    SmartPtrC<TestDerived2C> sptr2b;
    bos << sptr2b;
    
    StrIStreamC ins(outs.String());
    BinIStreamC bis(ins);
    SmartPtrC<TestDerived2C> lptr;
    bis >> lptr;
    if(!lptr.IsValid()) return __LINE__;
    if(lptr->m_i != 3) return __LINE__;
    bis >> lptr;
    if(lptr.IsValid()) return __LINE__;
  }
  return 0;
}

int testStreamParse() {
  
  {
    StringC testString = " \t\n  *1234$$5678$$9012";
    StrIStreamC istrm(testString);
    if(istrm.SkipWhiteSpace() != '*')
      return __LINE__;
    if(istrm.ClipTo("$$") != "1234")
      return __LINE__;
    if(istrm.ClipTo("$$") != "5678")
      return __LINE__;
    if(istrm.ClipTo("$$") != "9012")
      return __LINE__;
  }
  {
    StringC testString = "1234$5678$9012";
    StrIStreamC istrm(testString);
    if(istrm.ClipTo('$') != "1234")
      return __LINE__;
    if(istrm.ClipTo('$') != "5678")
      return __LINE__;
    if(istrm.ClipTo('$') != "9012")
      return __LINE__;
  }
  {
    StringC testString = "1234$$5678$9012$3456$!$1";
    StrIStreamC istrm(testString);
    if(!istrm.SkipTo("$$"))
      return __LINE__;
    if(istrm.ClipWord("$") != "5678")
      return __LINE__;
    if(istrm.GetChar() != '$')
      return __LINE__;
    if(!istrm.SkipTo('$'))
      return __LINE__;
    if(istrm.ClipTo('$') != "3456")
      return __LINE__;
    if(!istrm.Skip("$!"))
      return __LINE__;
    if(istrm.ClipTo('$') != "1")
      return __LINE__;
  }


  return 0;
}


void InitStream(IStreamC &istrm) {
  StringC buff =
    "<config> \n"
    " <test1 value=\"Hello\" /> \n"
    " <test2 /> \n"
    " <test3 /> \n"
    "</config> \n";

  StrIStreamC strIStrm(buff.Copy());
  istrm = strIStrm;
}

int testXMLTree() {

  IStreamC strm;
  InitStream(strm);
  XMLTreeC theTree(true);
  if(!theTree.Read(strm)) return __LINE__;
  //theTree.Dump(std::cout);
  XMLTreeC childTree;
  if(!theTree.Child("config",childTree)) return __LINE__;

  InitStream(strm);
  XMLIStreamC xs(strm);
  StringC s;
  xs.ReadTag(s);
  theTree = XMLTreeC("test1");
  if(!theTree.ReadNode(xs)) return __LINE__; // read node with non-static method
  XMLTreeC::ReadNode(xs, "test2"); // read node with static method
  bool xcpt(false);
  try { XMLTreeC::ReadNode(xs, "test2"); } // read node with static method: should fail
  catch (ExceptionBadConfigC &) { xcpt = true; }
  if (!xcpt) return __LINE__;
  return 0;
}


int testIndexRangeSet()
{
  for(int k = 0;k < 10;k++) {
    IndexRangeSetC rngSet;
    int maxRange = 30;
    SArray1dC<bool> done(maxRange);
    done.Fill(false);
    for(int i = 0;i < 100;i++) {
      IndexC ind = (int) RandomInt() % maxRange;
      done[ind] = true;
      //RavlDebug("Adding %d ",ind.V());

      // Add to set.
      rngSet.Add(ind);

      // Did it work ?
      RAVL_TEST_TRUE(rngSet.Contains(ind));

      // Check invariants.
      for(DLIterC<IndexRangeC> it(rngSet);it;it++) {
        RAVL_TEST_TRUE(it->Min() <= it->Max());
        if(!it.IsLast()) {
          RAVL_TEST_TRUE((it->Max()+1) < it.NextData().Min());
        }
      }

      // Check logically correct.
      for(int j = 0;j < maxRange;j++) {
        RAVL_TEST_TRUE(rngSet.Contains(j) == done[j]);
      }
    }
  }

  return 0;
}


int testBinStream()
{
  SArray1dC<char> testBuff(12450);
  for(int i = 0;i < testBuff.Size();i++)
    testBuff[i] = i;

  BufOStreamC obuf;
  {
    BinOStreamC bos(obuf);
    int n = 1;
    bos << n;
    bos << testBuff;
    bos << testBuff;
    StringC str("hello");
    bos << str;

    BufOStreamC ss;
    BinOStreamC sbos(ss);
    sbos << str;
    ss.os().flush();
    SArray1dC<char> buf = ss.Data();
    for(int i = 0;i < buf.Size();i++)
      RavlDebug(" %d %x ",i,(int)((ByteT)buf[i]));
    bos << buf;
  }

  {
    BufIStreamC ibuf(obuf.Data());
    BinIStreamC bis(ibuf);
    int n = 0;
    bis >> n;
    if(n != 1) return __LINE__;
    SArray1dC<char> rbuf1;
    bis >> rbuf1;
    if(rbuf1.Size() != testBuff.Size()) return __LINE__;
    for(int i = 0;i < testBuff.Size();i++)
      if(rbuf1[i] != testBuff[i]) return __LINE__;
    SArray1dC<char> rbuf2;
    bis >> rbuf2;
    if(rbuf2.Size() != testBuff.Size()) return __LINE__;
    for(int i = 0;i < testBuff.Size();i++)
      if(rbuf2[i] != testBuff[i]) return __LINE__;

    StringC aStr;
    bis >> aStr;
    if(aStr != "hello") return __LINE__;

    SArray1dC<char> subBuf;
    bis >> subBuf;

    for(int i = 0;i < subBuf.Size();i++)
      RavlDebug(" %d %x ",i,(int)((ByteT)subBuf[i]));

    StringC istr;
    BufIStreamC sis(subBuf);
    BinIStreamC sbis(sis);
    sbis >> istr;
    RavlDebug("String:%s ",istr.c_str());
    if(istr != "hello") return __LINE__;
  }


  return 0;
}

RAVL_ENTRY_POINT(testRavlCore);
