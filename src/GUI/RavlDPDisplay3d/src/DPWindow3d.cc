// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay/DPWindow3d.cc"

#include "Ravl/GUI/DPWindow3d.hh"
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
  
  static const IndexRange2dC g_defaultWindowSize(512,512);
  
  DPWindow3dBodyC::DPWindow3dBodyC(const StringC &nname,const IndexRange2dC &nWinSize)
    : name(nname),
      winSize(nWinSize),
      m_layerCount(0)
  {}
  
  //: Called when window is closed.
  
  bool DPWindow3dBodyC::WindowClosed() {
    ONDEBUG(cerr << "DPWindow3dBodyC::WindowClosed(), Called. \n");
    MutexLockC hold(accessMutex);
    view.Invalidate();
    win.Invalidate();
    return true;
  }
  
  //: Construct widgets.
  // Access mutex must be held when calling Init().
  
  void DPWindow3dBodyC::Init(const IndexRange2dC &size) {
    ONDEBUG(cerr << "DPWindow3dBodyC::Init(), Called. Size=" << winSize << " \n");
    if(view.IsValid()) // Check for race in setup.
      return ;
    winSize = size;
    // Ensure the manager is started.
    // And make sure program doesn't exit before use has closed the window.
    WaitForGUIExit(); 
    
    // Create a window and display it.
    win = WindowC(winSize.LCol().V()+10,
		  winSize.BRow().V()+10,name);
    DPDisplayView3dC nview(winSize);
    win.Add(nview);
    
    ConnectRef(win.Signal("delete_event"),*this,&DPWindow3dBodyC::WindowClosed);
    
    win.Show();
    
    // Don't setup view until we're ready to start processing data.
    view = nview;
  }
  
  //: Add object to the display list.
  
  bool DPWindow3dBodyC::AddObject(const DObject3DC &obj) { 
    ONDEBUG(cerr << "DPWindow3dBodyC::AddObject(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid()) 
      Init(g_defaultWindowSize);
    //DPDisplayObjC(obj).SetLayerNo(m_layerCount++);
    view.AddObject(obj);
    //view.Refresh();
    return true;
  }
  
  //: This clears the display list then adds 'obj'.
  
  bool DPWindow3dBodyC::ReplaceObject(const DObject3DC &obj,bool resetPosition) {
    ONDEBUG(cerr << "DPWindow3dBodyC::ReplaceObject(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid()) 
      Init(g_defaultWindowSize);
    view.Clear();
    if(resetPosition)
      view.SetResizeOnNextObject();
    m_layerCount = 0;
    //DObject3DC(obj).SetLayerNo(m_layerCount++);
    view.AddObject(obj);
    //view.Refresh();
    return true;
  }
  
  //: Clear the display list.
  
  bool DPWindow3dBodyC::Clear() {  
    ONDEBUG(cerr << "DPWindow3dBodyC::Clear(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid())
      return true;
    return view.Clear();
  }
  
  //: Refresh the display.
  
  bool DPWindow3dBodyC::Refresh() {
    ONDEBUG(cerr << "DPWindow3dBodyC::Refresh(), Called \n");
    MutexLockC hold(accessMutex);
    if(!view.IsValid())
      return true;
    return view.Refresh();
  }

}
