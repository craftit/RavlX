// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGUI

#include "Ravl/GUI/TriggerAction.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlGUIN {

  //: XML factory constructor
  TriggerShowWidgetBodyC::TriggerShowWidgetBodyC(const XMLFactoryContextC &context)
  {
    context.UseComponent("TargetWidget",m_targetWidget);
  }

  void TriggerShowWidgetBodyC::Invoke() {
    m_targetWidget.Show();
  }

  static XMLFactoryRegisterHandleConvertC<TriggerShowWidgetC,TriggerC> g_registerXMLFactoryShowWidget("RavlGUIN::TriggerShowWidgetC");

  // ------------------------------------------------------------------------

  TriggerHideWidgetBodyC::TriggerHideWidgetBodyC(const XMLFactoryContextC &context)
  {
    context.UseComponent("TargetWidget",m_targetWidget);
  }
  //: XML factory constructor

  void TriggerHideWidgetBodyC::Invoke() {
    m_targetWidget.Hide();
  }

  static XMLFactoryRegisterHandleConvertC<TriggerHideWidgetC,TriggerC> g_registerXMLFactoryHideWidget("RavlGUIN::TriggerHideWidgetC");

  void LinkTriggerAction()
  {}
}
