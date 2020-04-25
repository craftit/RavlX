// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay3d/DPWindow3dOPort.cc"

#include "Ravl/GUI/DPWindow3dOPort.hh"
#include "Ravl/GUI/ReadBack.hh"
#include "Ravl/GUI/Manager.hh"

namespace RavlGUIN {
  
  //: Constructor.
  
  DPWindow3dOPortBodyC::DPWindow3dOPortBodyC(DPWindow3dC &nwin,bool accum,bool resetWin) 
    : accumulate(accum),
      m_resetWin(resetWin),
      win(nwin)
  {}
  
  //: Process in coming display objects.
  
  bool DPWindow3dOPortBodyC::Put(const DObject3DC &newObj) {
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
      // Aquire a readback lock to ensure window is properly opened.
      ReadBackLockC rbLock(true);
    }
    return true;
  }
  
}
