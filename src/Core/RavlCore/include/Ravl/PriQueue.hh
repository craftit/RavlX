// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PRIQUEUE_HEADER
#define RAVL_PRIQUEUE_HEADER 1
////////////////////////////////////////////////
//! userlevel=Normal
//! example=testPriQH.cc
//! docentry="Ravl.API.Core.Queues"
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Queue/PriQueue.hh"
//! author="Charles Galambos"
//! date="24/08/1998"
//! lib=RavlCore

#include "Ravl/SDArray1d.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  //: Fixed size priority queue.
  // Priority queue implemented in a fixed size array. <p>
  // NOTE: The array will be resized if the queue overflows. 
  // BIG OBJECT <p>
  // <pre>
  // Keys must have operation '<' defined.
  // This queue assumes: **** Small numbers == High priority. ****
  // </pre>
  
  template <class KeyT,class DataT>
  class PriQueueC {
  public:
    PriQueueC(UIntT initSize = 32)
      : data(SDArray1dC<Tuple2C<KeyT,DataT> >(initSize))
    {}
    //: Default constructor.
    
    PriQueueC(const PriQueueC<KeyT,DataT> &body)
      : data(body.data)
    {}
    //: Copy constructor.
    
    bool IsElm(void) const
    { return Array().Size() > 0; }
    //: Does the queue contains any items ?
    
    bool IsEmpty(void) const
    { return Array().Size() == 0; }
    //: Is the queue empty ?
    
    DataT &Top(void) { 
      RavlAssert(IsElm()); 
      return Array()[0].Data2();
    }
    //: Look/Modify data on top of queue.
    // Reference not guaranteed to stay valid
    // after any insert/delete operation !
    
    const DataT &Top(void) const {
      RavlAssert(IsElm());
      return Array()[0].Data2();
    }
    //: Look at data on top of queue.
    // Reference not guaranteed to stay valid
    // after any insert/delete operation !
    
    const KeyT &TopKey(void) const {
      RavlAssert(IsElm());
      return Array()[0].Data1();
    }
    //: Look at key on top of queue.
    // Reference not guaranteed to stay valid
    // after any insert/delete operation !
    
    void DelTop(void);
    //: Delete item on top of queue.
    // NB. IsElm(), must be true before calling this.
    
    Tuple2C<KeyT,DataT> GetTopPair(void);
    //: Get Key/Data pair from queue.
    
    DataT GetTop(void);
    //: Get Data from top of queue.
    
    void Insert(const KeyT &Key,const DataT &Data)
    { Insert(Tuple2C<KeyT,DataT>(Key,Data)); }
    //: Insert Data/Key into queue.
    // Will resize the queue if necessary.
    
    void Insert(const Tuple2C<KeyT,DataT> &dat);
    //: Insert Data/Key into queue.
    // Will resize the queue if necessary.
    
    bool Remove(const Tuple2C<KeyT,DataT> &New)
    { return false; }
    //: Remove all instances of Key from queue.
    //!bug: NOT IMPLEMENTED
    // Returns True if found.
    
    bool Remove(const KeyT &Key)
    { return false; }
    //: Remove all instances of Key from queue.
    //!bug: NOT IMPLEMENTED
    // Returns True if found.
    
    SizeT Size(void) const
    { return Array().Size(); }
    //: Get number of items in queue.
    
    void Empty(void)
    { Array().Empty(); }
    //: Empty the queue of all its contents.
    
    bool Check();
    //: Check consistency.
  protected:
    RCWrapC<SDArray1dC<Tuple2C<KeyT,DataT> > > data;
    
    SDArray1dC<Tuple2C<KeyT,DataT> > &Array() 
    { return data.Data(); }
    
    const SDArray1dC<Tuple2C<KeyT,DataT> > &Array() const 
    { return data.Data(); }
  };
  
  //////////////////////////////////////////
  
  
  template <class KeyT,class DataT>
  void PriQueueC<KeyT,DataT>::DelTop(void) {
    SDArray1dC<Tuple2C<KeyT,DataT> > &arr = Array();
    SizeT child;
    const SizeT tsize = arr.Size() - 1;
    if(tsize == 0) {
      arr.Chop();
      return ;
    }
    Tuple2C<KeyT,DataT> lastelem = arr[tsize];
    SizeT i;
    for(i = 1;i * 2 <= tsize;i = child+1) {
      child = (i * 2) - 1;
      if((child != tsize) && arr[child+1].Data1() < arr[child].Data1())
	child++;
      if(arr[child].Data1() < lastelem.Data1())
	arr[i-1] = arr[child];
      else
	break;
    }
    arr[i-1] = lastelem;
    arr.Chop();
  }
  
  template <class KeyT,class DataT>
  Tuple2C<KeyT,DataT> PriQueueC<KeyT,DataT>::GetTopPair(void) {
    Tuple2C<KeyT,DataT> ret = Array()[0];
    DelTop();
    return ret;
  }
  
  template <class KeyT,class DataT>
  DataT PriQueueC<KeyT,DataT>::GetTop(void)  {
    DataT ret = Array()[0].Data2();
    DelTop();
    return ret;
  }
  
  template <class KeyT,class DataT>
  void PriQueueC<KeyT,DataT>::Insert(const Tuple2C<KeyT,DataT> &dat) {
    SDArray1dC<Tuple2C<KeyT,DataT> > &arr = Array();
    SizeT i = arr.Size() + 1;
    if(i == 1) { // First in queue ?
      arr.Add(dat);
      return ;
    }
    // Need do any shifting at all ?
    if(!(dat.Data1() < arr[i/2-1].Data1())) {
      arr.Add(dat); // No, just add on end.
      return ;
    }
    i /= 2;
    Tuple2C<KeyT,DataT> tmp(arr[i-1]); 
    // Have to use temporary here as array may be resized and the
    // reference returned from arr[] may be invalid.
    arr.Add(tmp); // Move up.
    for(;(i > 1) && dat.Data1() < arr[i/2-1].Data1();i /= 2)
      arr[i-1] = arr[i/2-1];
    arr[i-1] = dat;
  }
  
  template <class KeyT,class DataT>
  bool PriQueueC<KeyT,DataT>::Check() {
    for(SizeT i = 1;i < Size();i++) {
      RavlAssert(Array()[((i+1)/2)-1].Data1() < Array()[i].Data1());
    }
    return true;
  }
}

#endif

