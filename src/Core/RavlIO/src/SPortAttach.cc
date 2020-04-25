// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Algorithms.Data Processing.Composition" 
//! lib=RavlIO
//! file="Ravl/Core/IO/SPortAttach.cc"

#include "Ravl/DP/SPortAttach.hh"

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  DPSeekCtrlAttachBodyC::DPSeekCtrlAttachBodyC(const DPSeekCtrlC &oth)
    : sctrl(oth)
  {
    if(!sctrl.IsValid()) {
      sctrl = DPSeekCtrlC(true); // Put a silent stub in.
    }
  }
  
  //: Constructor
  
  DPSeekCtrlAttachBodyC::DPSeekCtrlAttachBodyC(const DPPortC &pb,bool smartDefault) {
    ONDEBUG(cerr << "DPSeekCtrlAttachBodyC::DPSeekCtrlAttachBodyC(const DPPortC &,bool), Called. \n");
    if(smartDefault) {
      DPPortC at = pb;
      while(at.IsValid()) {
	DPSeekCtrlC sc(at);
	if(sc.IsValid()) {
	  sctrl = sc;
	  break;
	}
	at = at.ConnectedTo();
      }
    } else // The stupid default..
      sctrl = DPSeekCtrlC(pb);
    
    if(!sctrl.IsValid()) // Found one ?
      sctrl = DPSeekCtrlC(true,pb); // If not put in a stub.
    
  } 
  
  
  //: Seek to location in stream.
  
  bool DPSeekCtrlAttachBodyC::Seek(UIntT off) { 
    //ONDEBUG(cerr << "DPSeekCtrlAttachBodyC::Seek(UIntT) called. \n");
    return sctrl.Seek(off); 
  }
  
  //: Delta Seek, goto location relative to the current one.
  
  bool DPSeekCtrlAttachBodyC::DSeek(IntT off)
  { return sctrl.DSeek(off); }

  
  //: Find current location in stream.
  
  UIntT DPSeekCtrlAttachBodyC::Tell() const
  { return sctrl.Tell(); }
  
  //: Find the total size of the stream.
  
  UIntT DPSeekCtrlAttachBodyC::Size() const
  { return sctrl.Size(); }
  
  //: Find the offset where the stream begins, normally zero.
  // Defaults to 0
  
  UIntT DPSeekCtrlAttachBodyC::Start() const
  { return sctrl.Start(); }

  //: Seek to location in stream.
  
  bool DPSeekCtrlAttachBodyC::Seek64(StreamPosT off) 
  { return sctrl.Seek64(off); }
  
  //: Seek to relative location in stream.
  
  bool DPSeekCtrlAttachBodyC::DSeek64(StreamPosT off) 
  { return sctrl.DSeek64(off); }

  //: Find current location in stream.
  
  StreamPosT DPSeekCtrlAttachBodyC::Tell64() const
  { return sctrl.Tell64(); }
  
  //: Find the total size of the stream. (assuming it starts from 0)
  
  StreamPosT DPSeekCtrlAttachBodyC::Size64() const 
  { return sctrl.Size64(); }
    
  //: Find the offset where the stream begins, normally zero.
  
  StreamPosT DPSeekCtrlAttachBodyC::Start64() const 
  { return sctrl.Start64();  }
  
  
  //: Set seel control.
  
  void DPSeekCtrlAttachBodyC::SetSeekCtrl(const DPSeekCtrlC &sc) { 
    ONDEBUG(cerr << "DPSeekCtrlAttachBodyC::SetSeekCtrl(.) called. \n");
    sctrl = sc; 
  }
  
}
