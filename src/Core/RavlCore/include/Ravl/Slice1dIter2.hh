// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SLICE1DITER2_HEADER
#define RAVL_SLICE1DITER2_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Advanced
//! file="Ravl/Core/Container/Buffer/Slice1dIter2.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Arrays"
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/Slice1dIter.hh"

// Ravl namespace.

namespace RavlN {
  
  //: Slice iterator.
  // Iterates through the elements in a slice
  
  template<class Data1T,class Data2T>
  class Slice1dIter2C {
  public:
    Slice1dIter2C()
      : at1(0),
        end(0)
    {}
    //: Creates an invalid iterator.
    
    Slice1dIter2C(const Slice1dC<Data1T> &nvec1,const Slice1dC<Data2T> &nvec2);
    //: Creates an iterator of 'nvec'
    
    Slice1dIter2C(const Slice1dC<Data1T> &nvec1,const Slice1dC<Data2T> &nvec2,const IndexRangeC &rng);
    //: Creates an iterator of 'nvec1' and 'nvec2' over range 'rng'.
    
    void First();
    //: Goto first element.

    operator bool() const
    { return at1 != end; }
    //: Test if we're at a valid element.

    bool IsElm() const
    { return at1 != end; }
    //: Test if we're at a valid element.

    inline bool IsLast() const
    { return (at1+vec1.Stride()) == end; }
    //: Test if we're at the last valid element in the slice.
    // This is slower than IsElm().
    
    inline bool IsFirst() const
    { return at1 == &vec1.First(); }
    //: Test if we're at the first element in the slice.
    // This is slower than IsElm().
    
    void Next() { 
      at1 += vec1.Stride(); 
      at2 += vec2.Stride();
    }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    void operator++(int) 
    { Next(); }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    Data1T &Data1()
    { return *at1; }
    //: Access data at current element.

    const Data1T &Data1() const
    { return *at1; }
    //: Access data at current element.

    Data2T &Data2()
    { return *at2; }
    //: Access data at current element.
    
    const Data2T &Data2() const
    { return *at2; }
    //: Access data at current element.
    
    IntT Index() const
    { return (at1 - &vec1.First())/vec1.Stride(); }
    //: Calculate current index.
    
  protected:
    Data1T *at1;
    Data2T *at2;
    Data1T *end;
    Slice1dC<Data1T> vec1;
    Slice1dC<Data2T> vec2;
  };
  
  /// Slice1dIterC ///////////////////////////////////////////////////////////////////
  
  template<class Data1T,class Data2T>
  Slice1dIter2C<Data1T,Data2T>::Slice1dIter2C(const Slice1dC<Data1T> &nvec1,const Slice1dC<Data2T> &nvec2)
    : vec1(nvec1),
      vec2(nvec2)
  { 
    RavlAssert(vec2.Size() <= vec1.Size());
    First(); 
  }

  template<class Data1T,class Data2T>
  Slice1dIter2C<Data1T,Data2T>::Slice1dIter2C(const Slice1dC<Data1T> &nvec1,const Slice1dC<Data2T> &nvec2,const IndexRangeC &rng) 
    : vec1(nvec1),
      vec2(nvec2)
  {
    if(rng.Size() <= 0) {
      at1 = 0;
      end = 0;
      return;
    }  
    at1 = &(vec1[rng.Min()]);
    at2 = &(vec2[rng.Min()]);
    end = &(at1[rng.Size() * vec1.Stride()]);
  }
  
  template<class Data1T,class Data2T>
  void Slice1dIter2C<Data1T,Data2T>::First() {
    if(vec1.Size() <= 0) {
      at1 = 0;
      end = 0;
      return;
    }
    at1 = &vec1.First();
    at2 = &vec2.First();
    end = &at1[vec1.Size() * vec1.Stride()];
  }
    
  
}

#endif
