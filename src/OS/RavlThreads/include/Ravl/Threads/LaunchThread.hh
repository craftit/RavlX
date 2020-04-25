// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_THREADLAUNCH_HEADER
#define RAVL_THREADLAUNCH_HEADER 1
////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/OS/Threads/Tools/LaunchThread.hh"
//! lib=RavlThreads
//! userlevel=Default
//! docentry="Ravl.API.OS.Threads"
//! author="Charles Galambos"
//! date="02/07/1999"
//! example=exLaunchThread.cc 

#include "Ravl/Threads/Thread.hh"
#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/Calls.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/CallMethodRefs.hh"

namespace RavlN {
  class LaunchThreadC;
  
  //! userlevel=Develop
  //: Launch event body.
  
  class LaunchThreadBodyC 
    : public ThreadBodyC
  {
  public:
    LaunchThreadBodyC(const TriggerC &nse);
    //: Constructor.
    
    LaunchThreadBodyC(SizeT initStackSize,const TriggerC &nse);
    //: Constructor.
    
    virtual int Start();
    //: Called on startup.
    
    ThreadEventC &Done()
    { return done; }
    //: Access done event.

    TriggerC &SigEvent()
    { return se; }
    //: Event to call.
    // NB. This is used by the child thread
    // and should not be modified until after the event 'done'
    // has occured.
    
  protected:    
    void Reset(const TriggerC &nse);
    //: Reset signal event.
    // Setup new event....
    
    TriggerC se;
    //: Event to call.
    
    ThreadEventC done;
    //: Signal when done.
    
    ThreadEventC reStart;
    //: Signal to restart.
    
    friend class LaunchThreadC;
  };
  
  //! userlevel=Normal
  //: Launch event handle.
  
  class LaunchThreadC
    : public ThreadC
  {
  public:
    LaunchThreadC()
    {}
    //: Default constructor.
    
    LaunchThreadC(const TriggerC &nse);
    //: Constructor.
    
    LaunchThreadC(SizeT initStackSize,const TriggerC &nse);
    //: Constructor.
    
  protected:
    LaunchThreadC(LaunchThreadBodyC &leb)
      : ThreadC(leb)
    {}
    //: Body constructor.
    
    LaunchThreadBodyC &Body()
    { return static_cast<LaunchThreadBodyC &>(ThreadC::Body()); }
    //: Access thread body.
    
    void Reset(const TriggerC &nse)
    { Body().Reset(nse); }
    //: Reset signal event.
    // Setup new event....
    
  public:
    ThreadEventC &Done()
    { return Body().Done(); }
    //: Access done event.
    // This is true when launched event completes.
    
    TriggerC &SigEvent()
    { return Body().SigEvent(); }
    //: Event to call.
    // NB. This is used by the child thread
    // and should not be modified until after the event 'done'
    // has occured.
    
    bool WaitForExit() { 
      Body().Done().Wait(); 
      return true;
    }
    //: Wait for thread to exit.
    
    friend class LaunchThreadBodyC;
  };
  
  ////////////////////////////////////////////////////////////////

  //! userlevel=Normal
  
  inline 
  LaunchThreadC LaunchThread(const TriggerC &nse)
  { return LaunchThreadC(nse); }
  //: Launch a routine on another thread.

  inline 
  LaunchThreadC LaunchThread(SizeT stackSize,const TriggerC &nse)
  { return LaunchThreadC(stackSize,nse); }
  //: Launch a routine on another thread.
  
  inline
  LaunchThreadC LaunchThread(bool (*nFunc)())
  { return LaunchThreadC(Trigger(nFunc)); }
  //: Launch a routine on another thread.

  template<class DataT>
  inline
  LaunchThreadC LaunchThread(bool (*nFunc)(DataT),
                const typename TraitsC<DataT>::BaseTypeT &def =  TraitsC<DataT>::BaseTypeT())
  { return LaunchThreadC(Trigger(nFunc,def)); }
  //: Launch a routine on another thread.

  template<class Data1T,class Data2T>
  inline
  LaunchThreadC LaunchThread(bool (*nFunc)(Data1T,Data2T),
                const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
		const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT())
  { return LaunchThreadC(Trigger(nFunc,def1,def2)); }
  //: Launch a routine on another thread.
 
  template<class Data1T,class Data2T,class Data3T>
  inline
  LaunchThreadC LaunchThread(bool (*nFunc)(Data1T,Data2T,Data3T),
                const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
                const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data1T>::BaseTypeT(),
                const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data1T>::BaseTypeT())
  { return LaunchThreadC(Trigger(nFunc,def1,def2,def3)); }
  //: Launch a routine on another thread.
  
  template<class ObjT>
  LaunchThreadC LaunchThread(const ObjT &nObj,bool (ObjT::*nFunc)())
  { return LaunchThreadC(Trigger(nObj,nFunc)); }
  //: Launch a method on another thread.
  // This calls a method on a copy of object 'nObj'.
  
  template<class ObjT,class DataT>
  LaunchThreadC LaunchThread(const ObjT &nObj,bool (ObjT::*nFunc)(DataT),
                const typename TraitsC<DataT>::BaseTypeT &def =  TraitsC<DataT>::BaseTypeT()) 
  { return LaunchThreadC(Trigger(nObj,nFunc,def)); }
  //: Launch a method on another thread.
  // This calls a method on a copy of object 'nObj'.
  
  template<class ObjT,class Data1T,class Data2T>
  LaunchThreadC LaunchThread(const ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT()) 
  { return LaunchThreadC(Trigger(nObj,nFunc,def1,def2)); }
  //: Launch a method on another thread.
  // This calls a method on a copy of object 'nObj'.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T>
  LaunchThreadC LaunchThread(const ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T,Data3T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT(),
    const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data3T>::BaseTypeT()) 
  { return LaunchThreadC(Trigger(nObj,nFunc,def1,def2,def3)); }
  //: Launch a method on another thread.
  // This calls a method on a copy of object 'nObj'.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
  LaunchThreadC LaunchThread(const ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T,Data3T,Data4T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT(),
    const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data3T>::BaseTypeT(),
    const typename TraitsC<Data4T>::BaseTypeT &def4 =  TraitsC<Data4T>::BaseTypeT())
  { return LaunchThreadC(Trigger(nObj,nFunc,def1,def2,def3,def4)); }
  //: Launch a method on another thread.
  // This calls a method on a copy of object 'nObj'.

  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T>
  LaunchThreadC LaunchThread(const ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT(),
    const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data3T>::BaseTypeT(),
    const typename TraitsC<Data4T>::BaseTypeT &def4 =  TraitsC<Data4T>::BaseTypeT(),
    const typename TraitsC<Data5T>::BaseTypeT &def5 =  TraitsC<Data5T>::BaseTypeT())
  { return LaunchThreadC(Trigger(nObj,nFunc,def1,def2,def3,def4,def5)); }
  //: Launch a method on another thread.
  // This calls a method on a copy of object 'nObj'.
  
  template<class ObjT>
  LaunchThreadC LaunchThreadR(ObjT &nObj,bool (ObjT::*nFunc)())
  { return LaunchThreadC(TriggerR(nObj,nFunc)); }
  //: Launch a method on another thread.
  // This calls a method on a reference to object 'nObj'.
  
  template<class ObjT,class DataT>
  LaunchThreadC LaunchThreadR(ObjT &nObj,bool (ObjT::*nFunc)(DataT),
    const typename TraitsC<DataT>::BaseTypeT &def =  TraitsC<DataT>::BaseTypeT()) 
  { return LaunchThreadC(TriggerR(nObj,nFunc,def)); }
  //: Launch a method on another thread.
  // This calls a method on a reference to object 'nObj'.
  
  template<class ObjT,class Data1T,class Data2T>
  LaunchThreadC LaunchThreadR(ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT()) 
  { return LaunchThreadC(TriggerR(nObj,nFunc,def1,def2)); }
  //: Launch a method on another thread.
  // This calls a method on a reference to object 'nObj'.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T>
  LaunchThreadC LaunchThreadR(ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T,Data3T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT(),
    const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data3T>::BaseTypeT()) 
  { return LaunchThreadC(TriggerR(nObj,nFunc,def1,def2,def3)); }
  //: Launch a method on another thread.
  // This calls a method on a reference to object 'nObj'.
  
  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
  LaunchThreadC LaunchThreadR(ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T,Data3T,Data4T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT(),
    const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data3T>::BaseTypeT(),
    const typename TraitsC<Data4T>::BaseTypeT &def4 =  TraitsC<Data4T>::BaseTypeT())
  { return LaunchThreadC(TriggerR(nObj,nFunc,def1,def2,def3,def4)); }
  //: Launch a method on another thread.
  // This calls a method on a reference to object 'nObj'.

  template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T,class Data5T>
  LaunchThreadC LaunchThreadR(ObjT &nObj,bool (ObjT::*nFunc)(Data1T,Data2T,Data3T,Data4T,Data5T),
    const typename TraitsC<Data1T>::BaseTypeT &def1 =  TraitsC<Data1T>::BaseTypeT(),
    const typename TraitsC<Data2T>::BaseTypeT &def2 =  TraitsC<Data2T>::BaseTypeT(),
    const typename TraitsC<Data3T>::BaseTypeT &def3 =  TraitsC<Data3T>::BaseTypeT(),
    const typename TraitsC<Data4T>::BaseTypeT &def4 =  TraitsC<Data4T>::BaseTypeT(),
    const typename TraitsC<Data1T>::BaseTypeT &def5 =  TraitsC<Data5T>::BaseTypeT())
  { return LaunchThreadC(TriggerR(nObj,nFunc,def1,def2,def3,def4,def5)); }
  //: Launch a method on another thread.
  // This calls a method on a reference to object 'nObj'.
  
}

#endif
