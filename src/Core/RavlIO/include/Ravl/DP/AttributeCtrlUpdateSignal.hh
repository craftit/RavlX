// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ATTRIBUTECTRLUPDATESIGNAL_HEADER
#define RAVL_ATTRIBUTECTRLUPDATESIGNAL_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data_Processing.Attributes"
//! date="2/4/2005"
//! rcsid="$Id$"
//! lib=RavlIO

#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/Trigger.hh"

namespace RavlN {

  //! userlevel=Normal
  //: Helper to make it easier to manage update signal's from attribute ctrl's
  // This provides the following functionality. <br>
  // - Ensure trigger is disconnected on exit. <br>
  // - Method for switching the AttributeCtrl we' listening to. <br>
  // - Prevents potential bugs with copy constructor's or assigment operators. <br>
  
  class AttributeCtrlUpdateSignalC {
  public:
    AttributeCtrlUpdateSignalC()
      : funcId(-1)
    {}
    //: Default constructor.
    
    AttributeCtrlUpdateSignalC(const AttributeCtrlC &newCtrl,const StringC &sigName,const TriggerC &ntrigger)
      : name(sigName),
        ctrl(newCtrl),
        trigger(ntrigger),
        funcId(-1)
    { 
      if(ctrl.IsValid())
        funcId = ctrl.RegisterChangedSignal(name,trigger); 
    }
    //: Constructor.
    //!param: newCtrl - Attribute control to listen to.
    //!param: sigName - Name of signal to attach the trigger to.
    //!param: ntrigger- New trigger to call.

    AttributeCtrlUpdateSignalC(const StringC &sigName,const TriggerC &ntrigger)
      : name(sigName),
        trigger(ntrigger),
        funcId(-1)
    {}
    //: Constructor.
    //!param: sigName - Name of signal to attach the trigger to.
    //!param: ntrigger- New trigger to call.
    
    ~AttributeCtrlUpdateSignalC();
    //: Destructor.
    
    bool Connect(const AttributeCtrlC &newCtrl,const StringC &sigName,const TriggerC &ntrigger);
    //: Connect a new trigger method and new attribute control.
    // This dissconnects any existing function.
    
    bool Connect(const StringC &sigName,const TriggerC &ntrigger);
    //: Connect a new trigger method.
    // This dissconnects any existing function.
    
    bool ChangeControl(const AttributeCtrlC &newCtrl);
    //: Change attribute control to list for update from.
    //!param: newCtrl - New attribute control to listen to.
    
  protected:
    StringC name;
    AttributeCtrlC ctrl;
    TriggerC trigger;
    IntT funcId;
    
  private:
    AttributeCtrlUpdateSignalC(const AttributeCtrlUpdateSignalC &)
    { RavlAssert(0); }
    //: Hide copy constructor.
    
    const AttributeCtrlUpdateSignalC &operator=(const AttributeCtrlUpdateSignalC &)
    { RavlAssert(0); return *this; }
    //: Don't use assigment either.
  };
}

#endif
