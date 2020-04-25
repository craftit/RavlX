// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_LISTIO_HEADER
#define RAVL_LISTIO_HEADER 1
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! author="James Smith"
//! date="24/03/2003"
//! docentry="Ravl.API.Core.Data Processing.IO"
//! file="Ravl/Core/IO/ListIO.hh"

#include "Ravl/DP/SPort.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Assert.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  //////////////////////////////
  //! userlevel=Develop
  //: Seekable port from a DList (body)
  
  template<class DataT>
  class DPISListBodyC 
    : public DPISPortBodyC<DataT> 
  {
  public:
    DPISListBodyC(const DListC<DataT> &dat);
    //: Constructor.
    
    DPISListBodyC()
      : next(0)
    {}
    //: Default constructor.
    
    virtual bool IsGetReady() const
    { return iter; }
    //: Is some data ready ?
    
    virtual bool IsGetEOS() const
    { return !iter; }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DataT Get();
    //: Get next piece of data.
    
    virtual IntT GetArray(SArray1dC<DataT> &data);
    //: Get an array of data from stream.
    // returns the number of elements processed.
    
    virtual bool Get(DataT &buff);
    //: Try and get next piece of data.
    
    virtual bool Seek(UIntT off);
    //: Seek to location in stream.
    // Returns false, if seek failed.
    // if an error occurered (Seek returned False) then stream position will not be changed.

    virtual UIntT Tell() const;
    //: Find current location in stream.
    // Defined as the index of the next object to be written or read. 

    virtual UIntT Size() const;
    //: Find the total size of the stream. (assuming it starts from 0)

    void SetList(const DListC<DataT> &lst) {
      container = lst;
      next = 0;
      iter = container;
    }
    //: Change iterator to new list.
    // This resets the stream to the begining of list 'lst'
    
  protected:
    DListC<DataT> container;
    typename DListC<DataT>::IteratorT iter;
    IntT next;
  };
  
  /////////////////////////////////
  //! userlevel=Normal
  //: Seekable port from a DList (body)
  
  template<class DataT>
  class DPISListC 
    : public DPISPortC<DataT>
  {
  public:
    DPISListC(const DListC<DataT> &dat)
      : DPEntityC(*new DPISListBodyC<DataT>(dat))      
    {}
    //: Constructor.

    DPISListC()
      : DPEntityC(true)
    {}
    //: Default constructor
    // Creates an invalid handle.
  };
  
  ////////////////////////////////////////
  //: Some helper functions.
  
  template<class DataT>
  DPISListC<DataT> DPISList(const DListC<DataT> &dat)
  { return DPISList<DataT>(dat); }
  
  //: Use container as source for stream.
  
  ////////////////////////////////////////////////////////
  
  template<class DataT>
  DPISListBodyC<DataT>::DPISListBodyC(const DListC<DataT> &dat)
    : container(dat),
      iter(const_cast<DListC<DataT> &>(dat)),
      next(0)
  {}

  template<class DataT>
  DataT DPISListBodyC<DataT>::Get() {
    if(!iter)
      throw DataNotReadyC("Invalid iterator in DPISListC");
    const DataT &dat = iter.Data();
    iter++;
    next++;
    ONDEBUG(cerr << "DPISListC::Get -  next frame = " << next << endl);
    return dat;
  }
  
  template<class DataT>
  bool DPISListBodyC<DataT>::Get(DataT &buff) {
    if(!iter)
      return false;
    RavlAssert(iter.IsValid());
    buff = *iter;
    iter++;
    next++;
    ONDEBUG(cerr << "DPISListC::Get -  next frame = " << next << endl);
    return true;
  }
  
  template<class DataT>
  IntT DPISListBodyC<DataT>::GetArray(SArray1dC<DataT> &data) {
    for(SArray1dIterC<DataT> it(data);it;it++) {
      if(!iter)
	return it.Index().V();
      *it = *iter;
      iter++;
      next++;
    }
    ONDEBUG(cerr << "DPISListC::GetArray -  next frame = " << next << endl);
    return data.Size();
  }

  template<class DataT>
  bool DPISListBodyC<DataT>::Seek(UIntT off) {
    ONDEBUG(cerr << "DPISListC::Seek -  target = " << off << ", next frame = " << next << endl);
    if ((IntT)off < 0) 
      return false;
    if (off >= Size()) 
      return false;
    next = off;
    iter.Nth(off);
    return true;
  }
  
  template<class DataT>
  UIntT DPISListBodyC<DataT>::Tell() const {
    ONDEBUG(cerr << "DPISListC::Tell - next frame = " << next << endl);
    return next;
  }
  
  template<class DataT>
  UIntT DPISListBodyC<DataT>::Size() const {
    ONDEBUG(cerr << "DPISListC::Size - size = " << container.Size() << endl);
    return container.Size();
  }
  
#undef ONDEBUG
#undef DODEBUG
  
}

#endif
