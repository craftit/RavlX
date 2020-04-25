// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPGOVERNOR_HEADER
#define RAVL_DPGOVERNOR_HEADER
///////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Extras"
//! author="Charles Galambos"
//! date="12/10/98"
//! lib=RavlDPMT
//! file="Ravl/OS/DataProc/Governor.hh"

#include "Ravl/DP/StreamOp.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Threads/ThreadEvent.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Governor base body.
  
  class DPGovernorBaseBodyC 
    : virtual public DPEntityBodyC
  {
  public:
    DPGovernorBaseBodyC(double ndelay,RealT nMinDelay = 0)
      : delay(ndelay),
	frameCnt(0),
	minDelay(nMinDelay),
  m_persistBypass(false)
    {}
    //: Constructor
    
    DPGovernorBaseBodyC(const DPGovernorBaseBodyC &oth) 
      : delay(oth.delay),
	frameCnt(0),
	minDelay(0),
  m_persistBypass(false)
    {}
    //: Copy Constructor
    
    UIntT FrameCount() const { return frameCnt; }
    //: Access frame count.
    
    RealT Delay() const { return delay; }
    //: Access frame count.
    
    RealT &Delay() { return delay; }
    //: Access frame count.
    
    RealT MinDelay() const { return minDelay; }
    //: Access frame count.
    
    RealT &MinDelay() { return minDelay; }
    //: Access frame count.
    
    bool SetDelay(RealT newDelay);
    //: Set new delay.
    // newDelay must be positive or zero

    bool Bypass(bool bypass, bool persist);
    //; Bypass the governor
    // bypass is cleared after next get, if persist is not true.
    
  protected:
    void WaitForTimeup();
    //: Wait for timeup.
    
    DateC next;
    RealT delay;
    UIntT frameCnt; // Frame count,can be used to measure frame rate.
    RealT minDelay; // Minimum delay to insert.
    ThreadEventC m_bypass; //Bypass the current waiting period
    volatile bool m_persistBypass; // bypass until further notice.
  };
   
  ////////////////////////////////////
  //! userlevel=Normal
  //: Governor base Handle.
  // Stream independant control of frame rate.
  
  class DPGovernorBaseC 
    : virtual public DPEntityC
  {
  public:
    DPGovernorBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPGovernorBaseC(DPGovernorBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body Constructor
    
    DPGovernorBaseC(const DPGovernorBaseC &bod)
      : DPEntityC(bod)
    {}
    //: Copy Constructor
    
  protected:
    DPGovernorBaseBodyC &Body() 
    { return dynamic_cast<DPGovernorBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPGovernorBaseBodyC &Body() const 
    { return dynamic_cast<const DPGovernorBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    UIntT FrameCount() const 
    { return Body().FrameCount(); }
    //: Access frame count.
    
    RealT Delay() const 
    { return Body().Delay(); }
    //: Access delay
    
    RealT &Delay()
    { return Body().Delay(); }
    //: Access delay

    RealT MinDelay() const 
    { return Body().MinDelay(); }
    //: Access delay
    
    RealT &MinDelay()
    { return Body().MinDelay(); }
    //: Access delay
    
    bool SetDelay(RealT newDelay)
    { return Body().SetDelay(newDelay); }
    //: Set new delay.
    // newDelay must be positive or zero

    bool Bypass(bool bypass, bool persist=false)
    { return Body().Bypass(bypass, persist); }
    //; Bypass the governor
    // bypass is cleared after next get, if persist is not true.

    
  };
  
  ///////////////////////////
  //! userlevel=Develop
  //: Governor body.
  
  template<class DataT>
  class DPGovernorBodyC 
    : public DPIStreamOpBodyC<DataT,DataT>,
      public DPGovernorBaseBodyC 
  {
  public:
    DPGovernorBodyC(double ndelay,RealT nMinDelay = 0)
      : DPGovernorBaseBodyC(ndelay,nMinDelay)
    {}
    //: Constructor
    
    DPGovernorBodyC(const DPGovernorBodyC<DataT> &oth) 
      : DPIStreamOpBodyC<DataT,DataT>(oth),
	DPGovernorBaseBodyC(oth)
    {}
    //: Copy Constructor

    virtual bool Save(ostream &out) const 
    { return DPGovernorBaseBodyC::Save(out); }
    //: Save to ostream.

    virtual DataT Get() { 
      DataT ret = this->input.Get(); 
      WaitForTimeup();
      return ret;
    }
    //: Process next piece of data.
    
    virtual bool Get(DataT &outbuff) { 
      bool ret = this->input.Get(outbuff);
      WaitForTimeup();
      return ret;
    }
    //: Process some data.  
    
    virtual bool IsGetReady() const {
      RavlAssert(this->input.IsValid());
      return this->input.IsGetReady(); 
    }
    
    virtual bool IsGetEOS() const { 
      RavlAssert(this->input.IsValid());
      return this->input.IsGetEOS(); 
    }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual RCBodyVC &Copy() const 
    { return *new DPGovernorBodyC(*this); }
    //: Creat a copy of this object.
    
  };
  
  ////////////////////////////////////
  //! userlevel=Normal
  //: Governor Handle.
  // This class limits the minimum time between
  // get operations.  This is used for things 
  // like control  the frame rate of a video 
  // sequence.
  
  template<class DataT>
  class DPGovernorC 
    : public DPIStreamOpC<DataT,DataT>,
      public DPGovernorBaseC 
  {
  public:
    DPGovernorC(double ndelay,RealT nminDelay = 0)
      : DPEntityC(*new DPGovernorBodyC<DataT>(ndelay,nminDelay))
    {}
    //: Constructor  

    bool Save(ostream &out) const 
    { return DPGovernorBaseC::Save(out); }
    //: Save to ostream.
  };
  
}

#endif
