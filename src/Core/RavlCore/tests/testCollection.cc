// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/System/testCollection.cc"
//! docentry="Ravl.API.Core.Misc"
//! author="Charles Galambos"

#include "Ravl/Collection.hh"
#include "Ravl/CollectionIter.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/DArray1d.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/DArray1dIter2.hh"
#include "Ravl/DArray1dIter3.hh"
#include "Ravl/DArray1dIter4.hh"
#include "Ravl/DList.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/SysLog.hh"

using namespace RavlN;

int testBasic();
int testDArray1d();
int testDArray1dIO();
int testDArray1dMore();
int testDArray1dEvenMore();
int testDArray1dFrom();
int testDArray1dReverseIter();
int testDArray1dAsQueue();

int main()
{
  RAVL_RUN_TEST(testBasic());
  RAVL_RUN_TEST(testDArray1d());
  RAVL_RUN_TEST(testDArray1dIO());
  RAVL_RUN_TEST(testDArray1dMore());
  RAVL_RUN_TEST(testDArray1dEvenMore());
  RAVL_RUN_TEST(testDArray1dFrom());
  RAVL_RUN_TEST(testDArray1dReverseIter());
  RAVL_RUN_TEST(testDArray1dAsQueue());
  RavlInfo("Collection test passed. ");
  return 0;
}

int testBasic() {
  
  CollectionC<IntT> c(100);
  
  if(!c.IsEmpty()) return __LINE__;
  int x = 0;
  c.Insert(x);
  if(c.Size() != 1) return __LINE__;
  if(c.IsEmpty()) return __LINE__;
  for(int i= 1;i < 100;i++)
    c.Insert(i);
  if(c.Size() != 100) return __LINE__;
  // Check pick.
  int v = c.Pick();
  if(v < 0 || v > 99) return __LINE__;
  // Check array function works, and the
  // value 'v' has been removed from the set.
  if(c.Size() != 99) return __LINE__;
  SArray1dC<IntT> vs(c.Array());
  for(SArray1dIterC<IntT> it(vs);it;it++)
    if(*it == v) return __LINE__;
  
  return 0;
}

namespace RavlN {
  template class CollectionC<IntT>;
  template class CollectionIterC<IntT>;

  template class DArray1dC<IntT>;
  template class DArray1dIterC<IntT>;
  template class DArray1dIter2C<IntT, RealT>;
  template class DArray1dIter3C<IntT, RealT, ByteT>;
  template class DArray1dIter4C<IntT, RealT, ByteT, bool>;
}
const int testSize = 10000;

int testDArray1d() {
  //std::cerr << "testDArray1d(), Called. \n";
  DArray1dC<int> test(10);
  
  if(!test.Contains(2)) return __LINE__;
  if(test.Contains(-1)) return __LINE__;
  if(test.Contains(11)) return __LINE__;
  
  test.Fill(1);
  UIntT count = 0;
  for(DArray1dIterC<int> it(test);it;it++) {
    if(*it != 1) return __LINE__;
    count++;
  }
  if(count != 10) return __LINE__;
  
  int i;
  for(i = 0;i < 10;i++)
    test[i] = i;
  i = 0;
  for(DArray1dIterC<int> it(test);it;it++,i++) 
    if(*it != i) return __LINE__;
  
  // Check SArray() method.
  i = 0;
  for(SArray1dIterC<int> dit(test.SArray(false));dit;dit++,i++)
    if(*dit != i) return __LINE__;
  
  DArray1dC<int> test2(5,true);
  for(i = 0;i < testSize;i++) {
    if(test2.Size() != (UIntT) i) {
      std::cerr << "Test failed at count=" << i << " size=" << test2.Size() << "\n";
      return __LINE__;
    }
    test2.Append(i);
    IntT j = 0;
    for(DArray1dIterC<int> it(test2);it;it++,j++)
      if(*it != j) return __LINE__;
  }
  i = 0;
  for(DArray1dIterC<int> it(test2);it;it++,i++)
    if(*it != i) return __LINE__;
  if(i != testSize) return __LINE__;

  i = 0;
  for(DArray1dIter2C<int,int> it(test,test2);it;it++,i++) {
    if(it.Data1() != it.Data2()) return __LINE__;
    if(it.Index() != i) return __LINE__;
  }
  if(i != 10) return __LINE__;

  i = 0;
  for(DArray1dIter2C<int,int> it(test,test2);it;++it,i++) {
    if(it.Data1() != it.Data2()) return __LINE__;
    if(it.Index() != i) return __LINE__;
  }
  if(i != 10) return __LINE__;

  return 0;
}

int testDArray1dIO() {
  //std::cerr << "testDArray1dIO() Called \n";
  const UIntT checkVal = 0x12349876;
  for(int j = 0;j < 2;j++) {
    int i;
    DArray1dC<int> test1;
    if(j == 1) {
      for(i = 0;i < 7623;i++)
        test1.Append(i);
    }
    
    StrOStreamC sos;
    BinOStreamC bos(sos);
    bos << test1 << checkVal;
    
    DArray1dC<int> test2;
    StrIStreamC sis(sos.String());
    BinIStreamC bis(sis);
    UIntT loadedCheckVal;
    bis >> test2 >> loadedCheckVal;
    if(loadedCheckVal != checkVal) return __LINE__;
    if(test1.Size() != test2.Size()) return __LINE__;
    
    for(DArray1dIter2C<int,int> it(test1,test2);it;it++)
      if(it.Data1() != it.Data2()) return __LINE__;
  }
  return 0;
}

int testDArray1dMore() {
  //std::cerr << "testDArray1dMore(), Called. \n";
  DArray1dC<int> test1;
  DArray1dC<int> test2;
  DArray1dC<int> test3;
  int i;
  for(i = 0;i < 4;i++)
    test2.Append(i);
  for(;i < 8;i++)
    test3.Append(i);
  test1.Append(test2);
  test1.Append(test3);
  if(test1.Size() != 8) return __LINE__;
  for(i = 0;i < 8;i++)
    if(test1[i] != i) return __LINE__;

  SArray1dC<int> sarray = test1.SArray(false);
  for(i = 0;i < 8;i++)
    if(sarray[i] != i) return __LINE__;
  
  // More checks for append.
  
  DArray1dC<int> dset(10,true);
  dset.Append(1);
  dset.Append(2);
  //cout << "dset=" << dset << std::endl;
  DArray1dC<int> dset2(10,true);
  dset2.Append(dset);
  //  std::cout << "dset2=" << dset2 << std::endl;
  if(dset2.Size() != 2) return __LINE__;
  int count = 0;
  for(DArray1dIterC<int> it(dset2);it;it++) {
    if(*it != (count+1)) return __LINE__;
    count++;
  }
  if(count != (IntT) dset2.Size()) return __LINE__;

  // Check remove of single element.
  
  dset = DArray1dC<int>(10,true);
  for(int i = 0;i < 10;i++)
    dset.Append(i);
  
  dset.Remove(5);
  if(dset.Size() != 9) return __LINE__;

  // Check remove of many elements.
  
  dset = DArray1dC<int>(10,true);
  for(int i = 0;i < 10;i++)
    dset.Append(i);
  dset.Remove(4,6);
  if(dset.Size() != 7) return __LINE__;

  // Check remove of many elements bridging block.
  for(int s = 0;s < 9;s++) {
    for(int e = s;e < 9;e++) {
      dset = DArray1dC<int>(4,true);
      int c = 0;
      for(int i = 0;i < 3;i++) {
	Array1dC<int> dat(0,2);
	for(int j = 0;j < 3;j++)
	  dat[j] = c++;
	dset.Append(dat);
      }
      dset.Remove(s,e);
      if(dset.Size() != (UIntT) (8 - (e-s))) {
	cerr << "s=" << s <<" e=" << e << " Collection=" << dset << ". \n"; 
	return __LINE__;
      }
    }
  }

  {
    DArray1dC<int> dset2(20,true);
    int i;
    for(i = 0;i < 9000;i++)
      dset2.Append() = i;
    i = 0;
    if(dset2.Size() != 9000) {
      std::cerr << "SIze=" << dset2.Size() << "\n";
      return __LINE__;
    }
    for(DArray1dIterC<int> it(dset2);it;it++,i++) {
      if(*it != i) return __LINE__;
      
    }
  }
  return 0;
}

int testDArray1dEvenMore() {
  //std::cerr << "testDArray1d(), Add/Delete. \n";
  DArray1dC<IntT> anArray;
  CollectionC<UIntT> validIndex(64);
  IntT opSize = 128;
  for(int j = 0;j < 10000;j++) {
    IntT opVal = RandomInt();
    IntT op = opVal % 2;
    switch(op)
      {
      case 0 : { // Append element.
        IntT count = RandomInt() % opSize;
        //UIntT oldSize = validIndex.Size();
        for(int k = 0;k < count; k++)
          validIndex.Append(anArray.Append(opVal).V());
        //if(oldSize <= 1024 && validIndex.Size() > 1024) std::cerr <<  "Block added. \n";
      } break;
      default:
      case 1: { // Delete element
        if(validIndex.Size() < 1) {
          //cerr << "Empty!\n";
          break; // Nothing to delete.
        }
        
        IntT count = (RandomInt() % (Round(validIndex.Size()*0.7))+1);
        for(int k = 0;k < count; k++) {
          UIntT index = RandomInt() % validIndex.Size();
          UIntT dindex = validIndex[index];
          validIndex.Delete(index);
          if(!anArray.Remove(dindex)) return __LINE__;
        } break;
      }
    //cerr << "Size=" << validIndex.Size()  << "\n";
      }
  }
  
  return 0;
}

int testDArray1dFrom() {
  //std::cerr << "testDArray1d(), From. \n";
  DArray1dC<IntT> anArray;
  Array1dC<IntT> arr1(5);
  Array1dC<IntT> arr2(5);
  Array1dC<IntT> arr3(5);
  for(unsigned i = 0;i < arr1.Size();i++) {
    arr1[i] = 10+i;
    arr2[i] = 20+i;
    arr3[i] = 30+i;
  }
  anArray.Append(arr1);
  anArray.Append(arr2);
  anArray.Append(arr3);
  RAVL_TEST_EQUALS(anArray.Size(),(SizeT) 15);

#if 0
  for(unsigned i = 0;i < anArray.Size();i++) {
    std::cerr << " " << i << "=" << anArray[i] << "\n";
  }
#endif

  DArray1dC<IntT> sa1 = anArray.CompactFrom(1,3);
  RAVL_TEST_EQUALS((SizeT) 3,sa1.Size());
  for(unsigned i = 1;i < 3;i++) {
    RAVL_TEST_EQUALS(sa1[i],(IntT) 10+i);
  }

  DArray1dC<IntT> sa2 = anArray.CompactFrom(1,7);
  RAVL_TEST_EQUALS((SizeT) 7,sa2.Size());
#if 0
  std::cerr << " Test:\n";
  for(unsigned i = 0;i < sa2.Size();i++) {
    std::cerr << " " << i << "=" << sa2[i] << "\n";
  }
#endif
  for(unsigned i = 0;i < 7;i++) {
    //std::cerr <<  " " << i <<  "=" << sa2[i] << " ";
    if(i < 4) {
      RAVL_TEST_EQUALS((IntT) 11+i,sa2[i]);
    } else {
      RAVL_TEST_EQUALS((IntT) 16+i,sa2[i]);
    }
  }

  return 0;
}

int testDArray1dReverseIter()
{
  DArray1dC<int> dset(100,true);
  for(int i = 0; i < 10000;i++) {
    dset.Append(i);
    DArray1dIterC<int> it(dset);
    RAVL_TEST_TRUE(it.IsElm());
    RAVL_TEST_TRUE(it.Last());
    RAVL_TEST_EQUALS(*it,i);
    int j = i;
    for(j--;;j--) {
      if(!it.Prev())
        break;
      RAVL_TEST_EQUALS(*it,j);
    }
    RAVL_TEST_EQUALS(j,-1);
  }
  return 0;
}

int testDArray1dAsQueue()
{
  DArray1dC<int> dset;
  int sizeTarget = 300;
  for(int i = 0;i < sizeTarget;i++)
    dset.Append(i);
  for(int i = 0; i < 100000;i++) {
    dset.Append(i);
    RAVL_TEST_TRUE(dset.Size() == sizeTarget + 1);
    dset.RemoveFirst();
    RAVL_TEST_TRUE(dset.Size() == sizeTarget);

    int size = 0;
    for(RavlN::DArray1dIterC<int> it(dset);it;it++) {
      size++;
    }
    RAVL_TEST_TRUE(dset.Size() == size);
  }
  return 0;
}

