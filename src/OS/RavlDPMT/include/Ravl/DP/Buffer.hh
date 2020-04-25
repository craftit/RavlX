// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPBUFFER_HEADER
#define RAVL_DPBUFFER_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPMT
//! docentry="Ravl.API.Core.Data Processing.Buffers"
//! file="Ravl/OS/DataProc/Buffer.hh"
//! author="Charles Galambos"
//! date="02/10/1998"

#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Semaphore.hh"
#include "Ravl/DList.hh"
#include "Ravl/DP/IOPort.hh"

namespace RavlN {
  //////////////////////////////
  //! userlevel=Develop
  //: MT safe queue body
  
  template<class DataT>
  class DPBufferBodyC 
    : public DPIOPortBodyC<DataT,DataT>
  {
  public:
    DPBufferBodyC(bool nblock = true) 
      : ready(0),
	block(nblock)
    {}
    //: Default constructor.
    
    virtual StringC OpName() const
    { return StringC("buffer"); }
    //: Op type name.
    
    virtual bool IsAsync() const
    { return true; }
    //: Does port work asynchronously ??
    
    virtual void PutEOS()
    { gotEOS = true; }
    //: Put End Of Stream marker.
    
    virtual bool IsPutReady() const
    { return !gotEOS; }
    //: Is port ready for data ?
    
    virtual bool Put(const DataT &dat);
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data);
    //: Put an array of data to stream.
    // returns number of elements processed.
    
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const;
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DataT Get();
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual IntT GetArray(SArray1dC<DataT> &data);
    //: Get an array of data from buffer.
    // returns number of elements processed.
    
    virtual bool Get(DataT &buff);
    //: Try and get next piece of data.
    
    //  virtual RCBodyVC &Copy() const; 
  private:
    DListC<DataT> list; // Data buffer
    MutexC access;     // Mutual exclusion.
    SemaphoreC ready; // Inc'd and Dec'd with items in list.
    bool block;   // Should be block if no data is available ?
    bool gotEOS;  // Have we got an EOS ?
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: MT safe queue handle.
  // This buffer has no size length limit.
  
  template<class DataT>
  class DPBufferC 
    : public DPIOPortC<DataT,DataT> 
  {
  public:
    DPBufferC()
      : DPEntityC(true)
    {}
    //:Default Constructor.
    // Creates an invalid handle.
    
    DPBufferC(bool nblock)
      : DPEntityC(*new DPBufferBodyC<DataT>(nblock))
    {}
    //: Constructor.
    
  };
    

  template<class DataT>
  bool DPBufferBodyC<DataT>::Put(const DataT &dat) { 
    access.Lock();
    list.InsLast(dat);
    access.Unlock();
    ready.Post();
    return true; 
  }
  
  template<class DataT>
  IntT DPBufferBodyC<DataT>::PutArray(const SArray1dC<DataT> &data) {
    access.Lock();
    for(SArray1dIterC<DataT> it(data);it;it++) {
      list.InsLast(*it);
      ready.Post();
    }
    access.Unlock();
    return data.Size(); 
  }
  
  template<class DataT>
  bool DPBufferBodyC<DataT>::IsGetReady() const { 
    if(!block)
      return (ready.Count() > 0); 
    return true;
  }
  
  template<class DataT>
  bool DPBufferBodyC<DataT>::IsGetEOS() const { 
    if(!gotEOS)
      return false;
    return (ready.Count() > 0);
  }
  
  template<class DataT>
  DataT DPBufferBodyC<DataT>::Get() { 
    DataT ret;
    if(!block) {
      if(!ready.TryWait())
	throw DataNotReadyC();
    } else
      ready.Wait();
    access.Lock();
#ifdef RAVL_CHECK
    if(list.IsEmpty()) {
      access.Unlock();
      cerr << "DPBufferBodyC<DataT>::Get(), WARNING: Sync lost!\n";
      throw DataNotReadyC("Unexpected data not ready.");
    }
#endif
    ret = list.PopFirst();
    access.Unlock();
    return ret; 
  }
  
  template<class DataT>
  bool DPBufferBodyC<DataT>::Get(DataT &buff) {
    if(!block) {
      if(!ready.TryWait())
	return false;
    } else
      ready.Wait();
    access.Lock();
#ifdef RAVL_CHECK
    if(list.IsEmpty()) {
      access.Unlock();
      cerr << "DPBufferBodyC<DataT>::Get(DataT &), WARNING: Sync lost!\n";
      return false;
    }
#endif
    buff = list.PopFirst();
    access.Unlock();
    return true;
  }
  
  template<class DataT>
  IntT DPBufferBodyC<DataT>::GetArray(SArray1dC<DataT> &data) {
    for(SArray1dIterC<DataT> it(data);it;it++) {
      if(!block) {
	if(!ready.TryWait())
	  return it.Index().V();
      } else
	ready.Wait();
      access.Lock();
#ifdef RAVL_CHECK
      if(list.IsEmpty()) {
	access.Unlock();
	cerr << "DPBufferBodyC<DataT>::GetArray(SArray1dC<DataT> &), WARNING: Sync lost!\n";
	return false;
      }
#endif
      // We've got at least 1 item.
      *it = list.PopFirst();
      it++;
      // Read anything else we can.
      // don't read a list item before its semaphore has been posted.
      for(;it && !list.IsEmpty();it++) {
	if(!ready.TryWait())
	  break;
	*it = list.PopFirst();
      }
      access.Unlock();
    }
    return data.Size();
  }
}
#endif
