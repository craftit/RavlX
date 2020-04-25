// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Array/testArray1d.cc"
//! docentry="Ravl.API.Core.Arrays.1D"
//! author="Charles Galambos"
//! userlevel=Develop

#include "Ravl/Array1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array1dIterR.hh"
#include "Ravl/Array1dIter2.hh"
#include "Ravl/Array1dIter3.hh"
#include "Ravl/Array1dIter4.hh"
#include "Ravl/Array1dIter5.hh"
#include "Ravl/Array1dIter6.hh"
#include "Ravl/Array1dIter7.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int BasicTest();
int ApplyTest();
int ReverseTest();
int testQuickSort();

int  main()
{
  int err;
  if((err = BasicTest()) != 0) {
    std::cerr << "Basic test failed at line " << err << "\n";
    return 1;
  }
  
  if((err = ApplyTest()) != 0) {
    std::cerr << "Apply test failed at line " << err << "\n";
    return 1;
  }
  if((err = ReverseTest()) != 0) {
    std::cerr << "Apply test failed at line " << err << "\n";
    return 1;
  }
  if((err = testQuickSort()) != 0) {
    std::cerr << "Test failed line " << err << "\n";
    return 1;
  }
  
  std::cerr << "Array test passed ok. \n";
  return 0;
}


int BasicTest() {
  std::cerr << "Testing basic funcionality of Array1dC. \n";
  
  ////////////////////////////////////////////////////
  // Test conversions between SArray1dC and Array1dC.
  
  SArray1dC<IntT> sarr1(2);
  sarr1[0] = 1;
  sarr1[1] = 2;
  
  if(sarr1.Contains(2)) {
    std::cerr << "Array bounds problem. 1 \n";
    return __LINE__;
  }
  
  Array1dC<IntT> arr1(sarr1);
  if(arr1[0] != 1 || arr1[1] != 2) {
    std::cerr << "Array to SArray conversion test 1 failed.  (" << arr1[0] << "," << arr1[1] <<")\n";
    return __LINE__;
  }
  if( arr1.Contains(2) || arr1.Contains(-1)) {
    std::cerr << "Array bounds problem. 2 \n";
    return __LINE__;
  }
  
  Array1dC<IntT> arr2(arr1);
  if(arr2[0] != 1 || arr2[1] != 2) {
    std::cerr << "Array to Array conversion test 2 failed. \n";
    return __LINE__;
  }
  
  if(arr1.Size() != arr2.Size()) {
    std::cerr << "Array size mismatch. " << arr1.Size() << "  " << arr2.Size() << "\n";
    return __LINE__;
  }
  
  Array1dC<IntT> a1;
  Array1dC<IntT> ax1(a1);
  a1.Append(arr1);
  a1.Append(arr2);
  if((arr1.Size() + arr2.Size()) != a1.Size()) {
    std::cerr << "Array size mismatch. " << (arr1.Size() + arr2.Size()) << "  " << a1.Size() << "\n";
    return __LINE__;
  }
  if(a1[0] != 1 || a1[1] != 2 || a1[2] != 1 || a1[3] != 2) {
    std::cerr << "Array to Array conversion test 3 failed. \n" << a1 ;
    return __LINE__;
  }
  
  Array1dC<IntT> a2(0);
  if(a2.Size() != 0) return __LINE__;

  Array1dC<IntT> ax2(a2);
  a2.Append(a1);
  a2.Append(arr1);
  if((a1.Size() + arr1.Size()) != a2.Size()) {
    std::cerr << "Array size mismatch. " << (a1.Size() + arr1.Size()) << "  " << a2.Size() << "\n";
    return __LINE__;
  }
  if(a2[0] != 1 || a2[1] != 2 || a2[2] != 1 || a2[3] != 2 || a2[4] != 1 || a2[5] != 2) {
    std::cerr << "Array to Array conversion test 4 failed. \n";
    return __LINE__;
  }
  // Double check the orginal arrays.
  if(arr1.Size() != arr2.Size()) {
    std::cerr << "Array size mismatch. " << arr1.Size() << "  " << arr2.Size() << "\n";
    return __LINE__;
  }
  std::cerr << ax2.Size() << " " << ax1.Size() << "\n";

  Array1dC<IntT> c1 = a2.Copy();
  if((a1.Size() + arr1.Size()) != c1.Size()) {
    std::cerr << "Array size mismatch. " << (a1.Size() + arr1.Size()) << "  " << a2.Size() << "\n";
    return __LINE__;
  }
  if(c1[0] != 1 || c1[1] != 2 || c1[2] != 1 || c1[3] != 2 || c1[4] != 1 || c1[5] != 2) {
    std::cerr << "Array to Array conversion test 5 failed. \n";
    return __LINE__;
  }
  if(a2.Range() != c1.Range()) {
    std::cerr << "Array copy range check failed. \n";
    return __LINE__;
  }
    
  IndexRangeC ir(3,4);
  if(ir.Size() != 2) {
    std::cerr << "Array range size check failed. " << ir.Size() <<  "\n";
    return __LINE__;
  }
  Array1dC<IntT> s1 = Array1dC<IntT>(a2,ir);
  if(s1.Range() != ir) {
    std::cerr << "Array sub range check failed. \n";
    return __LINE__;
  }
  if(s1[3] != 2 || s1[4] != 1) {
    std::cerr << "Array to Array conversion test 6 failed. \n";
    return __LINE__;
  }  
  Array1dC<IntT> s2 = s1.Copy();
  s1[3] = 5;
  if(s2.Range() != ir && s2.Size() == (UIntT) ir.Size()) {
    std::cerr << "Array sub range check failed. \n";
    return __LINE__;
  }
  if(s2[3] != 2 || s2[4] != 1) {
    std::cerr << "Array to Array conversion test 7 failed. \n";
    return __LINE__;
  }

  {
    IndexRangeC rng(3,4);
    
    Array1dC<IntT> ta(rng);
    ta.Fill(0);
    Array1dIterC<IntT> it(ta);
    if(it.Index() != ta.Range().Min()) {
      std::cerr << "Iterator index check failed. \n";
      return __LINE__;
    }
    UIntT count = 0;
    for(;it;it++) {
      *it = 0;
      count++;
    }
    if(count != (UIntT) rng.Size()) {
      std::cerr << "Iterator range size mis-match.\n";
      return __LINE__;
    }
  }
  return 0;
  
}


IntT timesTwo(const IntT &v) {
  return v * 2;
}

void timesTwoIP(IntT &v) {
   v *= 2;
}

int ApplyTest() {
  std::cerr << "Testing apply funcionality of Array1dC. \n";
  
  Array1dC<IntT> arr1(3);
  arr1[0] = 1;
  arr1[1] = 2;
  arr1[2] = 3;
  
  if(arr1.IndexOfMax() != 2) return __LINE__;
  Array1dC<IntT> res = arr1.Apply(timesTwo);
  if(res[0] != 2 || res[1] != 4 || res[2] != 6)
    return __LINE__;
  
  arr1.ApplyIP(timesTwoIP);
  
  if(arr1[0] != 2 || arr1[1] != 4 || arr1[2] != 6)
    return __LINE__;

  return 0;
}

int ReverseTest() {
  std::cerr << "Reverse test. \n";
  Array1dC<IntT> arr(3);
  for(IntT i = 0;i < (IntT) arr.Size();i++)
    arr[i] = i;
  Array1dIterRC<IntT> it(arr);
  if(!it) return __LINE__;
  if(*it != 2) return __LINE__;
  it--;
  if(!it) return __LINE__;
  if(*it != 1) return __LINE__;
  it--;
  if(!it) return __LINE__;
  if(*it != 0) return __LINE__;
  it--;
  if(it) return __LINE__;
  return 0;
}

// Comparison operator for reverse sorting
static bool MyComparisonOp(const int &l1,const int &l2)
  { return l1 > l2; }

int testQuickSort() {
  Array1dC<int> arr(3,7);
  arr[3] = 1;
  arr[4] = 5;
  arr[5] = 2;
  arr[6] = 4;
  arr[7] = 3;
  if(arr.IndexOfMax() != 4) return __LINE__;
  arr.QuickSort(); // sort ascending
  // std::cerr << "Out=" << arr << "\n";
  for(IndexC i = arr.IMin()+1;i <= arr.IMax();i++)
    if(arr[i-1] > arr[i]) return __LINE__;
  if(arr.IndexOfMax() != 7) return __LINE__;
  arr.QuickSort(MyComparisonOp);  // sort descending
  // std::cerr << "Out=" << arr << "\n";
  for(IndexC i = arr.IMin()+1;i <= arr.IMax();i++)
    if(arr[i-1] < arr[i]) return __LINE__;
  if(arr.IndexOfMax() != 3) return __LINE__;
  arr = Array1dC<int>(2);
  arr[0] = 1;
  arr[1] = 0;
  arr.QuickSort(); // sort ascending - check 2 element array works ok
  // std::cerr << "Out=" << arr << "\n";
  if (arr[0] != 0 || arr[1] != 1) return __LINE__;

  return 0;
}


#include "Ravl/Slice1d.hh"

// Force everything to be instantiated to check it at least compiles ok.
namespace RavlN {
  template
  class Array1dC<IntT>;

  template
  class Array1dIterC<IntT>;

  template
  class Array1dIterRC<IntT>;

  template
  class Array1dIter2C<IntT, RealT>;

  template
  class Array1dIter3C<IntT, RealT, ByteT>;

  template
  class Array1dIter4C<IntT, RealT, ByteT, bool>;

  template
  class Array1dIter5C<IntT, RealT, ByteT, bool, Int16T>;

  template
  class Array1dIter6C<IntT, RealT, ByteT, bool, Int16T, UByteT>;

  template
  class Array1dIter7C<IntT, RealT, ByteT, bool, Int16T, UByteT, UIntT>;
}