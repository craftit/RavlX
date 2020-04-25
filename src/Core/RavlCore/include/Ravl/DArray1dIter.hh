// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DARRAY1DITER_HEADER
#define RAVL_DARRAY1DITER_HEADER 1
//! docentry="Ravl.API.Core.Arrays.1D"
//! lib=RavlCore
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/System/DArray1dIter.hh"

#include "Ravl/DArray1d.hh"

namespace RavlN {

  template <typename DataT>
  class DArray1dC;
  template<class DataT>
  class DChunkC;
 
  //! userlevel=Normal
  //: Dynamic array iterator.
  
  template<class DataT>
  class DArray1dIterC {
  public:
    DArray1dIterC()
    {}
    //: Default constructor.

    DArray1dIterC(const DArray1dC<DataT> &narr)
      : arr(narr)
    { First(); }
    //: Constructor.
    
    bool First() {
      cit = arr.Body().chunks;
      if(!cit) {
        it.Invalidate();
        return false;
      }
      it.First(cit->Data()); 
      return true;
    }
    //: Goto first element in the array.
    // Returns true if iterator is at a valid element after operation.

    bool Last() {
      cit = arr.Body().chunks;
      cit.Last();
      if(!cit) {
        it.Invalidate();
        return false;
      }
      it.Last(cit->Data());
      return true;
    }
    //: Goto last element in the array.
    // Returns true if iterator is at a valid element after operation.

    const DArray1dIterC<DataT> &operator=(const DArray1dC<DataT> &narr) {
      arr = narr;
      First();
      return *this;
    }
    //: Assign to array.
    // Leave's iterator at first element in list.
    
    bool IsElm() const
    { return it.IsElm(); }
    //: At a valid element ?
    
    operator bool() const
    { return it.IsElm(); }
    //: At a valid element ?

    DataT &Data()
    { return it.Data(); }
    //: Access data.
    
    const DataT &Data() const
    { return it.Data(); }
    //: Access data.
    
    DataT &operator*() 
    { return it.Data(); }
    //: Access data.

    const DataT &operator*() const
    { return it.Data(); }
    //: Access data.

    DataT *operator->() 
    { return &it.Data(); }
    //: Access data.

    const DataT *operator->() const
    { return &it.Data(); }
    //: Access data.
    
    bool Prev() {
      it--;
      if(it.DataPtr() >= cit->Data().DataStart()) {
        return true;
      }
      cit--;
      if(!cit) return false;
      return it.Last(cit->Data());
    }
    //: Goto previous element.
    // Returns true if iterator is at a valid element after operation.

    bool Next() {
      it++;
      if(it) return true;
      cit++;
      if(!cit) return false;
      return it.First(cit->Data());
    }
    //: Goto next element.
    // Returns true if iterator is at a valid element after operation.
    
    void operator++(int)
    { Next(); }
    //: Goto next element.

    DArray1dIterC &operator++()
    { Next(); return *this; }
    //: Goto next element.

    void operator--(int)
    { Prev(); }
    //: Goto previous element.

    DArray1dIterC &operator--()
    { Prev(); return *this; }
    //: Goto previous element.

    IndexC Index() const
    { return IndexC((IntT)( &(*it) - cit.Data().Data().ReferenceElm())); }
    //: Get index of current element.
    // Note: This is only valid if we're addressing a valid element. That is if IsElm() is true.
    
    bool GotoNth(UIntT offset) {
      UIntT at;
      if(!arr.FindNthChunk(offset,at,cit))
        return false;
      it.First(cit->Nth(offset - at),cit->Data());
      return true;
    }
    //: Goto an offset in the array.

    void Invalidate()
    { it.Invalidate(); }
    //: Invalidate iterator.
    
  protected:
    DArray1dC<DataT> arr;
    BufferAccessIterC<DataT> it;
    IntrDLIterC<DChunkC<DataT> > cit;
  };
    

}


#endif
