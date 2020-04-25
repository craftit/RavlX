// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SARRAY1DITER4_HEADER
#define RAVL_SARRAY1DITER4_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.1D"
//! file="Ravl/Core/Container/SArray/SArray1dIter4.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/09/1998"
//! userlevel=Default

#include "Ravl/SArray1d.hh"

namespace RavlN {
  //! userlevel=Advanced
  //: Quad SArray1d iterator.
  // Fast simple array iterator.
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  class SArray1dIter4C {
  public:
    SArray1dIter4C();
    //: Default constructor.
    
    SArray1dIter4C(const SArray1dC<Data1T> &arr1,
		   const SArray1dC<Data2T> &arr2,
		   const SArray1dC<Data3T> &arr3,
		   const SArray1dC<Data4T> &arr4);
    //: Constructor.
    
    inline bool IsElm() const
    { return this->at1 != this->endOfRow; }
    // At a valid element ?
    
    inline operator bool() const
    { return this->at1 != this->endOfRow; }
    // At a valid element ?
    
    inline void First();
    // Goto first element in arrays.
    
    inline void Next();
    // Goto next element in arrays.
    
    inline void operator++()
    { Next(); }
    // Goto next element in arrays.
    
    inline void operator++(int)
    { Next(); }
    // Goto next element in arrays.
    
    inline Data1T &Data1()
    { return *at1; }
    // Get data.
    
    inline const Data1T &Data1() const
    { return *at1; }
    // Get data.
    
    inline Data2T &Data2()
    { return *at2; }
    // Get data.
    
    inline const Data2T &Data2() const
    { return *at2; }
    // Get data.
    
    inline Data3T &Data3()
      { return *at3; }
    // Get data.
    
    inline const Data3T &Data3() const
      { return *at3; }
    // Get data.
    
    inline Data4T &Data4()
      { return *at4; }
    // Get data.
  
    inline const Data4T &Data4() const
      { return *at4; }
  // Get data.
    
    SArray1dC<Data1T> &Array1()
      { return arr1; }
    //: Access array.
  
    SArray1dC<Data2T> &Array2()
      { return arr2; }
    //: Access array.
    
    SArray1dC<Data3T> &Array3()
      { return arr3; }
    //: Access array.
    
    SArray1dC<Data4T> &Array4()
      { return arr4; }
    //: Access array.
    
    IndexC Index() const
      { return (IntT) (at1 - &arr1[0]); }
    //: Get current index.
    // This is a little slow.
    
    void Invalidate()
    { at1 = const_cast<Data1T *>(endOfRow); }
    //: Invalidate iterator. Makes IsElm() return False.

    bool IsFirst() const
    { return at1 == &(arr1[0]); }
    //: Test if this is the first element in the range.
    // Note,this is slower than IsElm().
    
  private:
    SArray1dC<Data1T> arr1;
    SArray1dC<Data2T> arr2;
    SArray1dC<Data3T> arr3;
    SArray1dC<Data4T> arr4;
    Data1T *at1;
    Data2T *at2;
    Data3T *at3; 
    Data4T *at4;
    const Data1T *endOfRow; // Where's the end of array 1.
  };
  
  ///////////////////////////////////////////
  
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  SArray1dIter4C<Data1T,Data2T,Data3T,Data4T>::SArray1dIter4C()
    : at1(0),
      endOfRow(0)
  {}
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  inline 
  void 
  SArray1dIter4C<Data1T,Data2T,Data3T,Data4T>::First()
  { 
    if(arr1.Size() < 1) { // Check for zero length arrays.
      at1 = 0;
      endOfRow = 0;
      return ;
    }
    at1 = &arr1[0]; 
    at2 = &arr2[0];  
    at3 = &arr3[0]; 
    at4 = &arr4[0]; 
    endOfRow = &(at1)[arr1.Size()];
  }
  
  template<class Data1T,class Data2T,class Data3T,class Data4T>
  SArray1dIter4C<Data1T,Data2T,Data3T,Data4T>::SArray1dIter4C(const SArray1dC<Data1T> &narr1,
							      const SArray1dC<Data2T> &narr2,
							      const SArray1dC<Data3T> &narr3,
							      const SArray1dC<Data4T> &narr4)
    : arr1(narr1),
      arr2(narr2),
      arr3(narr3),
      arr4(narr4)
  {
    RavlAssert(arr1.Size() <= arr2.Size());
    RavlAssert(arr1.Size() <= arr3.Size());
    RavlAssert(arr1.Size() <= arr4.Size());
    First(); 
  }

  template<class Data1T,class Data2T,class Data3T,class Data4T>
  inline 
  void
  SArray1dIter4C<Data1T,Data2T,Data3T,Data4T>::Next()  { 
    RavlAssert(at1 != endOfRow);
    at1++;
    at2++;
    at3++;
    at4++;
  }
}


#endif
