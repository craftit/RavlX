// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/DeinterlaceStream.cc"


#include "Ravl/Image/DeinterlaceStream.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

namespace RavlImageN {
  
  //: Constructor
  
  DeinterlaceStreamBaseC::DeinterlaceStreamBaseC(const DPSeekCtrlC &newSeekCtrl,bool evenFieldDom) 
    : seekCtrl(newSeekCtrl),
      evenFieldDominant(evenFieldDom),
      state(0)
  {}
  
  //: Destructor.
  
  DeinterlaceStreamBaseC::~DeinterlaceStreamBaseC() {
  }
  
  //: Register attributes.
  
  void DeinterlaceStreamBaseC::RegisterAttribs(AttributeCtrlC &attrCtrl) {
    attrCtrl.RegisterAttribute(AttributeTypeBoolC("even_field_dominance","If true the even field is decoded first",true,true,true));
  }
  
  //: Get bool attribute.
  
  bool DeinterlaceStreamBaseC::HandleGetAttr(const StringC &attrName,bool &attrValue) {
    if(attrName=="even_field_dominance") {
      attrValue = evenFieldDominant;
      return true;
    }
    return false;
  }
  
  //: Set bool attribute.
  
  bool DeinterlaceStreamBaseC::HandleSetAttr(const StringC &attrName,const bool &attrValue) {
    if(attrName=="even_field_dominance") {
      evenFieldDominant = attrValue;
      return true;
    }
    return false;
  }
  
  //: Get a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DeinterlaceStreamBaseC::HandleGetAttr(const StringC &attrName,RealT &attrValue) {
    if(attrName == "framerate") {
      StringC tmp;
      if(!inputBase.GetAttr(attrName,tmp)) {
	attrValue = 50; // Guess at 50 Hz.
	return true;
      }
      // Double value before returning.
      RealT fr = tmp.RealValue();
      attrValue = fr * 2.0;
      return true;
    }
    // Just pass request back.
    return false;
  }
    
  //: Set a stream attribute.
  // Returns false if the attribute name is unknown.
  // This is for handling stream attributes such as frame rate, and compression ratios.
  
  bool DeinterlaceStreamBaseC::HandleSetAttr(const StringC &attrName,const RealT &attrValue) {
    if(attrName == "framerate") {
      // Half value.
      RealT fr = attrValue;
      RealT tmp = fr * 2.0;
	// Send it on back.
      return inputBase.SetAttr(attrName,tmp);
    }     
    // Just pass request back.
    return false;
  }
  
}
