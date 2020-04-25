// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: GUIMarkupCanvas.cc 3762 2004-07-26 17:47:55Z charles $"
//! lib=RavlGUI2D
//! file="RavlSoft/GUI/GUIMarkupCanvas.cc"

#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/HSetExtra.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/GUI/ReadBack.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/GUI/GUIMarkupLayerEditor.hh"

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN
{
  
  
  
  static bool DepthCompare(const MarkupInfoC &m0, const MarkupInfoC &m1)
  {
    return (m0.ZOrder() < m1.ZOrder());
  }

  static bool MyDeleteGC(GdkGC *gc) {
    gdk_gc_unref(gc);
    return true;
  }
  
  //: Constructor.
  
  GUIMarkupCanvasBodyC::GUIMarkupCanvasBodyC(int rows,int cols)
    : RawZoomCanvasBodyC(rows,cols),
      backgroundDrag(false),
      selectBox(false),
      gcDrawContext(0),
      fitToFrame(true),
      sigMarkupUpdate(0,MarkupInfoC()),
      sigSelection(DListC<Int64T>()),
      sigDisplayRange(RealRange2dC()),
      sigBackMenu(MouseEventC()),
      m_sigDoublePress(MouseEventC()),
      m_selectedLayerId(-1),
      m_dialogshowLine(true),
      m_aspectRatio(0)
  { ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::GUIMarkupCanvasBodyC(), \n"); }
  
  //: Destructor.
  
  GUIMarkupCanvasBodyC::~GUIMarkupCanvasBodyC() {
    connections.DisconnectAll();
    if(gcDrawContext != 0) {
      if(Manager.IsGUIThread())
        gdk_gc_unref(gcDrawContext);
      else
        Manager.Queue(Trigger(MyDeleteGC,gcDrawContext));
    }
  }
  
  //: Handle widget destruction
  
  void GUIMarkupCanvasBodyC::Destroy() {
    connections.DisconnectAll();
    RawZoomCanvasBodyC::Destroy();
  }
  
  //: Do the common creation stuff.
  
  bool GUIMarkupCanvasBodyC::CommonCreate() {
    connections += ConnectRef(Signal("expose_event"),*this,&GUIMarkupCanvasBodyC::EventExpose);
    connections += ConnectRef(Signal("button_press_event"),*this,&GUIMarkupCanvasBodyC::EventMousePress);
    connections += ConnectRef(Signal("button_release_event"),*this,&GUIMarkupCanvasBodyC::EventMouseRelease);
    connections += ConnectRef(Signal("motion_notify_event"),*this,&GUIMarkupCanvasBodyC::EventMouseMove);
    connections += ConnectRef(Signal("key_press_event"),*this,&GUIMarkupCanvasBodyC::EventKeyPress);
    connections += ConnectRef(Signal("scroll_event"),*this,&GUIMarkupCanvasBodyC::EventScroll);
    return true;
  }
  
  
  //: Create the widget.
  
  bool GUIMarkupCanvasBodyC::Create() {
    CommonCreate();
    if(!RawZoomCanvasBodyC::Create())
      return false;
    return true;
  }
  
  //: Create the widget. 
  bool GUIMarkupCanvasBodyC::Create(GtkWidget *widget) 
  {
    CommonCreate();
    if(!RawZoomCanvasBodyC::Create(widget))
      return false;
    return true;
  }
  
  //: Setup graphics context. 
  void GUIMarkupCanvasBodyC::SetupGC() 
  {
    if(Widget() == 0 || Widget()->window == 0)
      return; // Can't do anything yet.
    
    GdkGC *gcNormal = GUIDrawGCGrey();
    gcDrawContext = gdk_gc_new(Widget()->window);
    gdk_gc_copy(gcDrawContext,gcNormal);
    gdk_gc_set_line_attributes(gcDrawContext,2,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_MITER);
    GdkColor colour;
    colour.red = 65535;
    colour.blue = 65535;
    colour.green = 65535;
    gdk_gc_set_rgb_fg_color (gcDrawContext,&colour);
  }
  
  //: Add id to selection. 
  bool GUIMarkupCanvasBodyC::GUIAddSelect(Int64T id,bool signal) 
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(id >= 0 && selected.Insert(id)) 
    {
      if(signal) 
      {
        sigSelection(HSet2DList(selected));
        GUIRefresh();
      }
      
      return true;
    }
    return false;
  }
  
  //: Remove id from selection. 
  bool GUIMarkupCanvasBodyC::GUIDelSelect(Int64T id,bool signal) 
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(selected.Remove(id)) 
    {
      if(signal) 
        sigSelection(HSet2DList(selected));
      return true;
    }
    return false;    
  }
  
  //: Clear current selection.  
  bool GUIMarkupCanvasBodyC::GUIClearSelect(bool signal) 
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(selected.IsEmpty())
      return false;
    
    selected.Empty();
    if(signal) 
       sigSelection(DListC<Int64T>());
    return true;
  }

  //: Select object at given point.
  //!param: at - position to select.
  //!param: doRefresh - Refresh the screen 
  //!return: true if selection changed.  
  bool GUIMarkupCanvasBodyC::GUISelectObject(const Index2dC &at,bool enableRefresh) 
  {
    MouseEventC me(at[1].V(),at[0].V(),1,1); // Simulate a mouse press.
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::GUISelectObject, " << at << " me.At()=" << me.At() << " " << me.HasChanged(0) << " MPA=" << mousePressAt << " GUI2W=" << GUI2World(at));
    MarkupInfoC bestHit = FindTarget(GUI2World(at),me);
    if(!bestHit.IsValid())
      return false;
    //RealRange2dC updateArea;
    if(bestHit.Id() >= 0 && selected.Insert(bestHit.Id()))
      GUIRender(bestHit.Extent(*this));
    // Restore any state modified by MouseEvent
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::GUISelectObject, Selection= " << selected);
    return true;
  }

  //: Find target of click  
  MarkupInfoC GUIMarkupCanvasBodyC::FindTarget(const Point2dC &at,const MouseEventC &me) 
  {
    RealT bestSelectScore = -1;
    MarkupInfoC bestSelect;
  
    //Iterate over all the marked up examples in this frame and find the nearest example
    for(DLIterC<MarkupInfoC> it(m_frameMarkup.Markup());it;it++) 
    {
      RealT newScore = it->SelectTest(*this,at,me); // mousePressAt
      if(newScore < 0)
        continue;
      
      if(!bestSelect.IsValid() || (newScore < bestSelectScore)) 
      {
        bestSelectScore = newScore;
        bestSelect = *it;
      }
    }
    return bestSelect;
  }
  
  
  //: Mouse press.
  
  bool GUIMarkupCanvasBodyC::EventMousePress(MouseEventC &me) 
  { 
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventMousePress(), Called. \n");
    GUIGrabFocus();    
    
    if (me.IsDoublePressed())
    {
      m_sigDoublePress(me);
      return true;
    }

    if(me.HasChanged(2)) {
      sigBackMenu(me);
      return true;
    }

    mouseRawPressAt = me.At();
    mousePressAt = GUI2World(mouseRawPressAt);
    mouseLastEvent = mousePressAt;
    
    if(!m_mouseObj.IsValid())
      mouseState = 0;
    
    RealRange2dC updateArea;
    bool doRefresh = false; // Initialised update area ?
    bool gotHit = false;
    
    if(!m_mouseObj.IsValid()) 
    {
      //Get the closest pictograph example to the mouse click
      MarkupInfoC bestHit = FindTarget(mousePressAt,me);
      if(bestHit.IsValid()) 
      { // Try out best match
        gotHit = true;
        RealRange2dC orgRect = bestHit.Extent(*this);
        bool refresh = false;
        bool done = false;
        if(bestHit.MouseEventPress(*this,mousePressAt,me,mouseState,refresh)) 
	{
          m_mouseObj = bestHit;
          done = true;
        } 
	else 
	{
          m_mouseObj.Invalidate();
          mouseInfo.Invalidate();	
        }
        
	if(refresh) 
	{
          if(!doRefresh) 
	  {
            updateArea = orgRect;
            doRefresh = true;
          } 
	  else
	  {
            updateArea.Involve(orgRect);
	  }
          updateArea.Involve(bestHit.Extent(*this));
        }
        if(done) {
          if(doRefresh)
	  GUIRender(updateArea);
          return true;
        }
      }
    }
    
    if(me.HasChanged(0) && !m_mouseObj.IsValid())  //Left mouse button 
    {
      selectBox = true;
      if(!selected.IsEmpty() && (!me.IsCntrl() && !me.IsShift())) {
	GUIClearSelect();
	GUIRefresh();
	return true;
      }
    }
    if(doRefresh)
      GUIRender(updateArea);
    
    if(!gotHit && me.HasChanged(1)) //Middle mouse button 
    {  // Background hit ?
      mouseViewOffset = Offset();
      backgroundDrag = true;
      m_mouseObj.Invalidate();
      return true;
    }
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventMousePress(), Done  ");
    return true;
  }

  //: Mouse move.
  
  bool GUIMarkupCanvasBodyC::EventMouseMove(MouseEventC &me) 
  {
    //ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventMouseMove(), Called. \n");
    Point2dC at = GUI2World(me.At());
    mouseLastEvent = at;
    
    if(m_mouseObj.IsValid()) 
    {
      bool refresh = false;
      RealRange2dC updateArea = m_mouseObj.Extent(*this);
      if(m_mouseObj.MouseEventMove(*this,at,me,mouseState,refresh)) 
      {
        updateArea.Involve(m_mouseObj.Extent(*this));
        //Enlarge area by 1 *screen* pixel to eliminate quantising artefacts
        updateArea.RowRange() = updateArea.RowRange().Expand(1.0/scale[0]); 
        updateArea.ColRange() = updateArea.ColRange().Expand(1.0/scale[1]); 
      } 
      else 
      {
        m_mouseObj.Invalidate();
        mouseInfo.Invalidate();	
      }
      if(refresh)
        GUIRender(updateArea);
    }
    if(backgroundDrag) {
      GUISetOffset(mouseViewOffset + (Point2dC(me.At()) - mouseRawPressAt));
      RealRange2dC rng(GUI2World(Point2dC(0,0)),GUI2World(Size()));
      sigDisplayRange(rng);
    }
    
    if(selectBox) 
    {
      // Look at what to select...
      RealRange2dC box(mousePressAt,mouseLastEvent);
      if(box.TRow() > box.BRow())
        Swap(box.TRow(),box.BRow());
      if(box.LCol() > box.RCol())
        Swap(box.LCol(),box.RCol());
      HSetC<Int64T> oldSelected = selected.Copy();
      
      if(!me.IsCntrl() && !me.IsShift())
        GUIClearSelect(false);
      
      for(DLIterC<MarkupInfoC> it(m_frameMarkup.Markup());it;it++) 
      {
        if(it->Id() < 0)
          continue;
        if(box.Contains(it->Extent(*this))) 
          GUIAddSelect(it->Id(),false);
      }
      
      RealRange2dC redraw = box;
      redraw.Involve(lastSelectBox);
      GUIRender(redraw.Expand(2));
      lastSelectBox = box;
      
      if(oldSelected != selected)
        sigSelection(HSet2DList(selected));
    } 
    else 
    {
      if(backgroundDrag)
        GUIRefresh();
    }
    return true;
  }
  
  //: Mouse release.
  
  bool GUIMarkupCanvasBodyC::EventMouseRelease(MouseEventC &me) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventMouseRelease(), Called. ");
 //   std::cerr << "Release " << me.HasChanged(0) << " " << me.HasChanged(1) << " " << me.HasChanged(2) << " " << me.HasChanged(3) << " " << " " << me.HasChanged(4) << "\n";
    
    
    Point2dC at = GUI2World(me.At());
    mouseLastEvent = at;
    //RWLockHoldC hold(accessLock,RWLOCK_WRITE);
    if(m_mouseObj.IsValid()) 
    {
      RealRange2dC updateArea = m_mouseObj.Extent(*this);
      bool refresh = false;
      
      if(m_mouseObj.MouseEventRelease(*this,GUI2World(me.At()),me,mouseState,refresh)) 
      {
	updateArea.Involve(m_mouseObj.Extent(*this));
      } 
      else 
      {
	//Polygon selected or corner has been dragged
	m_mouseObj.Invalidate();
	mouseInfo.Invalidate();
      }
      if(refresh)
      {
	GUIRender(updateArea);
      }
    }
    if(selectBox) 
    {
      selectBox = false;
      GUIRefresh();
    }
    backgroundDrag = false;
    
    return true;
  }
  
  //: Scroll event
  
  bool GUIMarkupCanvasBodyC::EventScroll(ScrollEventC &event) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventScroll(), Called. ");
    Point2dC guiOn = Point2dC(event.Row(), event.Col());
    Point2dC worldOn = GUI2World(guiOn);
    
    switch(event.Direction()) {
    case ScrollDirectionUp:
    case ScrollDirectionLeft: {
      GUISetScale(Scale() * 1.2);
    } break;
    case ScrollDirectionDown:
    case ScrollDirectionRight:
      GUISetScale(Scale() * 0.8);
      break;
    default:
      break;
    }
    
    Vector2dC newOffset = guiOn - worldOn * Scale();
    GUISetOffset(newOffset);
    
    RealRange2dC rng(GUI2World(Point2dC(0,0)),GUI2World(Size()));
    sigDisplayRange(rng);
    
    backgroundDrag = false;
    GUIRefresh();
    return true;
  }
  
  //: Configure.
  
  bool GUIMarkupCanvasBodyC::EventConfigure(GdkEvent* &event) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventConfigure(), Called. ");
    if(!RawZoomCanvasBodyC::EventConfigure(event))
      return false;
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventConfigure(), Size=" << widgetSize << " ");
    RealRange2dC rng(GUI2World(Point2dC(0,0)),GUI2World(Size()));
    sigDisplayRange(rng);
    
    // Have we done a fit to canvas in the last half second ?
    if((DateC::NowUTC() - lastTimeOfFitToFrame).Double() < 0.8) { 
      // If so, redo fit for the display size.
      GUIFitImageToScreen(m_aspectRatio);
    }
    
    return true;
  }
  
  //: Expose of area.
  
  bool GUIMarkupCanvasBodyC::EventExpose(GdkEvent* &event) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventExpose(), Called. ");
    RealRange2dC range;
    IntT toFollow;
    TranslateExposeEvent(event,range,toFollow);    
    GUIRender(range);
    return true;
  }
  
  //: Handle key press events.
  
  bool GUIMarkupCanvasBodyC::EventKeyPress(GdkEventKey *&key) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventKeyPress(), Called. ");
    
    if(key->keyval == GDK_Escape) {
      RWLockHoldC hold(accessLock,RWLOCK_WRITE);
      
      return true;
    }
      
    return true;
  }
  
  //: Handle key release events.
  
  bool GUIMarkupCanvasBodyC::EventKeyRelease(GdkEventKey *&key) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::EventKeyRelease(), Called. ");
    return true;
  }
  
  //: Update the screen.
  
  bool GUIMarkupCanvasBodyC::Refresh() {
    Manager.Queue(Trigger(GUIMarkupCanvasC(*this),&GUIMarkupCanvasC::GUIRefresh));
    return true;
  }
  
  //: Update current markup.
  
  bool GUIMarkupCanvasBodyC::UpdateMarkup(const FrameMarkupC &markup) {
    Manager.Queue(Trigger(GUIMarkupCanvasC(*this),&GUIMarkupCanvasC::GUIUpdateMarkup,markup));
    return true;
  }
  
  //: Update current markup.
  
  bool GUIMarkupCanvasBodyC::GUIUpdateMarkup(const FrameMarkupC &markup) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::GUIUpdateMarkup(), Called. ");
    RWLockHoldC hold(accessLock,RWLOCK_WRITE);
    m_frameMarkup = markup;
    hold.Unlock();

    GUIRefresh();

    return true;
  }
  
  //: Fit current display to screen.
  
  bool GUIMarkupCanvasBodyC::GUIFitImageToScreen(RealT aspectRatio) {
    ONDEBUG(std::cerr << "GUIMarkupCanvasBodyC::GUIFitImageToScreen\n");
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    m_aspectRatio = aspectRatio;
    lastTimeOfFitToFrame = DateC::NowUTC();
    
    ImageC<ByteRGBValueC> img = m_frameMarkup.Image();
    if(img.Frame().Area() <= 0)
      return false;
    if(widgetSize.Area() <= 0)
      return false;
    Vector2dC newScale;
    Vector2dC imageSize((RealT) img.Frame().Rows(),(RealT) img.Frame().Cols());
    RealRange2dC rr(widgetSize);
    Vector2dC displaySize(rr.Rows(),rr.Cols());
    
    if(aspectRatio > 0.0001) {
      RealT imageRatio;
      if(imageSize[0] > 0 && imageSize[1] > 0)
        imageRatio = imageSize[1]/imageSize[0];
      else
        imageRatio = 1;
      
      RealT pixelAspectRatio = aspectRatio / imageRatio;
      newScale = Vector2dC(displaySize[0] / imageSize[0],
                           displaySize[1] / imageSize[1]);
      RealT minScale = Min(newScale[0],newScale[1] / pixelAspectRatio);
      newScale = Vector2dC(minScale,minScale * pixelAspectRatio);
    } else {
      newScale = Vector2dC(displaySize[0] / imageSize[0],
                           displaySize[1] / imageSize[1]);
      RealT minScale = Min(newScale[0],newScale[1]);
      newScale = Vector2dC(minScale,minScale);
    }
    
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GUIMarkupCanvasBodyC::GUIFitImageToScreen(), Scale =" << newScale << " AspectRatio=" << aspectRatio << " ");
    Vector2dC slack = displaySize - Vector2dC(newScale[0] * imageSize[0],newScale[1] * imageSize[1]);
    
    GUISetScale(newScale);
    GUISetOffset(Vector2dC(Point2dC(img.Frame().Origin()))+(slack/2));

    GUIRefresh();
    return true;
  }
  
  //: Fit current display to screen.
  
  bool GUIMarkupCanvasBodyC::FitImageToScreen(RealT aspectRatio) {
    Manager.Queue(Trigger(GUIMarkupCanvasC(*this),&GUIMarkupCanvasC::GUIFitImageToScreen,aspectRatio));
    return true;
  }
  
  //: Set the current aspect ratio of the displayed image.
  
  bool GUIMarkupCanvasBodyC::GUISetAspectRatio(RealT aspectRatio) {
    ONDEBUG(std::cerr << "GUIMarkupCanvasBodyC::GUISetAspectRatio\n");
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    m_aspectRatio = aspectRatio;
    
    // We have to compute the pixel aspect ratio from the size of the current image.
    RealT pixelAspectRatio = 1.0;
    if(aspectRatio > 0.0001) {
      ImageC<ByteRGBValueC> img = m_frameMarkup.Image();
      if(img.Frame().Area() > 0) {
        Vector2dC newScale;
        Vector2dC imageSize((RealT) img.Frame().Rows(),(RealT) img.Frame().Cols());
        RealT imageRatio;
        if(imageSize[0] > 0 && imageSize[1] > 0)
          imageRatio = imageSize[1]/imageSize[0];
        else
          imageRatio = 1;
        pixelAspectRatio = aspectRatio / imageRatio;
      }
    }
    
    // Modify the displayed ratio.
    Vector2dC tmpScale = Scale();
    tmpScale = Vector2dC(tmpScale[0],tmpScale[0] * pixelAspectRatio);
    GUISetScale(tmpScale);
    return true;
  }
  
  
  //: Set the current aspect ratio
  
  bool GUIMarkupCanvasBodyC::GUIFixAspectRatio(RealT aspectRatio) {
    ONDEBUG(std::cerr << "UIMarkupCanvasBodyC::GUIFixAspectRatio\n");
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    // Modify the displayed ratio.
    Vector2dC tmpScale = Scale();
    tmpScale = Vector2dC(tmpScale[0],tmpScale[0] * aspectRatio);
    GUISetScale(tmpScale);
    return true;
  }
  
  //: Update part of the display
  
  bool GUIMarkupCanvasBodyC::GUIRender(const RealRange2dC &range) {
    if(widget == 0) // Are we ready to go ?
      return true;
    
    RWLockHoldC hold(accessLock,RWLOCK_READONLY);
    if(gcDrawContext == 0) {
      if(Widget() == 0 || Widget()->window == 0)
        return true; // Can't do anything yet.
      SetupGC();
    }
    
    // Markup
    RealRange2dC rrange = range;
    DListC<MarkupInfoC> renderList;

    // Update the coverage
    UpdateCoverage();
    
    // Iterate through the markup objects, last first
    DLIterC<MarkupInfoC> itMarkup(m_frameMarkup.Markup());
    SArray1dC<bool> renderFlag(m_frameMarkup.Markup().Size());
    renderFlag.Fill(false);
    SArray1dIterC<bool> itRenderFlag(renderFlag);
    bool advance = true;
    for (itMarkup.First(); itMarkup;)
    {
      advance = true;
      
      // Only check if markup not currently flagged for rendering
      if(!*itRenderFlag)
      {
        // Ignore the markup if it's on the remove list, or isn't overlapped by our render area
        if (!removeZOrder[itMarkup->ZOrder()] && rrange.IsOverlapping(itMarkup->Extent(*this)))
        {
          // Flag it for rendering
          *itRenderFlag = true;
          
          // Can the markup be partially rendered?
          if(!itMarkup->CanClip())
          {
            // Whole markup needs to be rendered, so expand our render area
            rrange.Involve(itMarkup->Extent(*this));
            
            // We need to re-evaluate any objects that were previously ignored
            itMarkup.First();
            itRenderFlag.First();
            advance = false;
          }
        }
      }
      
      // Next 
      if (advance)
      {
        itRenderFlag++;
        itMarkup++;
      }
    }
    
    // Build the render list in order
    itRenderFlag.First();
    itMarkup.First();
    for (;itRenderFlag && itMarkup;)
    {
      if (*itRenderFlag)
      {
        renderList.InsLast(*itMarkup);
      }
      itRenderFlag++;
      itMarkup++;
    }
    
    // Depth sort
    renderList.MergeSort(DepthCompare);
    
    // Make sure redraw area lies on a pixel boundry
    rrange = GUI2World(World2GUI(rrange).IndexRange());
    
    // Work out the local background.
    RealRange2dSetC uncoveredBackground = RealRange2dSetC(DisplayArea()).Subtract(coverage);

    // Draw background as black.
    for(DLIterC<RealRange2dC> it(uncoveredBackground);it;it++)
    {
      RealRange2dC backgroundPatch(*it);
      backgroundPatch.ClipBy(rrange);
      if (backgroundPatch.Area() > 0)
      {
        GUIDrawRectangle(GUIDrawGCBlack(),backgroundPatch,true);
      }
    }
    
    // Render to canvas
    for(DLIterC<MarkupInfoC> it(renderList);it;it++)
    {
      if(removeZOrder[it->ZOrder()])
        continue;
      if (it->Id() >=0 && selected.IsMember(it->Id()))
      {
        it->PushLayer(m_selectedLayerId);
        it->Render(*this,rrange,true);
        it->PopLayer();
      }
      else
      {
        it->Render(*this,rrange,false);
      }
    }

    if(selectBox)
    {
      RealRange2dC box(mousePressAt,mouseLastEvent);
      if(box.TRow() > box.BRow())
        Swap(box.TRow(),box.BRow());
      if(box.LCol() > box.RCol())
        Swap(box.LCol(),box.RCol());
      GUIDrawRectangle(GUIDrawGCWhite(),box);
    }

    return true;
  }
  
  //: Update the screen.
  
  bool GUIMarkupCanvasBodyC::GUIRefresh() {
    ONDEBUG(std::cerr << "GUIMarkupCanvasBodyC::GUIRefresh()\n");
    if(widget == 0) // Are we ready to go ?
      return true; 
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    displayArea = DisplayArea();
    if(displayArea.Range1().Size() < 0 || displayArea.Range2().Size() < 0) {
      //RavlSysLog(SYSLOG_WARNING) << "GUIMarkupCanvasBodyC::GUIRefresh(), Negative display area=" << displayArea << " ";
      return true;
    }
    GUIRender(displayArea);
    return true;
  }

  //: Access list of selected polygons
  
  DListC<Int64T> GUIMarkupCanvasBodyC::Selected() const {
    ReadBackLockC lock;
    DListC<Int64T> ret = HSet2DList(selected);
    return ret;
  }
  
  //: Set the current selection.
  
  bool GUIMarkupCanvasBodyC::SetSelection(const DListC<Int64T> &list,bool sigChanged) {
    Manager.Queue(Trigger(GUIMarkupCanvasC(*this),&GUIMarkupCanvasC::GUISetSelection,list,sigChanged));
    return true;
  }
  
  //: Set the current selection.
  
  bool GUIMarkupCanvasBodyC::GUISetSelection(const DListC<Int64T> &list,bool sigChanged) 
  {
    ONDEBUG(RavlSysLog(SYSLOG_WARNING) << "GUIMarkupCanvasBodyC::GUISetSelection(), Called.  List.Size()=" << list.Size() << " sigChanged=" << sigChanged);
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    HSetC<Int64T> newSelected = DList2HSet(list);
    if(newSelected == selected) // Same as old selection ?
      return true;
    selected = newSelected;
    if(sigChanged) sigSelection(list);
    GUIRefresh();
    return true;
  }
  
  //: Exclude a Z order layer from display.
  
  bool GUIMarkupCanvasBodyC::ExcludeZOrder(IntT zOrder) {
    Manager.Queue(Trigger(GUIMarkupCanvasC(*this),&GUIMarkupCanvasC::GUIExcludeZOrder,zOrder));
    return true;
  }
  
  //: Include a Z order layer in display.
  
  bool GUIMarkupCanvasBodyC::IncludeZOrder(IntT zOrder) {
    Manager.Queue(Trigger(GUIMarkupCanvasC(*this),&GUIMarkupCanvasC::GUIIncludeZOrder,zOrder));
    return true;
  }
  
  //: Exclude a Z order layer from display.
  
  bool GUIMarkupCanvasBodyC::GUIExcludeZOrder(IntT zOrder) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    bool ok = removeZOrder.Insert(zOrder);
    if(ok)
    {
      GUIRefresh();
    }
    return ok;
  }
  
  //: Include a Z order layer in display.
  
  bool GUIMarkupCanvasBodyC::GUIIncludeZOrder(IntT zOrder) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    bool ok = removeZOrder.Remove(zOrder);
    if(ok)
    {
      GUIRefresh();
    }
    return ok;
  }


  
  bool GUIMarkupCanvasBodyC::ZOrderExcluded(const IntT zOrder)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    return removeZOrder.Contains(zOrder);
  }
  
  
  
  void GUIMarkupCanvasBodyC::UpdateCoverage()
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    coverage.Empty();
    for (DLIterC<MarkupInfoC> it(m_frameMarkup.Markup()); it; it++)
    {
      if (!removeZOrder[it->ZOrder()] && it->Coverage(*this).Area() > 0)
      {
        coverage += it->Coverage(*this);
      }
    }
  }



  void GUIMarkupCanvasBodyC::GUIAddLayerInfo(const MarkupLayerInfoC &layerInfo)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    m_layerInfo.InsLast(layerInfo);
    // TODO: Update the dialog
  }
  
  
  
  bool GUIMarkupCanvasBodyC::LayerInfo(const IntT layerId, MarkupLayerInfoC &layerInfo)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    bool found = false;
    for (DLIterC<MarkupLayerInfoC> it(m_layerInfo); it; it++)
    {
      if (layerId == it->Id())
      {
        layerInfo = *it;
        found = true;
        break;
      }
    }
    return found;
  }
  
  
  
  void GUIMarkupCanvasBodyC::SetSelectedLayer(IntT selectedLayerId)
  {
    m_selectedLayerId = selectedLayerId;
  }
  
  
  
  bool GUIMarkupCanvasBodyC::GUIShowLayerDialog()
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GUIMarkupCanvasC ref(*this);
    GUIMarkupLayerEditorC layerEditor(ref, m_dialogshowLine);
    layerEditor.GUISetLayerList(m_layerInfo);
    ConnectRef(layerEditor.SigVisibilityChanged(), *this, &GUIMarkupCanvasBodyC::GUISetLayerVisibility);
    ConnectRef(layerEditor.SigLayerInfoChanged(), *this, &GUIMarkupCanvasBodyC::GUISetLayerInfo);
    layerEditor.GUIShow();
    return true;
  }
  
  
  
  bool GUIMarkupCanvasBodyC::GUISetLayerVisibility(IntT zOrder, bool visible)
  {
    if (zOrder > 0)
    {
      if (visible)
      {
        GUIIncludeZOrder(zOrder);
      }
      else
      {
        GUIExcludeZOrder(zOrder);
      }
    }
    return true;
  }
  
  
  
  bool GUIMarkupCanvasBodyC::GUISetLayerInfo(MarkupLayerInfoC layerInfo)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if (layerInfo.Id() >= 0)
    {
      for (DLIterC<MarkupLayerInfoC> it(m_layerInfo); it; it++)
      {
        if (it->Id() == layerInfo.Id())
        {
          *it = layerInfo;
          break;
        }
      }
      
      GUIRefresh();
    }
    
    return true;
  }
  
  
    
  bool GUIMarkupCanvasBodyC::LayerDialogShowLine(const bool showLine)
  {
    m_dialogshowLine = showLine;
    return true;
  }
  
  
  
  bool GUIMarkupCanvasBodyC::FrameContains(const StringC &key, const StringC val)
  {
    // Make sure markup doesn't change while we're checking.
    RWLockHoldC hold(accessLock,RWLOCK_READONLY);

    for (DLIterC<MarkupInfoC> it(m_frameMarkup.Markup()); it; it++)
    {
      StringC strVal;
      if (it->Info().Lookup(key, strVal))
        if (strVal == val)
          return true;
    }
    return false;
  }
  
  
  
}
