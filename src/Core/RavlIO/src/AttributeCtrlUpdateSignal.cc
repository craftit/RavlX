// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlIO

#include "Ravl/DP/AttributeCtrlUpdateSignal.hh"

namespace RavlN {

  //: Destructor.
  
  AttributeCtrlUpdateSignalC::~AttributeCtrlUpdateSignalC() {
    if(ctrl.IsValid() && funcId > 0)
      ctrl.RemoveChangedSignal(funcId);
  }
  
  //: Change attribute control to list for update from.
  //!param: newCtrl - New attribute control to listen to.
  
  bool AttributeCtrlUpdateSignalC::ChangeControl(const AttributeCtrlC &newCtrl) {
    // Disconnect any existing trigger.
    if(ctrl.IsValid() && funcId > 0)
      ctrl.RemoveChangedSignal(funcId);
    // Write up new one.
    ctrl = newCtrl;
    funcId = -1;
    if(ctrl.IsValid() && trigger.IsValid()) {
      funcId = ctrl.RegisterChangedSignal(name,trigger);
      return funcId >= 0;
    }
    return true;
  }
  
  //: Connect a new trigger method.
  // This dissconnects any existing function.
  
  bool AttributeCtrlUpdateSignalC::Connect(const StringC &sigName,const TriggerC &ntrigger) {
    // Disconnect any existing trigger.
    if(ctrl.IsValid() && funcId > 0)
      ctrl.RemoveChangedSignal(funcId);
    name = sigName;
    trigger = ntrigger;
    funcId = -1;
    if(ctrl.IsValid() && trigger.IsValid()) {
      funcId = ctrl.RegisterChangedSignal(name,trigger);
      return funcId >= 0;
    }
    return true;
  }

  
  //: Connect a new trigger method.
  // This dissconnects any existing function.
  
  bool AttributeCtrlUpdateSignalC::Connect(const AttributeCtrlC &newCtrl,const StringC &sigName,const TriggerC &ntrigger) {
    // Disconnect any existing trigger.
    if(ctrl.IsValid() && funcId > 0)
      ctrl.RemoveChangedSignal(funcId);
    // Connect new function.
    name = sigName;
    trigger = ntrigger;
    ctrl = newCtrl;
    funcId = -1;
    if(ctrl.IsValid() && trigger.IsValid()) {
      funcId = ctrl.RegisterChangedSignal(name,trigger);
      return funcId >= 0;
    }
    return true;
  }
    

  
}
