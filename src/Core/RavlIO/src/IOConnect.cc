// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/IOConnect.cc"

#include "Ravl/DP/IOConnect.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

namespace RavlN {

  //: Default constructor.
  
  DPIOConnectBaseBodyC::DPIOConnectBaseBodyC()
    : running(false)
  {
    RegisterAttribute(AttributeTypeBoolC("running","Pump is processing data.",true,true,false));
  }
  
  //: Op type name.
  
  StringC DPIOConnectBaseBodyC::OpName() const
  { return StringC("pump"); }
  
  //: Run until a stream completes.
  
  bool DPIOConnectBaseBodyC::Run() {
    RavlAssertMsg(0,"DPIOConnectBaseBodyC::Run(), Abstract method called.");
    return false;
  }

  //: Attempt to stop stream processing.
  
  bool DPIOConnectBaseBodyC::Stop() {
    return false;
  }
  
  //: Do a single processing step.
  
  bool DPIOConnectBaseBodyC::Step() {
    RavlAssertMsg(0,"DPIOConnectBaseBodyC::Step(), Abstract method called.");
    return false;
  }
  
  //: Check if we're ready to run.
  
  bool DPIOConnectBaseBodyC::IsReady() const {
    RavlAssertMsg(0,"DPIOConnectBaseBodyC::IsReady(), Abstract method called.");
    return false;
  }
  
  //: Get a attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling attributes such as frame rate, and compression ratios.
  
  bool DPIOConnectBaseBodyC::GetAttr(const StringC &attrName,bool &attrValue) {
    if(attrName == "running") {
      attrValue = running;
      return true;
    }
    return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Set a attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling attributes such as frame rate, and compression ratios.
  
  bool DPIOConnectBaseBodyC::SetAttr(const StringC &attrName,const bool &attrValue) {
    if(attrName == "running") {
      if(attrValue) {
	if(!running)
	  if(Run())
	    SignalChange("running");
      } else {
	if(running)
	  if(Stop())
	    SignalChange("running");
      }
      return true;
    }
    return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
  }
  
  //: Get a attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling attributes such as frame rate, and compression ratios.
  
  bool DPIOConnectBaseBodyC::GetAttr(const StringC &attrName,StringC &attrValue) {
    return AttributeCtrlBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Set a attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling attributes such as frame rate, and compression ratios.
  
  bool DPIOConnectBaseBodyC::SetAttr(const StringC &attrName,const StringC &attrValue) {
    return AttributeCtrlBodyC::SetAttr(attrName,attrValue);
  }
  
}
