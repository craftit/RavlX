// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TICKERTRIGGER_HEADER
#define RAVL_TICKERTRIGGER_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Threads/Tools/TickerTrigger.hh"
//! lib=RavlThreads
//! userlevel=Default
//! docentry="Ravl.API.OS.Time"
//! author="Charles Galambos"
//! date="02/07/1999"

#include "Ravl/OS/Date.hh"
#include "Ravl/Threads/Thread.hh"
#include "Ravl/Calls.hh"
#include "Ravl/CallMethods.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Ticker body.
  // This class triggers a method at repeatedly with a fixed delay between calls.
  
  class TickerTriggerBodyC 
    : public ThreadBodyC
  {
  public:
    TickerTriggerBodyC(RealT ndelay,const TriggerC &nse)
      : delay(ndelay),
	se(nse) 
      {}
    //: Let rip !
    
    virtual int Start();
    //: Called on startup.
    
    virtual void Terminate();
    //: Thread shutdown.
    // Provide a clean way of shuting down this thread.
    
    void SendTerminate()
    { delay = -1; }
    //: Set terminate.
    // Obsolete use Shutdown()
    
    void Shutdown()
    { delay = -1; }
    //: Shutdown ticker.
    
    bool SetDelay(RealT ndelay) { 
      delay = ndelay; 
      return true; 
    }
    //: Set delay between events.
    // Will be used for the wait following the current one.
    
  protected:
    volatile RealT delay;
    DateC next;
    TriggerC se;
  };
  
  //! userlevel=Normal
  //: Ticker handle.
  // This class triggers a method at repeatedly with a fixed delay between calls.
  
  class TickerTriggerC
    : public ThreadC
  {
  public:
    TickerTriggerC()
      {}
    //: Default constructor.
    
    TickerTriggerC(RealT ndelay,const TriggerC &nse)
      : ThreadC(*new TickerTriggerBodyC(ndelay,nse))
      { Execute(); }
    //: Constructor.
    
  protected:
    TickerTriggerBodyC &Body()
      { return static_cast<TickerTriggerBodyC &>(ThreadC::Body()); }
    //: Access body.
    
    const TickerTriggerBodyC &Body() const
      { return static_cast<const TickerTriggerBodyC &>(ThreadC::Body()); }
    //: Access body.
    
  public:
    void SendTerminate() 
    { Body().SendTerminate(); }
    //: Send terminate.
    // Obsolete use Shutdown()
    
    void Shutdown() 
    { Body().SendTerminate(); }
    //: Send terminate.
    
    bool SetDelay(RealT delay)
    { return Body().SetDelay(delay); }
    //: Set delay between events.
    
  };
  
  ////////////////////////////////////////////////////////////////
  
  inline 
  TickerTriggerC TickerTrigger(RealT ndelay,const TriggerC &nse)
  { return TickerTriggerC(ndelay,nse); }
  
  inline
  TickerTriggerC TickerTrigger(RealT ndelay,bool (*nFunc)())
  { return TickerTriggerC(ndelay,Trigger(nFunc)); }

  template<class DataT>
  TickerTriggerC TickerTrigger(RealT ndelay,bool (*nFunc)(DataT &dat),const DataT &dat)
  { return TickerTriggerC(ndelay,Trigger(nFunc,dat)); }
  
  template<class ObjT>
  TickerTriggerC TickerTrigger(RealT ndelay,const ObjT &nObj,bool (ObjT::*nFunc)())
  { return TickerTriggerC(ndelay,Trigger(nObj,nFunc)); }
  
  template<class ObjT,class DataT>
  TickerTriggerC TickerTrigger(RealT ndelay,const ObjT &nObj,bool (ObjT::*nFunc)(DataT &),const DataT &nDat)
  { return TickerTriggerC(ndelay,Trigger(nObj,nFunc,nDat)); }
  
  template<class ObjT,class Data1T,class Data2T>
  TickerTriggerC TickerTrigger(RealT ndelay,const ObjT &nObj,bool (ObjT::*nFunc)(Data1T &,Data2T &),const Data1T &nDat1,const Data2T &nDat2)
  { return TickerTriggerC(ndelay,Trigger(nObj,nFunc,nDat1,nDat2)); }
  
}

#endif
