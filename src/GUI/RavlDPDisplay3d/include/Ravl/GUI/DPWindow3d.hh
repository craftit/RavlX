// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPWINDOW3D_HEADER
#define RAVLGUI_DPWINDOW3D_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay3d/DPWindow3d.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.3D Data Display"

#include "Ravl/DP/Port.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/DPDisplayView3d.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlGUIN {
  class DPWindow3dC;
  
  //! userlevel=Develop
  //: DP Window base.
  
  class DPWindow3dBodyC 
    : public RCBodyVC
  {
  public:
    DPWindow3dBodyC(const StringC &nname,const IndexRange2dC &nWinSize = IndexRange2dC(IndexC(100),IndexC(100)));
    //: Default constructor.
    
    const StringC &Name() const
    { return name; }
    //: Access window name.
    
    void Init(const IndexRange2dC &size);
    //: Construct widgets.
    
    bool AddObject(const DObject3DC &obj);
    //: Add object to the display list.
    
    bool ReplaceObject(const DObject3DC &obj,bool resetPosition = false);
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
    DPDisplayView3dC view;
    UIntT m_layerCount;
    friend class DPWindow3dC;    
  };
  
  //! userlevel=Normal
  //: DP Window.
  
  class DPWindow3dC 
    : public RCHandleC<DPWindow3dBodyC>
  {
  public:
    DPWindow3dC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPWindow3dC(const StringC &nname,const IndexRange2dC &nWinSize = IndexRange2dC(IndexC(100),
										 IndexC(100)))
      : RCHandleC<DPWindow3dBodyC>(*new DPWindow3dBodyC(nname,nWinSize))
    {}
    //: Construct a named window.

  protected:
    DPWindow3dC(DPWindow3dBodyC &body)
      : RCHandleC<DPWindow3dBodyC>(body)
    {}
    //: Body constructor.
    
    DPWindow3dBodyC &Body()
    { return RCHandleC<DPWindow3dBodyC>::Body(); }
    //: Access body.

    const DPWindow3dBodyC &Body() const
    { return RCHandleC<DPWindow3dBodyC>::Body(); }
    //: Access body.

    
  public:
    
    bool AddObject(const DObject3DC &obj) 
    { return Body().AddObject(obj); }
    //: Add object to the display list.
    
    bool ReplaceObject(const DObject3DC &obj,bool resetPosition = false)
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

    friend class DPWindow3dBodyC;
  };
  
}



#endif
