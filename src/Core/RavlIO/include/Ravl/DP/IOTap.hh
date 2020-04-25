// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPIOTAP_HEADER
#define RAVL_DPIOTAP_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Taps"
//! rcsid="$Id$"
//! file="Ravl/Core/IO/IOTap.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="20/07/1998"

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/IOPort.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  ////////////////////////////////////
  //! userlevel=Develop
  //: Tap body
  
  template<class InT,class OutT>
  class DPIOTapBodyC 
    : public DPIOPortBodyC<InT,OutT>
  {
  public:
    DPIOTapBodyC(const DPIOPortC<InT,OutT> &ntarget,const DPOPortC<Tuple2C<OutT,InT> > &ntap)
      : tap(ntap),
	target(ntarget),
        donePut(false)
    { RavlAssert(target.IsValid()); }
    //: Constructor.
    
    DPIOTapBodyC()
    {}
    //: Default Constructor.
    
    DPIOTapBodyC(std::istream &in)
      : tap(in),
	target(in)
    {}
    //: Stream constructor.
    
    virtual void PutEOS();
    //: Put End Of Stream marker.
    
    virtual bool Put(const OutT &);
    //: Put data.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
    virtual InT Get();
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual bool Get(InT &buff);
    //: Try and get next piece of data.
    // This will NOT block, if no data is ready
    // it will return false, and not set buff.
    
    virtual bool IsGetReady() const
    { return target.IsGetReady(); }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to std::ostream.
    
    inline DPOPortC<Tuple2C<OutT,InT> > &Tap()
    { return tap; }
    //: Access to tap.
    
    inline DPIOPortC<InT,OutT> &Target()
    { return target; }
    //: Access the object being tapped.
    
  protected:
    DPOPortC<Tuple2C<OutT,InT> > tap;
    DPIOPortC<InT,OutT> target;
    Tuple2C<OutT,InT> tapout;
    bool donePut;
  };
  
  ///////////////////////////////////////
  //! userlevel=Normal
  //: Tap handle.
  
  template<class InT,class OutT>
  class DPIOTapC 
    : public DPIOPortC<InT,OutT>
  {
  public:
    DPIOTapC(const DPIOPortC<InT,OutT> &ntarget,
	     const DPOPortC<Tuple2C<OutT,InT> > &ntap)
      : DPEntityC(*new DPIOTapBodyC<InT,OutT> (ntarget,ntap))
    {}
    //: Constructor.
    //= DPOPortC<Tuple2C<OutT,InT> > ()
    
    DPIOTapC()
      : DPEntityC(*new DPIOTapBodyC<InT,OutT> ())
    {}
    //: Default Constructor.
    
    DPIOTapC(std::istream &in)
      : DPEntityC(in)
    {}
    //: Stream constructor.
    
  protected:
    DPIOTapBodyC<InT,OutT> &Body()
    { return dynamic_cast<DPIOTapBodyC<InT,OutT> &>(DPEntityC::Body()); }
    
    const DPIOTapBodyC<InT,OutT> &Body() const
    { return dynamic_cast<const DPIOTapBodyC<InT,OutT> &>(DPEntityC::Body()); }

  protected:
    inline DPOPortC<Tuple2C<OutT,InT> > &Tap()
    { return Body().Tap(); }
    //: Access to tap.
    
    inline DPIOPortC<InT,OutT> &Target()
    { return Body().Target(); }
    //: Access the object being tapped.
    
    inline DPIOPortC<InT,OutT> &IOPort() 
    { return *this; }
    //: Simlified cast.

  };
  
  
  template<class InT,class OutT>
  void DPIOTapBodyC<InT,OutT>::PutEOS() { 
    target.PutEOS(); 
    tap.PutEOS();
  }
  
  template<class InT,class OutT>
  bool DPIOTapBodyC<InT,OutT>::Put(const OutT &dat) {
    RavlAssert(target.IsValid());
    if(donePut == true) {
      tapout.Data2() = InT();
      tap.Put(tapout);
    }
    tapout.Data1() = dat;
    donePut = true;
    return target.Put(dat); 
  }
  
  template<class InT,class OutT>
  bool DPIOTapBodyC<InT,OutT>::IsPutReady() const { 
    RavlAssert(target.IsValid());
    return target.IsPutReady(); 
  }
  
  template<class InT,class OutT>
  InT DPIOTapBodyC<InT,OutT>::Get() { 
    RavlAssert(target.IsValid());
    tapout.Data2() = target.Get();
    if(!donePut)
      tapout.Data1() = OutT();
    tap.Put(tapout);
    donePut=false;
    return tapout.Data2(); 
  }
  
  template<class InT,class OutT>
  bool DPIOTapBodyC<InT,OutT>::Get(InT &buff) { 
    RavlAssert(target.IsValid());
    bool ret = target.Get(buff); 
    if(ret) {
      tapout.Data2() = buff;
      if(!donePut)
	tapout.Data1() = OutT();
      tap.Put(tapout);
      donePut=false;
    }
    return ret;
  }
  
  template<class InT,class OutT>
  bool DPIOTapBodyC<InT,OutT>::Save(std::ostream &out) const {
    if(!DPIOPortBodyC<InT,OutT>::Save(out))
      return false;
    if(!tap.Save(out))
      return false;
    if(!target.Save(out))
      return false;
    return true;
  }
}
#endif
