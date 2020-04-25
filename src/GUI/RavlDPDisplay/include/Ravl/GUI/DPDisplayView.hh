// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPDISPLAYVIEW_HEADER
#define RAVLGUI_DPDISPLAYVIEW_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPDisplayView.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Data Display"

#include "Ravl/GUI/Table.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/GUI/DPDisplayObj.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/GUI/Ruler.hh"
#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"

namespace RavlGUIN {
  class DPDisplayViewC;
  class MouseEventC;
  //! userlevel=Develop
  //: Widget for displaying DPDisplayObjC's.
  
  class DPDisplayViewBodyC
    : public TableBodyC
  {
  public:
    DPDisplayViewBodyC(const IndexRange2dC &size);
    //: Default constructor.
    
    virtual ~DPDisplayViewBodyC();
    //: Need virtual destructor for class with virtual methods
    
    virtual bool Create();
    //: Create the widget.
    
    bool AddObject(const DPDisplayObjC &obj);
    //: Add object to the display list.
    
    bool HandleUpdateDisplayRange(RealRange2dC &rng);
    //: Update range.
    
    bool Clear();
    //: Clear the display list.
    
    bool Refresh();
    //: Refresh the display.
    
    bool SetResizeOnNextObject();
    //: Resize on display of next object.
    
    GUIMarkupCanvasC &Canvas()
    { return canvas; }
    //: Access canvas.

    const GUIMarkupCanvasC &Canvas() const
    { return canvas; }
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
    
    bool CallbackExpose(GdkEvent *&event);
    //: Handle an expose event.
    
    bool CallbackConfigure(GdkEvent *&event);
    //: Handle configure callback.
    
    bool CallbackMouseMotion(MouseEventC &mouseEvent);
    //: Call back for mouse movements in the window.
    
    bool CallbackMousePress(MouseEventC &mouseEvent);
    //: Call back for mouse press events.
    
    bool CallbackFitToWindow();
    //: Fit image to window.
    
    bool UpdateInfo(const Vector2dC &at);
    //: Update info for mouse position.
    
    bool CallbackStartSave();
    //: Start image save.
    
    bool CallbackSave(StringC &str);
    //: Save image to a file.
    
    virtual void Destroy();
    //: Handle widget destruction
    
    IndexRange2dC winSize;
    GUIMarkupCanvasC canvas;
    RWLockC lockDisplayList;
    DListC<DPDisplayObjC> displayList;
    
    
    bool refreshQueued;
    
    RulerC vRuler,hRuler;
    LabelC rowPos,  colPos,info;
    
    IndexRange2dC displaySize; // Size of displayable objects.
    
    Vector2dC lastMousePos;
    
    MenuC backMenu;
    MenuBarC menuBar;
    FileSelectorC fileSelector;

    SignalConnectionSetC connections;
    bool m_resizeOnNextObject;
    
    friend class DPDisplayViewC;
  };

  //! userlevel=Normal
  //: Widget for displaying DPDisplayObjC's.
  
  class DPDisplayViewC
    : public TableC
  {
  public:
    DPDisplayViewC(const IndexRange2dC &size)
      : TableC(*new DPDisplayViewBodyC(size))
    {}
    //: Constructor.

    DPDisplayViewC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
  protected:
    DPDisplayViewC(DPDisplayViewBodyC &body)
      : TableC(body)
    {}
    //: Body constructor.
    
    DPDisplayViewBodyC &Body()
    { return static_cast<DPDisplayViewBodyC &>(WidgetC::Body()); }
    //: Access body.

    const DPDisplayViewBodyC &Body() const
    { return static_cast<const DPDisplayViewBodyC &>(WidgetC::Body()); }
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
    
    bool AddObject(const DPDisplayObjC &obj) 
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
    
    friend class DPDisplayViewBodyC;
  };
}


#endif
