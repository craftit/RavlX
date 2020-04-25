// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlDPMT

#include "Ravl/DP/TailIStream.hh"
#include "Ravl/CallMethodRefs.hh"

namespace RavlN {
  
  //! Constructor.
  TailStreamBaseC::TailStreamBaseC()
    : m_sizeUpdate(0),
      m_terminate(false),
      m_updateSigId(-1)
  {
  }
  
  //! Destructor.
  
  TailStreamBaseC::~TailStreamBaseC()
  {
  }
  
  //: Stop tailing the stream.
  
  bool TailStreamBaseC::Terminate() {
    m_terminate = true;
    m_sizeUpdate.Post();
    return true;
  }
  
  //: Initailse 
  
  void TailStreamBaseC::Init() {
    AttributeCtrlBodyC &attrCtrl = dynamic_cast<AttributeCtrlBodyC &>(*this);
    m_updateSigId = attrCtrl.RegisterChangedSignal("size",TriggerR(*this,&TailStreamBaseC::HandleUpdate));
  }
  
  
  //! Process an update.
  
  bool TailStreamBaseC::HandleUpdate() {
    DPSeekCtrlBodyC &seekCtrl = dynamic_cast<DPSeekCtrlBodyC &>(*this);
    Int64T newSize = seekCtrl.Size64();
    if(newSize > m_currentSize) {
      m_sizeUpdate.Post();
    }
    m_currentSize = newSize;
    return true;
  }
  

}
