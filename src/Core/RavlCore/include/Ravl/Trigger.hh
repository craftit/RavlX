// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TRIGGER_HEADER
#define RAVL_TRIGGER_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Base/Trigger.hh"
//! lib=RavlCore
//! userlevel=Default
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Core.Calls"
//! example=exTrigger.cc

#include "Ravl/RCHandleV.hh"


namespace RavlN {
  //! userlevel=Develop
  typedef void (*VoidFuncPtrT)();
  //: Void function pointer.
  
  //:-
  
  //! userlevel=Develop
  //: Body for TriggerC

  
  class TriggerBodyC
    : public RCBodyVC
  {
  public:
    TriggerBodyC()
    {}
    //: Default constructor.
    
    virtual void Invoke();
    //: Trigger event.
  };
  
  //! userlevel=Normal
  //: Parent class for function / method callers
  // Use when no arguments or return values are needed at the point of calling
  
  class TriggerC
    : public RCHandleVC< TriggerBodyC>
  {
  public:
    TriggerC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    TriggerC(const TriggerBodyC *bod)
      : RCHandleVC<TriggerBodyC>(bod)
    {}
    //: Body constructor.

  protected:
    TriggerC(TriggerBodyC &bod) 
      : RCHandleVC<TriggerBodyC>(bod)
    {}
    //: Body constructor.
    
    TriggerBodyC &Body()
    { return  RCHandleC<TriggerBodyC>::Body(); }
    //: Access body.
    
    const TriggerBodyC &Body() const
    { return RCHandleC<TriggerBodyC>::Body(); }
    //: Access body.
    
  public:
    void Invoke()
    { Body().Invoke(); }
    //: Invoke the method.
    
    void operator()()
    { Body().Invoke(); }
    //: Invoke the method.
    
  };



}

#endif
