// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPWindow.cc"

#include "Ravl/GUI/DPWindow.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/GUI/WaitForExit.hh"

#include <stdlib.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlGUIN {
  
  DPWindowBodyC::DPWindowBodyC(const StringC &nname,const IndexRange2dC &nWinSize)
    : name(nname),
      winSize(nWinSize),
      m_layerCount(0)
  {}
  
  //: Called when window is closed.
  
  bool DPWindowBodyC::WindowClosed() {
    ONDEBUG(std::cerr << "DPWindowBodyC::WindowClosed(), Called. \n");
    MutexLockC hold(accessMutex);
    view.Invalidate();
    win.Invalidate();
    return true;
  }
  
  //: Construct widgets.
  // Access mutex must be held when calling Init().
  
  void DPWindowBodyC::Init(const IndexRange2dC &size) {
    ONDEBUG(std::cerr << "DPWindowBodyC::Init(), Called. Size=" << winSize << " \n");
    if(view.IsValid()) // Check for race in setup.
      return ;
    winSize = size;
    // Ensure the manager is started.
    // And make sure program doesn't exit before use has closed the window.
    WaitForGUIExit(); 
    
    // Create a window and display it.
    win = WindowC(winSize.LCol().V()+10,
		  winSize.BRow().V()+10,name);
    DPDisplayViewC nview(winSize);
    win.Add(nview);
    
    ConnectRef(win.Signal("delete_event"),*this,&DPWindowBodyC::WindowClosed);
    
    win.Show();
    
    // Don't setup view until we're ready to start processing data.
    view = nview;
  }
  
  //: Add object to the display list.
  
  bool DPWindowBodyC::AddObject(const DPDisplayObjC &obj) { 
    ONDEBUG(std::cerr << "DPWindowBodyC::AddObject(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid()) 
      Init(obj.Frame());
    DPDisplayObjC(obj).SetLayerNo(m_layerCount++);
    view.AddObject(obj);
    //view.Refresh();
    return true;
  }
  
  //: This clears the display list then adds 'obj'.
  
  bool DPWindowBodyC::ReplaceObject(const DPDisplayObjC &obj,bool resetPosition) {
    ONDEBUG(std::cerr << "DPWindowBodyC::ReplaceObject(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid()) 
      Init(obj.Frame());
    view.Clear();
    if(resetPosition)
      view.SetResizeOnNextObject();
    m_layerCount = 0;
    DPDisplayObjC(obj).SetLayerNo(m_layerCount++);
    view.AddObject(obj);
    //view.Refresh();
    return true;
  }
  
  //: Clear the display list.
  
  bool DPWindowBodyC::Clear() {  
    ONDEBUG(std::cerr << "DPWindowBodyC::Clear(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid())
      return true;
    return view.Clear();
  }
  
  //: Refresh the display.
  
  bool DPWindowBodyC::Refresh() {
    ONDEBUG(std::cerr << "DPWindowBodyC::Refresh(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid())
      return true;
    return view.Refresh();
  }

}
