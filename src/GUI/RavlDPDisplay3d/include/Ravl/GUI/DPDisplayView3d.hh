// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPDISPLAYVIEW3D_HEADER
#define RAVLGUI_DPDISPLAYVIEW3D_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay/DPDisplayView.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.3D Data Display"

#include "Ravl/GUI/Table.hh"
#include "Ravl/GUI/View3D.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/GUI/Ruler.hh"
#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"
#include "Ravl/GUI/DObject3D.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlGUIN {
  class DPDisplayView3dC;
  class MouseEventC;
  //! userlevel=Develop
  //: Widget for displaying DPDisplayObjC's.
  
  class DPDisplayView3dBodyC
    : public TableBodyC
  {
  public:
    DPDisplayView3dBodyC(const IndexRange2dC &size);
    //: Default constructor.
    
    virtual ~DPDisplayView3dBodyC();
    //: Need virtual destructor for class with virtual methods
    
    virtual bool Create();
    //: Create the widget.
    
    bool AddObject(const DObject3DC &obj);
    //: Add object to the display list.
    
    bool HandleUpdateDisplayRange(RealRange2dC &rng);
    //: Update range.
    
    bool Clear();
    //: Clear the display list.
    
    bool Refresh();
    //: Refresh the display.
    
    bool SetResizeOnNextObject();
    //: Resize on display of next object.
    
    View3DC &View3d()
    { return m_view3d; }
    //: Access canvas.
    
    const View3DC &View3d() const
    { return m_view3d; }
    //: Access canvas.
    
  protected:
    bool GUIRefresh();
    //: Refresh the display.
    
    bool UpdateRuler();
    //: Update ruler info.

    bool GUIUpdateRuler();
    //: Update ruler info.
    
    bool Query(Vector2dC pos,StringC &info);
    //: Query position,
    
    bool CallbackConfigure(GdkEvent *&event);
    //: Handle configure callback.
    
    bool CallbackMouseMotion(MouseEventC &mouseEvent);
    //: Call back for mouse movements in the window.
    
    bool CallbackMousePress(MouseEventC &mouseEvent);
    //: Call back for mouse press events.
    
    bool UpdateInfo(const Vector2dC &at);
    //: Update info for mouse position.
    
    bool CallbackStartSave();
    //: Start image save.
    
    bool CallbackSave(StringC &str);
    //: Save image to a file.
    
    virtual void Destroy();
    //: Handle widget destruction
    
    IndexRange2dC winSize;
    View3DC m_view3d;
    RWLockC lockDisplayList;
    DListC<DObject3DC> displayList;
    
    
    bool refreshQueued;
    
    RulerC vRuler,hRuler;
    LabelC rowPos,  colPos,info;
    
    IndexRange2dC displaySize; // Size of displayable objects.
    
    Vector2dC lastMousePos;
    
    MenuBarC menuBar;
    FileSelectorC fileSelector;
    
    SignalConnectionSetC connections;
    bool m_resizeOnNextObject;
    
    friend class DPDisplayView3dC;
  };

  //! userlevel=Normal
  //: Widget for displaying DPDisplayObjC's.
  
  class DPDisplayView3dC
    : public TableC
  {
  public:
    DPDisplayView3dC(const IndexRange2dC &size)
      : TableC(*new DPDisplayView3dBodyC(size))
    {}
    //: Constructor.

    DPDisplayView3dC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
  protected:
    DPDisplayView3dC(DPDisplayView3dBodyC &body)
      : TableC(body)
    {}
    //: Body constructor.
    
    DPDisplayView3dBodyC &Body()
    { return static_cast<DPDisplayView3dBodyC &>(WidgetC::Body()); }
    //: Access body.

    const DPDisplayView3dBodyC &Body() const
    { return static_cast<const DPDisplayView3dBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool GUIRefresh()
    { return Body().GUIRefresh(); }
    //: Refresh the display.
    
    bool CallbackSave(StringC &str)
    { return Body().CallbackSave(str); }
    //: Save image to a file.
    
    bool GUIUpdateRuler()
    { return Body().GUIUpdateRuler(); }
    //: Update ruler info.
    
    bool HandleUpdateDisplayRange(RealRange2dC &rng)
    { return Body().HandleUpdateDisplayRange(rng); }
    //: Update range.
    
  public:
    
    bool CallbackStartSave()
    { return Body().CallbackStartSave(); } 
    //: Start image save.
    
    bool AddObject(const DObject3DC &obj) 
    { return Body().AddObject(obj); }
    //: Add object to the display list.
    
    bool Clear()
    { return Body().Clear(); }
    //: Clear the display list.
    
    bool Refresh()
    { return Body().Refresh(); }
    //: Refresh the display.
    
    bool SetResizeOnNextObject()
    { return Body().SetResizeOnNextObject(); }
    //: Resize on display of next object.
    
    friend class DPDisplayView3dBodyC;
  };
}


#endif
