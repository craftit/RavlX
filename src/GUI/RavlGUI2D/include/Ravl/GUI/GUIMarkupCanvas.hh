// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPCANVAS_HEADER
#define RAVLGUI_MARKUPCANVAS_HEADER 1
//! rcsid="$Id: GUIMarkupCanvas.hh 3530 2004-07-01 13:40:39Z charles $"
//! lib=RavlGUI2D
//! file="Magellan/GUI/GUIMarkupCanvas.hh"
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"
//! example="exGUIMarkupCanvas.cc"

#include "Ravl/GUI/RawZoomCanvas.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/GUI/ScrollEvent.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Tuple3.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/Signal2.hh"
#include "Ravl/RealRange2d.hh"
#include "Ravl/RealRange2dSet.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/GUI/FrameMarkup.hh"
#include "Ravl/GUI/MarkupLayerInfo.hh"
#include "Ravl/Threads/SignalConnectionSet.hh"
#include "Ravl/OS/Date.hh"

namespace RavlGUIN {
  
  //! userlevel=Develop
  //: Image with polygon markups.
  
  class GUIMarkupCanvasBodyC 
    : public RawZoomCanvasBodyC
  {
  public:
    GUIMarkupCanvasBodyC(int rows,int cols);
    //: Constructor.
    
    virtual ~GUIMarkupCanvasBodyC();
    //: Destructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create the widget.
    
    bool GUIRefresh();
    //: Update the whole screen.
    
    bool GUIRender(const RealRange2dC &range);
    //: Update part of the display
    
    bool Refresh();
    //: Update the screen.
    
    bool GUISelectObject(const Index2dC &at,bool doRefresh = true);
    //: Select object at given point.
    //!param: at - position to select.
    //!param: doRefresh - Refresh the screen 
    //!return: true if selection changed.
    
    bool EventMouseMove(MouseEventC &me);
    //: Mouse move.
    
    bool EventMousePress(MouseEventC &me);
    //: Mouse press.
    
    bool EventMouseRelease(MouseEventC &me);
    //: Mouse release.
    
    bool EventScroll(ScrollEventC &event);
    //: Scroll event
    
    virtual bool EventConfigure(GdkEvent* &event);
    //: Configure.
    
    bool EventExpose(GdkEvent* &event);
    //: Expose of area.
    
    bool EventKeyPress(GdkEventKey *&key);
    //: Handle key press events.
    
    bool EventKeyRelease(GdkEventKey *&key);
    //: Handle key release events.
    
    bool SetFitToFrame(bool value)
    { return fitToFrame = value; }
    //: Set fit to frame mode.
    
    Signal2C<Int64T,MarkupInfoC> &SignalPolygonUpdate()
    { return sigMarkupUpdate; }
    //: Signal called on button release mouse button in a polygon update.
    
    Signal1C<RealRange2dC> &SignalDisplayRange()
    { return sigDisplayRange; }
    //: Signal called on change of display range.
    
    Signal1C<MouseEventC> &SignalBackMenu()
    { return sigBackMenu; }
    //: Signal request for back menu.

    Signal1C<MouseEventC> &SigDoublePress()
    { return m_sigDoublePress; }
    //: Signal called on mouse button double press.

    const HSetC<Int64T> &GUISelected() const
    { return selected; }
    //: Access list of selected objects
    
    DListC<Int64T> Selected() const;
    //: Access list of selected objects
    
    GdkGC *GcDrawContext()
    { return gcDrawContext; }
    //: Access drawing context
    
    FrameMarkupC &FrameMarkup()
    { return m_frameMarkup; }
    //: Access current frame markup.
    
    bool UpdateMarkup(const FrameMarkupC &markup);
    //: Update current markup.
    
    bool GUIUpdateMarkup(const FrameMarkupC &markup);
    //: Update current markup.
    
    MarkupInfoC &MouseObject()
    { return m_mouseObj; }
    //: Access current mouse object
    
    IntT MouseState() const
    { return mouseState; }
    //: Access mouse state variable
    
    RCAbstractC &MouseInfo()
    { return mouseInfo; }
    //: Access mouse state variable
    
    void SetMouseInfo(const RCAbstractC &info) 
    { mouseInfo = info; }
    //: Access mouse state variable
    
    const Point2dC &MousePressAt() const
    { return mousePressAt; }
    //: Access location of mouse press.
    
    const Point2dC & EventMouseLast() const
    { return mouseLastEvent; } 
    //: Access the location of the last event....
    
    
    bool GUIAddSelect(Int64T id,bool signal = true);
    //: Add id to selection.
    // Note: This does not cause a refresh
    
    bool GUIDelSelect(Int64T id,bool signal = true);
    //: Remove id from selection.
    // Note: This does not cause a refresh
    
    bool GUIClearSelect(bool signal = true);
    //: Clear current selection.
    
    bool GUIIsSelected(Int64T id)
    { return selected.IsMember(id); }
    //: Test if we're selected.
    
    Signal1C<DListC<Int64T> > &SigSelected()
    { return sigSelection; }
    //: Access selection changed signal
    // Called on the GUI thread.
    
    bool SetSelection(const DListC<Int64T> &list,bool sigChange = false);
    //: Set the current selection.
    //!param: list - Set of objects to select.
    //!param: sigChanged - Issue signal about selection changing.
    
    bool GUISetSelection(const DListC<Int64T> &list,bool sigChange = false);
    //: Set the current selection.
    //!param: list - Set of objects to select.
    //!param: sigChanged - Issue signal about selection changing.
    
    bool GUIFitImageToScreen(RealT aspectRatio = 0);
    //: Fit current display to screen.
    // If aspect ratio is <=0 square pixels are assumed.
    
    bool FitImageToScreen(RealT aspectRatio = 0);
    //: Fit current display to screen.
    // If aspect ratio is <=0 square pixels are assumed.
    
    bool GUISetAspectRatio(RealT aspectRatio);
    //: Set the current aspect ratio of the displayed image.
    
    bool GUIFixAspectRatio(RealT aspectRatio);
    //: Set the current aspect ratio of a pixel
    
    bool ExcludeZOrder(IntT zOrder);
    //: Exclude a Z order layer from display.
    
    bool IncludeZOrder(IntT zOrder);
    //: Include a Z order layer in display.
    
    bool GUIExcludeZOrder(IntT zOrder);
    //: Exclude a Z order layer from display.
    
    bool GUIIncludeZOrder(IntT zOrder);
    //: Include a Z order layer in display.
    
    bool ZOrderExcluded(const IntT zOrder);
    //: Is the Z order layer excluded?
    
    void GUIAddLayerInfo(const MarkupLayerInfoC &layerInfo);
    //: Add a layer info descriptor
    
    bool LayerInfo(const IntT layerId, MarkupLayerInfoC &layerInfo);
    //: Get a layer info descriptor by layer id
    // Returns false if id not found
    
    void SetSelectedLayer(IntT selectedLayerId);
    //: Set the selected layer id
    
    bool GUIShowLayerDialog();
    //: Show the layer properties dialog
    
    bool LayerDialogShowLine(const bool showLine);
    //: Should the layer editor dialog show the line column?
    
    bool FrameContains(const StringC &key, const StringC val);
    //: Does the markup info contain a key with the specified val?
    
  protected:
    bool CommonCreate();
    //: Do the common creation stuff.
   
    void SetupGC();
    //: Setup graphics context.
    
    MarkupInfoC FindTarget(const Point2dC &at,const MouseEventC &me);
    //: Find target of click
    
    void UpdateCoverage();
    //: Update the coverage list
    
    bool GUISetLayerVisibility(IntT zOrder, bool visible);
    //: Set the layer visibility
    
    bool GUISetLayerInfo(MarkupLayerInfoC layerInfo);
    //: Set the layer visibility
    
    virtual void Destroy();
    //: Handle widget destruction
    
    RWLockC accessLock;

    FrameMarkupC m_frameMarkup; // Current frame info.
    
    RealRange2dSetC coverage;   // Area covered by markup.
    RealRange2dC displayArea;   // Current display area.
    
    HSetC<Int64T> selected;
    HSetC<IntT> removeZOrder;   // Set of Z order layers to be excluded from display.
    
    Point2dC mousePressAt;        // Position of last mouse button press event.
    Point2dC mouseRawPressAt;     // Position of last mouse button press event.
    Point2dC mouseLastEvent;      // Position of last event.
    MarkupInfoC m_mouseObj;       // Object selected with last press.
    IntT mouseState;              // State information for handing mouse presses
    RCAbstractC mouseInfo;        // Extra info for current mouse operation.
    RealRange2dC lastSelectBox;   // Last drawn select box.
    
    bool backgroundDrag;
    bool selectBox;
    Vector2dC mouseViewOffset;    // Offset of view at mouse press.
    
    GdkGC *gcDrawContext;
    
    bool fitToFrame;
    DateC lastTimeOfFitToFrame; // The time of the last fit to frame, used to flag updates if window geometry changes shortly after
    
    Signal2C<Int64T,MarkupInfoC> sigMarkupUpdate; // Signal called on button release of a polygon update.
    //: Signal called on button release mouse button in a polygon update.
    
    Signal1C<DListC<Int64T> > sigSelection;
    //: Selection changed signal.
    
    Signal1C<RealRange2dC> sigDisplayRange;
    //: Signal called on change of display range.
    
    Signal1C<MouseEventC> sigBackMenu;
    //: Signal request for back menu.

    Signal1C<MouseEventC> m_sigDoublePress;
    //: Signal called on mouse button double press.

    DListC<MarkupLayerInfoC> m_layerInfo;
    IntT m_selectedLayerId;
    bool m_dialogshowLine;
    
    SignalConnectionSetC connections;

    RealT m_aspectRatio;
    
    friend class GUIMarkupCanvasC;
    
  };
  
  
  //! userlevel=Normal
  //: Image with polygon markups.
  
  class GUIMarkupCanvasC 
    : public RawZoomCanvasC
  {
  public:
    GUIMarkupCanvasC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    GUIMarkupCanvasC(int sx,int sy)
      : RawZoomCanvasC(*new GUIMarkupCanvasBodyC(sx,sy))
    {}
    //: Constructor.
    
    bool Create() 
    { return Body().Create(); }
    //: Create the widget. 
    //!cwiz:author
    
    bool Create(GtkWidget *widget)
    { return Body().Create(widget); }
    //: Create the widget.
    
  protected:
    GUIMarkupCanvasC(GUIMarkupCanvasBodyC &bod)
      : RawZoomCanvasC(bod)
    {}
    //: Body constructor.
    
    GUIMarkupCanvasBodyC &Body()
    { return static_cast<GUIMarkupCanvasBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const GUIMarkupCanvasBodyC &Body() const
    { return static_cast<const GUIMarkupCanvasBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    
  public:
    bool GUIRefresh()
    { return Body().GUIRefresh(); }
    //: Update the screen.
    // GUI Thread only.
    
    bool GUIRender(const RealRange2dC & range) 
    { return Body().GUIRender(range); }
    //: Update part of the display 
    //!cwiz:author
    
    bool Refresh()
    { return Body().Refresh(); }
    //: Update the screen.
    // Redraw everything.
    
    bool EventMouseMove(MouseEventC & me) 
    { return Body().EventMouseMove(me); }
    //: Mouse move. 
    //!cwiz:author
    
    bool EventMousePress(MouseEventC & me) 
    { return Body().EventMousePress(me); }
    //: Mouse press. 
    //!cwiz:author
    
    bool EventMouseRelease(MouseEventC & me) 
    { return Body().EventMouseRelease(me); }
    //: Mouse release. 
    //!cwiz:author

    const Point2dC & EventMouseLast() const
    { return Body().EventMouseLast(); } 
    //: Access the location of the last event....
        
    bool EventScroll(ScrollEventC &event)
    { return Body().EventScroll(event); }
    //: Scroll event 
    //!cwiz:author
    
    bool EventConfigure(GdkEvent *& event) 
    { return Body().EventConfigure(event); }
    //: Configure. 
    //!cwiz:author
    
    bool EventExpose(GdkEvent *& event) 
    { return Body().EventExpose(event); }
    //: Expose of area. 
    //!cwiz:author
    
    bool EventKeyPress(GdkEventKey *& key) 
    { return Body().EventKeyPress(key); }
    //: Handle key press events. 
    //!cwiz:author
    
    bool EventKeyRelease(GdkEventKey *& key) 
    { return Body().EventKeyRelease(key); }
    //: Handle key release events. 
    //!cwiz:author
    
    bool SetFitToFrame(bool value)
    { return Body().SetFitToFrame(value); }
    //: Set fit to frame mode.
    
    Signal2C<Int64T,MarkupInfoC> & SignalPolygonUpdate() 
    { return Body().SignalPolygonUpdate(); }
    //: Signal called on button release mouse button in a polygon update. 
    //!cwiz:author
    
    Signal1C<RealRange2dC> &SignalDisplayRange()
    { return Body().SignalDisplayRange(); }
    //: Signal called on change of display range.
    
    Signal1C<MouseEventC> &SignalBackMenu()
    { return Body().SignalBackMenu(); }
    //: Signal request for back menu.
    
    Signal1C<MouseEventC> &SigDoublePress()
    { return Body().SigDoublePress(); }
    //: Signal called on mouse button double press.

    const HSetC<Int64T> & GUISelected() const
    { return Body().GUISelected(); }
    //: Access list of selected polygons 
    //!cwiz:author
    
    DListC<Int64T> Selected() const
    { return Body().Selected(); }
    //: Access list of selected polygons
    
    GdkGC * GcDrawContext() 
    { return Body().GcDrawContext(); }
    //: Access drawing context

    FrameMarkupC & FrameMarkup() 
    { return Body().FrameMarkup(); }
    //: Access current frame markup. 
    //!cwiz:author
    
    bool UpdateMarkup(const FrameMarkupC & markup) 
    { return Body().UpdateMarkup(markup); }
    //: Update current markup. 
    //!cwiz:author
    
    bool GUIUpdateMarkup(const FrameMarkupC & markup) 
    { return Body().GUIUpdateMarkup(markup); }
    //: Update current markup. 
    //!cwiz:author
    
    MarkupInfoC & MouseObject() 
    { return Body().MouseObject(); }
    //: Access current mouse object 
    //!cwiz:author
    
    IntT MouseState() const
    { return Body().MouseState(); }
    //: Access mouse state variable 
    //!cwiz:author
    
    RCAbstractC & MouseInfo() 
    { return Body().MouseInfo(); }
    //: Access mouse state variable 
    //!cwiz:author
    
    void SetMouseInfo(const RCAbstractC & info) 
    { Body().SetMouseInfo(info); }
    //: Access mouse state variable 
    //!cwiz:author
    
    const Point2dC & MousePressAt() const
    { return Body().MousePressAt(); }
    //: Access location of mouse press. 
    //!cwiz:author
    
    bool GUIAddSelect(Int64T id,bool signal = true) 
    { return Body().GUIAddSelect(id,signal = true); }
    //: Add id to selection. 
    // Note: This does not cause a refresh
    //!cwiz:author
    
    bool GUIDelSelect(Int64T id,bool signal = true) 
    { return Body().GUIDelSelect(id,signal = true); }
    //: Remove id from selection. 
    // Note: This does not cause a refresh
    //!cwiz:author
    
    bool GUIClearSelect(bool signal = true) 
    { return Body().GUIClearSelect(signal = true); }
    //: Clear current selection. 
    //!cwiz:author
    
    bool GUIIsSelected(Int64T id) 
    { return Body().GUIIsSelected(id); }
    //: Test if we're selected. 
    //!cwiz:author
    
    Signal1C<DListC<Int64T> > &SigSelected()
    { return Body().SigSelected(); }
    //: Access selection changed signal
    // Called on the GUI thread.
    
    bool SetSelection(const DListC<Int64T> &list,bool sigChange = false)
    { return Body().SetSelection(list,sigChange); }
    //: Set the current selection.
    //!param: list - Set of objects to select.
    //!param: sigChanged - Issue signal about selection changing.
    
    bool GUISetSelection(const DListC<Int64T> &list,bool sigChange = false)
    { return Body().GUISetSelection(list,sigChange); }
    //: Set the current selection.
    //!param: list - Set of objects to select.
    //!param: sigChanged - Issue signal about selection changing.
    
    bool GUIFitImageToScreen(RealT aspectRatio = 0)
    { return Body().GUIFitImageToScreen(aspectRatio); }
    //: Fit current display to screen.    
    // If aspect ratio is <=0 square pixels are assumed.
    
    bool FitImageToScreen(RealT aspectRatio = 0)
    { return Body().FitImageToScreen(aspectRatio); }
    //: Fit current display to screen.
    // If aspect ratio is <=0 square pixels are assumed.
    
    bool GUIFixAspectRatio(RealT aspectRatio)
    { return Body().GUIFixAspectRatio(aspectRatio); }
    //: Set the current aspect ratio of a pixel
    
    bool GUISetAspectRatio(RealT aspectRatio)
    { return Body().GUISetAspectRatio(aspectRatio); }
    //: Set the current aspect ratio of an image.
    
    bool ExcludeZOrder(IntT zOrder)
    { return Body().ExcludeZOrder(zOrder); }
    //: Exclude a Z order layer from display.
    
    bool IncludeZOrder(IntT zOrder)
    { return Body().IncludeZOrder(zOrder); }
    //: Include a Z order layer in display.
    
    bool GUIExcludeZOrder(IntT zOrder)
    { return Body().GUIExcludeZOrder(zOrder); }
    //: Exclude a Z order layer from display.
    
    bool GUIIncludeZOrder(IntT zOrder)
    { return Body().GUIIncludeZOrder(zOrder); }
    //: Include a Z order layer in display.
    
    bool ZOrderExcluded(const IntT zOrder)
    { return Body().ZOrderExcluded(zOrder); }
    //: Is the Z order layer excluded?
    
    bool GUISelectObject(const Index2dC &at,bool doRefresh = true)
    { return Body().GUISelectObject(at); }
    //: Select object at given point.
    //!param: at - position to select.
    //!param: doRefresh - Refresh the screen 
    //!return: true if selection changed.
    
    void GUIAddLayerInfo(const MarkupLayerInfoC &layerInfo)
    { Body().GUIAddLayerInfo(layerInfo); }
    //: Add a layer info descriptor
    
    bool LayerInfo(const IntT layerId, MarkupLayerInfoC &layerInfo)
    { return Body().LayerInfo(layerId, layerInfo); }
    //: Get a layer info descriptor by layer id
    // Returns false if id not found
    
    void SetSelectedLayer(IntT selectedLayerId)
    { Body().SetSelectedLayer(selectedLayerId); }
    //: Set the selected layer id
    
    bool GUIShowLayerDialog()
    { return Body().GUIShowLayerDialog(); }
    //: Show the layer properties dialog
    
    bool LayerDialogShowLine(const bool showLine)
    { return Body().LayerDialogShowLine(showLine); }
    //: Should the layer editor dialog show the line setting?
    
    bool FrameContains(const StringC &key, const StringC val)
    { return Body().FrameContains(key, val); }
    //: Does the markup info contain a key with the specified val?
    
    friend class GUIMarkupCanvasBodyC;
  };
}


#endif
