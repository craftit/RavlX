// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPHOLD_HEADER
#define RAVL_DPHOLD_HEADER 1
///////////////////////////////////////////////
//! lib=RavlDPMT
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Data Processing.Hold" 
//! file="Ravl/OS/DataProc/Hold.hh"
//! author="Charles Galambos"
//! date="03/08/1998"

#include "Ravl/DP/IOPort.hh"
#include "Ravl/DP/Process.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlN {
  //////////////////////////////
  //! userlevel=Develop
  //: Hold value until replaced by put.
  // Get will always return that last value
  // put to this class.
  
  template<class DataT>
  class DPHoldBodyC : public DPIOPortBodyC<DataT,DataT>
  {
  public:
    DPHoldBodyC(const DataT &init = DataT());
    //: Default constructor.
    
    virtual StringC OpName() const
    { return StringC("hold"); }
    //: Op type name.
    
    virtual bool IsPutReady() const;
    //: Ready to accept data ?
    // true = yes.
  
    virtual bool Put(const DataT &dat);
    //: Put data.
  
    virtual IntT PutArray(const SArray1dC<DataT> &data);
    //: Put an array of data to stream.
  
    virtual void PutEOS();
    //: Put End Of Stream marker.
  
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
  
    virtual DataT Get();
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.

    virtual bool Get(DataT &buff);
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
  
    virtual IntT GetArray(SArray1dC<DataT> &data);
    //: Get an array of data from stream.
  
    virtual RCBodyVC &Copy() const; 
    //: Make a deep copy.
  
    inline void Set(const DataT &dat);
    //: Set holding value.
  
    inline DataT Data() const;
    //: Get data directly.
  
  private:
    bool gotEOS;
    DataT hold; // Hold onto data.
    MutexC access;
  };

  //////////////////////////////////
  //! userlevel=Normal
  //: Hold value until replaced by put.
  // Get will always return that last value
  // put to this class.

  template<class DataT>
  class DPHoldC : public DPIOPortC<DataT,DataT> 
  {
  public:
    DPHoldC()
      : DPEntityC(*new DPHoldBodyC<DataT>())
    {}
    //: Default constructor.
  
    DPHoldC(const DataT & init)
      : DPEntityC(*new DPHoldBodyC<DataT>(init))
    {}
    //: Constructor.
  
    inline DPHoldBodyC<DataT> &Body() 
    { return dynamic_cast<DPHoldBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
  
    inline const DPHoldBodyC<DataT> &Body() const
    { return dynamic_cast<const DPHoldBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
  
    inline void Set(const DataT &dat)
    { Body().Set(dat); }
    //: Set holding value.
  
    inline DataT Data() const
    { return Body().Data(); }
    //: Get data directly.
  
  };

  //////////////////////////////

  //: Shortcut for creating threads.

  template<class DataT>
  DPHoldBodyC<DataT> DPHold(DataT &init)
  { return DPHoldC<DataT>(init); }

  ///////////////////////////////////////////

  template<class DataT>
  DPHoldBodyC<DataT>::DPHoldBodyC(const DataT &init)
    : gotEOS(false),
      hold(init)
  {}

  template<class DataT>
  bool 
  DPHoldBodyC<DataT>::IsPutReady() const 
  { return !gotEOS; }

  template<class DataT>
  bool
  DPHoldBodyC<DataT>::Put(const DataT &dat) 
  { 
    if(gotEOS) 
      return false; 
    MutexLockC lock(access);
    hold = dat;
    return true; 
  }

  template<class DataT>
  IntT DPHoldBodyC<DataT>::PutArray(const SArray1dC<DataT> &data) {
    if(gotEOS) 
      return 0;
    if(data.Size() == 0)
      return 0;
    MutexLockC lock(access);
    hold = data[data.Size()-1];
    return data.Size();
  }

  template<class DataT>
  inline 
  void DPHoldBodyC<DataT>::Set(const DataT &dat) 
  {
    MutexLockC lock(access); 
    hold = dat; 
  }

  template<class DataT>
  inline 
  DataT DPHoldBodyC<DataT>::Data() const 
  {
    MutexLockC lock(access); 
    return hold; 
  }

  template<class DataT>
  void 
  DPHoldBodyC<DataT>::PutEOS()
  { gotEOS = true; }
  
  template<class DataT>
  bool 
  DPHoldBodyC<DataT>::IsGetReady() const 
  { return (!gotEOS); }

  template<class DataT>
  DataT 
  DPHoldBodyC<DataT>::Get() 
  { 
    MutexLockC lock(access);
    return hold; 
  }

  template<class DataT>
  bool DPHoldBodyC<DataT>::Get(DataT &buff)
  {
    MutexLockC lock(access);
    buff = hold;
    return true;
  }

  template<class DataT>
  IntT DPHoldBodyC<DataT>::GetArray(SArray1dC<DataT> &data) {
    MutexLockC lock(access);
    for(SArray1dIterC<DataT> it(data);it;it++)
      *it = hold;
    return data.Size();
  }

  template<class DataT>
  RCBodyVC &
  DPHoldBodyC<DataT>::Copy() const 
  { 
    MutexLockC lock(access);
    return *new DPHoldBodyC<DataT>(hold); 
  }

}
//////////////////////////////////////////////////////

#endif
