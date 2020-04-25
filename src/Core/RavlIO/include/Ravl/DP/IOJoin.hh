// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPIOPORTJOIN_HEADER
#define RAVL_DPIOPORTJOIN_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/Core/IO/IOJoin.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Ports" 
//! date="12/10/1998"
//! rcsid="$Id$"
//! userlevel=Normal

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/IOPort.hh"

namespace RavlN {
  //! userlevel=Develop
  //: Join an IPort and a OPort
  
  template<class InT,class OutT>
  class DPIOPortJoinBodyC 
    : public DPIOPortBodyC<InT,OutT> 
  {
  public:
    DPIOPortJoinBodyC() 
      : hold(true)
      {}
    //: Default constructor.
    
    DPIOPortJoinBodyC(const DPIPortC<InT> &nin,const DPOPortC<OutT> &nout,const DPEntityC &ahold = DPEntityC(true))
      : in(nin),
	out(nout),
	hold(ahold)
    {}
    //: Constructor. 
    
    virtual bool IsGetReady() const 
      { return in.IsGetReady(); }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const 
      { return in.IsGetEOS(); }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual InT Get() 
      { return in.Get();  }
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual bool Get(InT &buff) 
      { return in.Get(buff); }
    //: Try and get next piece of data.
    // This will NOT block, if no data is ready
    // it will return false, and not set buff.
    
    virtual IntT GetArray(SArray1dC<InT> &data)
      { return in.GetArray(data); }
    //: Get an array of data.
    // returns the number of elements processed.
    
    virtual void PutEOS() 
      { out.PutEOS(); }
    //: Put End Of Stream marker.
    
    virtual bool IsPutReady() const 
      { return out.IsPutReady(); }
    //: Is port ready for data ?
    
    virtual bool Put(const OutT &dat) 
      { return out.Put(dat); }
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<OutT> &data)
      { return out.PutArray(data); }
    //: Put an array of data to stream.
    // returns the number of elements processed.
    
  protected:
    DPIPortC<InT> in;
    DPOPortC<OutT> out;
    DPEntityC hold; // User to hold structure ports may connect to.
  };
  
  //! userlevel=Advanced
  //: Join an IPort and a OPort
  
  template<class InT,class OutT>
  class DPIOPortJoinC 
    : public DPIOPortC<InT,OutT> 
  {
  public:
    DPIOPortJoinC() 
      : DPEntityC(true)
      {}
  //: Default constructor.  
    
    DPIOPortJoinC(const DPIPortC<InT> &in,const DPOPortC<OutT> &out,const DPEntityC &ahold = DPEntityC(true))
      : DPEntityC(*new DPIOPortJoinBodyC<InT,OutT>(in,out,ahold))
      {}
    //: Constructor.  
    
  };
  
  //: Join function.
  
  template<class InT,class OutT>
  DPIOPortC<InT,OutT> DPIOPortJoin(const DPIPortC<InT> &in,const DPOPortC<OutT> &out,const DPEntityC &ahold = DPEntityC(true))
  { return DPIOPortJoinC<InT,OutT>(in,out,ahold); }
}
  
#endif
