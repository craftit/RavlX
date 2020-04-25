// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPWINDOW_HEADER
#define RAVLGUI_DPWINDOW_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPWindow.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Data Display"

#include "Ravl/DP/Port.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/DPDisplayView.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlGUIN {
  class DPWindowC;
  
  void DisableWaitForGUIExit();
  //: Disable waiting for GUI to finish on program exit.
  // Call this method to allow program to exit immediatly even if
  // there are DPWindowC's open.
  
  //! userlevel=Develop
  //: DP Window base.
  
  class DPWindowBodyC 
    : public RCBodyVC
  {
  public:
    DPWindowBodyC(const StringC &nname,const IndexRange2dC &nWinSize = IndexRange2dC(IndexC(100),IndexC(100)));
    //: Default constructor.
    
    const StringC &Name() const
    { return name; }
    //: Access window name.
    
    void Init(const IndexRange2dC &size);
    //: Construct widgets.
    
    bool AddObject(const DPDisplayObjC &obj);
    //: Add object to the display list.
    
    bool ReplaceObject(const DPDisplayObjC &obj,bool resetPosition = false);
    //: This clears the display list then adds 'obj'.
    
    bool Clear();
    //: Clear the display list.
    
    bool Refresh();
    //: Refresh the display.
    
    bool WindowClosed();
    //: Called when window is closed.
    
    bool IsOpen() const
    { return view.IsValid(); }
    //: Test if view has been created.
    
  protected:    
    StringC name;
    MutexC accessMutex; // Make sure window is only setup at once.
    WindowC win;
    IndexRange2dC winSize;
    DPDisplayViewC view;
    UIntT m_layerCount;
    friend class DPWindowC;    
  };
  
  //! userlevel=Normal
  //: DP Window.
  
  class DPWindowC 
    : public RCHandleC<DPWindowBodyC>
  {
  public:
    DPWindowC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPWindowC(const StringC &nname,const IndexRange2dC &nWinSize = IndexRange2dC(IndexC(100),
										 IndexC(100)))
      : RCHandleC<DPWindowBodyC>(*new DPWindowBodyC(nname,nWinSize))
    {}
    //: Construct a named window.

  protected:
    DPWindowC(DPWindowBodyC &body)
      : RCHandleC<DPWindowBodyC>(body)
    {}
    //: Body constructor.
    
    DPWindowBodyC &Body()
    { return RCHandleC<DPWindowBodyC>::Body(); }
    //: Access body.

    const DPWindowBodyC &Body() const
    { return RCHandleC<DPWindowBodyC>::Body(); }
    //: Access body.

    
  public:
    
    bool AddObject(const DPDisplayObjC &obj) 
    { return Body().AddObject(obj); }
    //: Add object to the display list.
    
    bool ReplaceObject(const DPDisplayObjC &obj,bool resetPosition = false)
    { return Body().ReplaceObject(obj,resetPosition); }
    //: This clears the display list then adds 'obj'.
    
    bool Clear()
    { return Body().Clear(); }
    //: Clear the display list.
    
    bool Refresh()
    { return Body().Refresh(); }
    //: Refresh the display.
    
    bool WindowClosed()
    { return Body().WindowClosed(); }
    //: Called when window is closed.
    
    bool IsOpen() const
    { return Body().IsOpen(); }
    //: Test if view has been created.

    friend class DPWindowBodyC;
  };
  
}



#endif
