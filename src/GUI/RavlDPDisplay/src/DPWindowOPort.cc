// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPWindowOPort.cc"

#include "Ravl/GUI/DPWindowOPort.hh"
#include "Ravl/GUI/ReadBack.hh"
#include "Ravl/GUI/Manager.hh"

namespace RavlGUIN {
  
  //: Constructor.
  
  DPWindowOPortBodyC::DPWindowOPortBodyC(DPWindowC &nwin,bool accum,bool resetWin) 
    : accumulate(accum),
      m_resetWin(resetWin),
      win(nwin)
  {}
  
  //: Process in coming display objects.
  
  bool DPWindowOPortBodyC::Put(const DPDisplayObjC &newObj) {
    bool wasOpen = win.IsOpen();
    if(accumulate) {
      win.AddObject(newObj);
    } else {
      win.ReplaceObject(newObj,m_resetWin);
    }
    // Wait for initial window to open before returning, this avoids
    // some race conditions if the program exists soon after displaying
    // something.
    if(!wasOpen) {
      // Acquire a readback lock to ensure window is properly opened.
      ReadBackLockC rbLock(true);
    }
    return true;
  }
  
}
