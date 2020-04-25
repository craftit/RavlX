// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/testArray2d.cc"
//! docentry="Ravl.API.Core.Arrays.2D"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Array2d.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Array2dIter4.hh"
#include "Ravl/Array2dIter5.hh"
#include "Ravl/Array2dIter6.hh"
#include "Ravl/Array2dIter7.hh"
#include "Ravl/Array2dSqr2Iter.hh"
#include "Ravl/Array2dSqr2Iter2.hh"
#include "Ravl/Array2dSqr3Iter.hh"
#include "Ravl/Array2dSqr31Iter2.hh"
#include "Ravl/Array2dSqr33Iter2.hh"
#include "Ravl/Array2dSqr311Iter3.hh"
#include "Ravl/Array2dSqr3111Iter4.hh"
#include "Ravl/Array2dSqr3311Iter4.hh"
#include "Ravl/Array2dSqr51Iter2.hh"
#include "Ravl/Array2dSqr71Iter2.hh"

#include "Ravl/SArray2dIter.hh"

using namespace RavlN;

int testBasic();
int testSlice();
int testShift();
int testSArrayConv();
int testSqr2();
int testSqr3();
int testSqr31();
int testSqr31b();
int testSqr33();
int testSqr311();
int testSqr3111();
int testSqr51();
int testSqr71();

int main()
{
  int ln;
#if 1
  if((ln = testBasic()) != 0) {
    std::cerr << "Basic Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSlice()) != 0) {
    std::cerr << "Basic slice test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testShift()) != 0) {
    std::cerr << "Basic Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSArrayConv()) != 0) {
    std::cerr << "SArrayConv Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr2()) != 0) {
    std::cerr << "Sqr2Iter Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr3()) != 0) {
    std::cerr << "Sqr3Iter Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr31()) != 0) {
    std::cerr << "Sqr31Iter2 Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr31b()) != 0) {
    std::cerr << "Sqr31Iter2 Array2d test failed line:" << ln << "\n";
    return 1;
  }
#endif
  if((ln = testSqr33()) != 0) {
    std::cerr << "Sqr33Iter2 Array2d test failed line:" << ln << "\n";
    return 1;
  }
#if 1
  if((ln = testSqr311()) != 0) {
    std::cerr << "Sqr311Iter3 Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr3111()) != 0) {
    std::cerr << "Sqr3111Iter4 Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr51()) != 0) {
    std::cerr << "Sqr31Iter2 Array2d test failed line:" << ln << "\n";
    return 1;
  }
  if((ln = testSqr71()) != 0) {
    std::cerr << "Sqr31Iter2 Array2d test failed line:" << ln << "\n";
    return 1;
  }
#endif
  std::cerr << "Test passed ok. \n";
  return 0;
}

int testBasic() {

  Array2dC<IntT> arr1(1,10,2,10);
  Array2dC<IntT> arrX(arr1,arr1.Frame());
  if(arrX.Frame() != arr1.Frame()) return __LINE__;
  
  arr1[1][2]  = 0;
  arr1.Fill(1);
  if(arr1[1][2] != 1) {
    std::cerr << "Fill failed. ("  << arr1[1][2] << ")\n";
    return __LINE__;
  }
  
  UIntT x = 0;
  {
    Array2dIterC<IntT> it(arr1);
    if(it.Index() != arr1.Rectangle().Origin()) {
      std::cerr << "Index check 1 failed.  Inital:" << it.Index() << "  Origin:" << arr1.Rectangle().Origin() << "\n";
      return __LINE__;
    }
    for(;it;it++)
      x += *it;
  }
  if(x != arr1.Rectangle().Area()) {
    std::cerr << "Area test failed. \n";
    return __LINE__;
  }
  arr1 /= 2;
  arr1 += 1;
  arr1 = arr1 + arr1;
  
  {
    Array2dIter2C<IntT,IntT> it(arr1,arr1);
    if(it.Index() != arr1.Rectangle().Origin()) {
      std::cerr << "Index check 2 failed. \n";
      return __LINE__;
    }    
    for(;it;it++)
      it.Data1() = it.Data2();
  }
  {
    Array2dIter3C<IntT,IntT,IntT> it(arr1,arr1,arr1);
    if(it.Index() != arr1.Rectangle().Origin()) {
      std::cerr << "Index check 3 failed. \n";
      return __LINE__;
    }    
    for(;it;it++)
      it.Data1() = it.Data2();
  }
  {
    Array2dIter4C<IntT,IntT,IntT,IntT> it(arr1,arr1,arr1,arr1);
    if(it.Index() != arr1.Rectangle().Origin()) {
      std::cerr << "Index check 4 failed. \n";
      return __LINE__;
    }
    for(;it;it++)
      it.Data1() = it.Data2();
  }


  Array2dC<int> test1(10,10);
  test1.Fill(0);
  Array2dC<int> test2(IndexRangeC(2,4),IndexRangeC(3,5));
  test2.Fill(1);
  test1.SetSubArray(Index2dC(1,1),test2);
  //cerr << test1 << "\n";
  if(test1.Sum() != 9) return __LINE__;
  return 0;
}

int testSlice() {
  Array2dC<int> test(3,3);
  int i = 0;
  for(Array2dIterC<int> it(test);it;it++)
    *it = i++;
  std::cerr << "Test=" << test << "\n";
  Slice1dC<int> slice;
  for(i = 0;i < (IntT) test.Frame().Rows();i++) {
    slice = test.SliceColumn(i);
    std::cerr << "Slice=" << slice << "\n";
    for(int j = 0;j < (IntT) test.Frame().Cols();j++) {
      //cerr << "Val=" << slice[j] << " " << test[j][i] << "\n";
      if(slice[j] != test[j][i]) return __LINE__;
    }
  }
  return 0;
}

int testShift() {
  Array2dC<IntT> arr(IndexRangeC(1,2),IndexRangeC(3,4));
  int c = 0;
  for(Array2dIterC<IntT> it(arr);it;it++)
    *it = c++;
  //cerr << "Org=" << arr << "\n";
  Array2dC<IntT> shift0(arr.CopyAccess());
  for(Array2dIter2C<IntT,IntT> it(arr,shift0);it;it++)
    if(it.Data1() != it.Data2()) return __LINE__;
  Array2dC<IntT> shift1(arr.CopyAccess(-1,-1));
  for(Array2dIter2C<IntT,IntT> it2(arr,shift1);it2;it2++)
    if(it2.Data1() != it2.Data2()) return __LINE__;
  Array2dC<IntT> shift2(arr,arr.Frame(),Index2dC(0,0));
  for(Array2dIter2C<IntT,IntT> it3(arr,shift1);it3;it3++)
    if(it3.Data1() != it3.Data2()) return __LINE__;
  // std::cerr << shift2;
  return 0;
}

int testSArrayConv() {
  Array2dC<IntT> data(IndexRangeC(1,3),IndexRangeC(2,6));
  int i = 0;
  for(Array2dIterC<IntT> it(data);it;it++,i++)
    *it = i;
  Array2dC<IntT> data2(3,3);  
  i = 0;
  for(Array2dIterC<IntT> it(data2);it;it++,i++)
    *it = i;
  SArray2dC<IntT> d1(data2.SArray2d());
  i = 0;
  for(SArray2dIterC<IntT> it(d1);it;it++,i++)
    if(*it != i) return __LINE__;
  i = 0;
  Array2dC<IntT> a1(d1);
  for(Array2dIterC<IntT> it(a1);it;it++,i++)
    if(*it != i) return __LINE__;
  
  SArray2dC<IntT> d2(data.SArray2d(true));
  i = 0;
  for(SArray2dIterC<IntT> it(d2);it;it++,i++)
    if(*it != i) return __LINE__;
  
  return 0;
}


// Test 2x2 iterators.

int testSqr2() {
  Array2dC<IntT> data(4,4);
  
  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  //cerr <<"Locs:" << count << "\n";
  count = 0;
  IntT sqrs = 0;
  Array2dSqr2IterC<IntT> it(data);
  if(!it.First()) return __LINE__;
  if(!it) return __LINE__;
  for(;it;it++,sqrs++)
    count += it.DataBR() + it.DataBL() + it.DataTR() + it.DataTL();
  if(sqrs != 9) return __LINE__;
  if(count != 306) return __LINE__;
  // Seems ok.

  Array2dC<Int16T> data2(4,4);
  data2.Fill(1);
  Array2dSqr2Iter2C<IntT,Int16T> it2(data,data2);
  if(!it2) return __LINE__;
  if(!it2.First()) return __LINE__;
  sqrs = 0;
  count = 0;
  for(;it2;it2++,sqrs++)
    count += it2.DataBR1() + it2.DataBL1() + it2.DataTR1() + it2.DataTL1() +
      it2.DataBR2() + it2.DataBL2() + it2.DataTR2() + it2.DataTL2();
  if(sqrs != 9) return __LINE__;
  if(count != 342) return __LINE__;
  
  Array2dSqr2Iter2C<IntT,IntT> it2a(data,data);
  if(it2a.DataTL1() != 1) return __LINE__;
  if(it2a.DataTR1() != 2) return __LINE__;
  if(it2a.DataBL1() != 5) return __LINE__;
  if(it2a.DataBR1() != 6) return __LINE__;
  for(;it2a;it2a++) {
    if(it2a.DataBR1() != it2a.DataBR2()) return __LINE__;
    if(it2a.DataTR1() != it2a.DataTR2()) return __LINE__;
    if(it2a.DataBL1() != it2a.DataBL2()) return __LINE__;
    if(it2a.DataTL1() != it2a.DataTL2()) return __LINE__;
  }
  return 0;
}

// Test 3x3 iterators.

int testSqr3() {
  Array2dC<IntT> data(5,5);
  data.Fill(1);
  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr3IterC<IntT> it(data);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++)
    count += 
      it.DataBR() + it.DataBM() + it.DataBL() + 
      it.DataMR() + it.DataMM() + it.DataML() + 
      it.DataTR() + it.DataTM() + it.DataTL();
  //cerr << "Count:" << count << "\n";
  if(count != 1053) return __LINE__;
  if(sqrs != 9) return __LINE__;
  
  Array2dC<IntT> datab(IndexRangeC(17,22),
		       IndexRangeC(275,280));
  int i = 0;
  for(Array2dSqr3IterC<IntT> itb(datab);itb;itb++) {
    i += itb.DataMM() + itb.DataMR() + itb.DataML() + itb.DataTM() + itb.DataBM();
  }
  
  return 0;
}

// Test 3x3 iterators.

int testSqr31() {
  Array2dC<IntT> data(5,5);
  data.Fill(1);

  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr31Iter2C<IntT,IntT> it(data,data);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++)
    count += 
      it.DataBR1() + it.DataBM1() + it.DataBL1() + 
      it.DataMR1() + it.DataMM1() + it.DataML1() + 
      it.DataTR1() + it.DataTM1() + it.DataTL1();
  //cerr << "Count:" << count << "\n";
  if(count != 1053) return __LINE__;
  if(sqrs != 9) return __LINE__;
  return 0;
}

// Test 3x3 iterators.

int testSqr31b() {
  Array2dC<IntT> data(5,4);
  Array2dC<IntT> data2(5,4);
  data.Fill(1);
  data2.Fill(0);
  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr31Iter2C<IntT,IntT> it(data,data2);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++) {
    count += 
      it.DataBR1() + it.DataBM1() + it.DataBL1() + 
      it.DataMR1() + it.DataMM1() + it.DataML1() + 
      it.DataTR1() + it.DataTM1() + it.DataTL1();
    it.Data2() = count;
  }
  //cerr << data2;
  //cerr << "Count:" << count << " Sqrs:" << sqrs << "\n";
  if(sqrs != 6) return __LINE__;
  if(count != 567) return __LINE__;
  return 0;
}

// Test 3x3 iterators.

int testSqr33() {
  std::cerr << "testSqr33() \n";
  Array2dC<IntT> data(5,5);
  data.Fill(1);

  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr33Iter2C<IntT,IntT> it(data,data);
  
  
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  IntT count1 = 0;
  IntT count2 = 0;
  for(;it;it++,sqrs++) {
    count1 += 
      it.DataBR1() + it.DataBM1() + it.DataBL1() + 
      it.DataMR1() + it.DataMM1() + it.DataML1() + 
      it.DataTR1() + it.DataTM1() + it.DataTL1();
    
    count2 += 
      it.DataBR1() + it.DataBM1() + it.DataBL1() + 
      it.DataMR1() + it.DataMM1() + it.DataML1() + 
      it.DataTR1() + it.DataTM1() + it.DataTL1();
  }
  
  //cerr << "Count:" << count << "\n";
  if(count1 != 1053) return __LINE__;
  if(count2 != 1053) return __LINE__;
  if(sqrs != 9) return __LINE__;
  
  // Check other iteration style
  
  it.First();
  count1 = 0;
  count2 = 0;
  sqrs = 0;
  while(it) {
    do {
      count1 += 
	it.DataBR1() + it.DataBM1() + it.DataBL1() + 
	it.DataMR1() + it.DataMM1() + it.DataML1() + 
	it.DataTR1() + it.DataTM1() + it.DataTL1();
    
      count2 += 
	it.DataBR1() + it.DataBM1() + it.DataBL1() + 
	it.DataMR1() + it.DataMM1() + it.DataML1() + 
	it.DataTR1() + it.DataTM1() + it.DataTL1();      
      sqrs++;
    } while(it.Next()) ;
  }
  
  if(count1 != 1053) return __LINE__;
  if(count2 != 1053) return __LINE__;
  if(sqrs != 9) return __LINE__;
  std::cerr << "Sqrs=" << sqrs << "\n";
#if 0
  it.First();
  std::cerr << "1:\n";
  std::cerr << it.DataTL1() << " " <<  it.DataTM1() << " " << it.DataTR1() << "\n";
  std::cerr << it.DataML1() << " " <<  it.DataMM1() << " " << it.DataMR1() << "\n";
  std::cerr << it.DataBL1() << " " <<  it.DataBM1() << " " << it.DataBR1() << "\n\n";
  while(it.Next()) {
    std::cerr << it.DataTL1() << " " <<  it.DataTM1() << " " << it.DataTR1() << "\n";
    std::cerr << it.DataML1() << " " <<  it.DataMM1() << " " << it.DataMR1() << "\n";
    std::cerr << it.DataBL1() << " " <<  it.DataBM1() << " " << it.DataBR1() << "\n\n";
  }
  std::cerr << "2:\n";
  std::cerr << it.DataTL1() << " " <<  it.DataTM1() << " " << it.DataTR1() << "\n";
  std::cerr << it.DataML1() << " " <<  it.DataMM1() << " " << it.DataMR1() << "\n";
  std::cerr << it.DataBL1() << " " <<  it.DataBM1() << " " << it.DataBR1() << "\n\n";
  std::cerr << "Original=" << data;
#endif
  return 0;
}

int testSqr311() {
  Array2dC<IntT> data(5,5);
  data.Fill(1);

  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr311Iter3C<IntT,IntT,IntT> it(data,data,data);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++)
    count += 
      it.DataBR1() + it.DataBM1() + it.DataBL1() + 
      it.DataMR1() + it.DataMM1() + it.DataML1() + 
      it.DataTR1() + it.DataTM1() + it.DataTL1();
  //cerr << "Count:" << count << "\n";
  if(count != 1053) return __LINE__;
  if(sqrs != 9) return __LINE__;
  return 0;
}


// Test 3x3 iterators.

int testSqr3111() {
  Array2dC<IntT> data(5,5);
  data.Fill(1);

  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr3111Iter4C<IntT,IntT,IntT,IntT> it(data,data,data,data);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++)
    count += 
      it.DataBR1() + it.DataBM1() + it.DataBL1() + 
      it.DataMR1() + it.DataMM1() + it.DataML1() + 
      it.DataTR1() + it.DataTM1() + it.DataTL1();
  //cerr << "Count:" << count << "\n";
  if(count != 1053) return __LINE__;
  if(sqrs != 9) return __LINE__;
  return 0;
}

// Test 5x5 iterators.

int testSqr51() {
  Array2dC<IntT> data(7,7);
  data.Fill(1);
  
  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr51Iter2C<IntT,IntT> it(data,data);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++) {
    count += it.DataU2()[-2] + it.DataU2()[-1] + it.DataU2()[0] + it.DataU2()[1] + it.DataU2()[2];
    count += it.DataU1()[-2] + it.DataU1()[-1] + it.DataU1()[0] + it.DataU1()[1] + it.DataU1()[2];
    count += it.DataM0()[-2] + it.DataM0()[-1] + it.DataM0()[0] + it.DataM0()[1] + it.DataM0()[2];
    count += it.DataD1()[-2] + it.DataD1()[-1] + it.DataD1()[0] + it.DataD1()[1] + it.DataD1()[2];
    count += it.DataD2()[-2] + it.DataD2()[-1] + it.DataD2()[0] + it.DataD2()[1] + it.DataD2()[2];
  }
  
  //cerr << "Count:" << count << "\n";
  if(count != 5625) return __LINE__;
  if(sqrs != 9) return __LINE__;
  return 0;
}

// Test 7x7 iterators.

int testSqr71() {
  Array2dC<IntT> data(9,9);
  data.Fill(1);
  
  IntT count = 1;
  for(Array2dIterC<IntT> ita(data);ita;ita++)
    *ita = count++;
  
  Array2dSqr71Iter2C<IntT,IntT> it(data,data);
  if(!it) return __LINE__;
  if(!it.First()) return __LINE__;
  IntT sqrs = 0;
  count = 0;
  for(;it;it++,sqrs++) {
    count += it.DataU3()[-3] + it.DataU3()[-2] + it.DataU3()[-1] + it.DataU3()[0] + it.DataU3()[1] + it.DataU3()[2] + it.DataU3()[3];
    count += it.DataU2()[-3] + it.DataU2()[-2] + it.DataU2()[-1] + it.DataU2()[0] + it.DataU2()[1] + it.DataU2()[2] + it.DataU2()[3];
    count += it.DataU1()[-3] + it.DataU1()[-2] + it.DataU1()[-1] + it.DataU1()[0] + it.DataU1()[1] + it.DataU1()[2] + it.DataU1()[3];
    count += it.DataU1()[-3] + it.DataM0()[-2] + it.DataM0()[-1] + it.DataM0()[0] + it.DataM0()[1] + it.DataM0()[2] + it.DataM0()[3];
    count += it.DataD1()[-3] + it.DataD1()[-2] + it.DataD1()[-1] + it.DataD1()[0] + it.DataD1()[1] + it.DataD1()[2] + it.DataD1()[3];
    count += it.DataD2()[-3] + it.DataD2()[-2] + it.DataD2()[-1] + it.DataD2()[0] + it.DataD2()[1] + it.DataD2()[2] + it.DataD2()[3];
    count += it.DataD3()[-3] + it.DataD3()[-2] + it.DataD3()[-1] + it.DataD3()[0] + it.DataD3()[1] + it.DataD3()[2] + it.DataD3()[3];
  }
  
  //cerr << "Count:" << count << "\n";
  if(count != 18000) return __LINE__;
  if(sqrs != 9) return __LINE__;
  return 0;
}

// Force everything to be instantiated to check it at least compiles ok.

#include "Ravl/Array1d.hh"
#include "Ravl/Slice1d.hh"

namespace RavlN {
  template
  class Array2dC<IntT>;

  template
  class Array2dIterC<IntT>;

  template
  class Array2dIter2C<IntT, RealT>;

  template
  class Array2dIter3C<IntT, RealT, ByteT>;

  template
  class Array2dIter4C<IntT, RealT, ByteT, bool>;

  template
  class Array2dIter5C<IntT, RealT, ByteT, bool, Int16T>;

  template
  class Array2dIter6C<IntT, RealT, ByteT, bool, Int16T, UByteT>;

  template
  class Array2dIter7C<IntT, RealT, ByteT, bool, Int16T, UByteT, UIntT>;

  template
  class Array2dSqr2Iter2C<IntT, Int16T>;

  template
  class Array2dSqr3IterC<IntT>;

  template
  class Array2dSqr31Iter2C<IntT, Int16T>;

  template
  class Array2dSqr33Iter2C<IntT, Int16T>;

  template
  class Array2dSqr311Iter3C<IntT, Int16T, RealT>;

  template
  class Array2dSqr3111Iter4C<IntT, Int16T, RealT, ByteT>;

  template
  class Array2dSqr3311Iter4C<IntT, Int16T, RealT, ByteT>;

  template
  class Array2dSqr51Iter2C<IntT, Int16T>;

  template
  class Array2dSqr71Iter2C<IntT, Int16T>;
}