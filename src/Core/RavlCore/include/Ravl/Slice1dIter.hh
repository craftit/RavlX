// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SLICEITER_HEADER
#define RAVL_SLICEITER_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Advanced
//! file="Ravl/Core/Container/Buffer/Slice1dIter.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Arrays"
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/Slice1d.hh"

// Ravl namespace.

namespace RavlN {
  
  
  //: Slice iterator.
  // Iterates through the elements in a slice
  
  template<class DataT>
  class Slice1dIterC {
  public:
    Slice1dIterC()
      : place(0),
        end(0)
    {}
    //: Creates an invalid iterator.
    
    Slice1dIterC(const Slice1dC<DataT> &nvec);
    //: Creates an iterator of 'nvec'
    
    Slice1dIterC(const Slice1dC<DataT> &nvec,const IndexRangeC &rng);
    //: Creates an iterator for sub range of 'nvec'
    
    void First();
    //: Goto first element.

    operator bool() const
    { return place != end; }
    //: Test if we're at a valid element.
    
    bool IsElm() const
    { return place != end; }
    //: Test if we're at a valid element.
    
    inline bool IsLast() const
    { return (place+vec.Stride()) == end; }
    //: Test if we're at the last valid element in the slice.
    // This is slower than IsElm().
    
    inline bool IsFirst() const
    { return place == &vec.First(); }
    //: Test if we're at the first element in the slice.
    // This is slower than IsElm().
    
    void Next() 
    { place += vec.Stride(); }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    void operator++(int) 
    { place += vec.Stride(); }
    //: Goto next element.
    // Call ONLY if IsElm() is valid.
    
    DataT &Data()
    { return *place; }
    //: Access data at current element.

    const DataT &Data() const
    { return *place; }
    //: Access data at current element.

    DataT &operator*() 
    { return *place; }
    //: Access data.
    
    const DataT &operator*() const
    { return *place; }
    //: Access data.
    
    DataT *operator->() 
    { return place; }
    //: Access data.
    
    const DataT *operator->() const
    { return place; }
    //: Access data.
    
    IntT Index() const
    { return (place - &vec.First())/vec.Stride(); }
    //: Calculate current index.
    
  protected:
    DataT *place;
    DataT *end;
    IntT stide;
    Slice1dC<DataT> vec;
  };
  
  /// Slice1dIterC ///////////////////////////////////////////////////////////////////

  template<class DataT>
  Slice1dIterC<DataT>::Slice1dIterC(const Slice1dC<DataT> &nvec)
    : vec(nvec)
  { First(); }

  template<class DataT>
  Slice1dIterC<DataT>::Slice1dIterC(const Slice1dC<DataT> &nvec,const IndexRangeC &rng)
    : vec(nvec)
  { 
    RavlAssert(vec.Range().Contains(rng));
    if(rng.Size() <= 0) {
      place = 0;
      end = 0;
      return ;
    }
    place= &(vec[rng.Min()]);
    end = &(place[rng.Size() * vec.Stride()]);
  }
  
  template<class DataT>
  void Slice1dIterC<DataT>::First() {
    if(vec.Size() <= 0) {
      place = 0;
      end = 0;
      return;
    }
    place = &vec.First();
    end = &(place[(IntT) vec.Size() * vec.Stride()]);
  }
    
  
}

#endif
