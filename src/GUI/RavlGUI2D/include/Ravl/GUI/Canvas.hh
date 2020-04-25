// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_CANVAS_HEADER
#define RAVLGUI_CANVAS_HEADER 1
////////////////////////////////////////////////
//! docentry="Ravl.API.Graphics.GTK.Control"
//! file="Ravl/GUI/2D/Canvas.hh"
//! lib=RavlGUI2D
//! author="Charles Galambos"
//! example=exCanvas.cc
//! date="17/03/1999"
//! rcsid="$Id$"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/DList.hh"
#include "Ravl/Trigger.hh"
#include <gdk/gdktypes.h>
#include <gdk/gdkgc.h>

namespace RavlGUIN {
  using namespace RavlImageN;
  class CanvasC;
  
  //! userlevel=Develop
  //: Widget in which graphics can be rendered.
  
  class CanvasBodyC 
    : public WidgetBodyC 
  {
  public:
    CanvasBodyC(int width,int height,bool direct = false);
    //: Constructor.
    // If direct rendering is enabled (true) it is the user's responsibity to provide an 
    // expose method which updates the window on the display.
    
    CanvasBodyC(const ImageRectangleC& rect, bool direct = false);
    //: Constructor.
    
    virtual ~CanvasBodyC();
    //: Destructor.
    
    GtkWidget *DDrawArea() 
    { return widget; }
    //: Direct draw area.
  
    GdkPixmap * &Pixmap() 
    { return pixmap; }
    //: Pixmap.
    
    GdkDrawable *DrawArea();
    //: Get draw area.
    
    GdkColor &GetColour(int n);
    //: Access colour.
    // GUI thread only. (n=65 is black)
    
    bool Resize(int width,int height);
    //: Resize canvas.
    // The canvas will be resized on the next render event.
    
    bool GUIResize(int &width,int &height);
    //: Resize canvas.
    // GUI Thread only
    
    bool &ConfigDone() 
    { return configDone; }
    //: Flag configure as done.
    
    GdkGC *DrawGC() 
    { return drawGC; }
    //: Drawing context.
    
    void DoSomeSetup();
    //: Don't call directly!!!!!!!!!!
    
    virtual void DrawImage(ImageC<ByteT> &img,Index2dC offset=Index2dC(0,0));
    //: Draw an image on the canvas.
    
    virtual void DrawImage(const ImageC<ByteRGBValueC> &img,Index2dC offset=Index2dC(0,0));
    //: Draw an rgb image on the canvas.
    
    void DrawLine(IntT col1,IntT row1,IntT col2,IntT row2,IntT colourId = 0); 
    //: Draw a line.
    
    void DrawArc(ImageRectangleC rect, IntT start, IntT angle, IntT colourId = 0, bool fill = false); 
    //: Draw an arc.
    // The rectangle defines an ellipse in the image
    // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
    // angle is the length of the arc, in 1/64ths of a degree
    // If fill is true, the arc is a pie, otherwise it's a line arc
    
    void DrawRectangle(IntT col1,IntT row1,IntT col2,IntT row2,IntT colourId = 0); 
    //: Draw a filled rectangle.
    
    void DrawFrame(IntT col1,IntT row1,IntT col2,IntT row2,IntT colourId = 0); 
    //: Draw an empty rectangle.
    
    void DrawText(IntT col,IntT row,StringC text,IntT colourId = 0);
    //: Draw some text
    
    bool GUIDrawImage(ImageC<ByteT> &img,const Index2dC &offset);
    //: Draw an image on the canvas.
    // Call with GUI thread only!
    
    bool GUIDrawRGBImage(ImageC<ByteRGBValueC> &img,const Index2dC &offset);
    //: Draw an rgb image on the canvas.
    // Call with GUI thread only!
    
    bool GUIDrawLine(const IntT &col1,const IntT &row1,const IntT &col2,const IntT &row2,const IntT &colourId); 
    //: Draw a line.
    // Call with GUI thread only!

    bool GUIDrawLine(Index2dC start, Index2dC end, IntT colourId); 
    //: Draw a line.
    // Call with GUI thread only!

    
    bool GUIDrawArc(const ImageRectangleC& rect, const IntT& start, const IntT& angle, const IntT& colourId, const bool& fill); 
    //: Draw an arc.
    // The rectangle defines an ellipse in the image
    // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
    // angle is the length of the arc, in 1/64ths of a degree
    // If fill is true, the arc is a pie, otherwise it's a line arc
    
    bool GUIDrawText(const IntT &col,const IntT &row,const StringC &text,const IntT &colourId);
    //: Draw some text
    // Call with GUI thread only!

    void SetLineStyle(IntT iWidth, GdkLineStyle linestyle=GDK_LINE_SOLID, GdkCapStyle capstyle=GDK_CAP_NOT_LAST, GdkJoinStyle joinstyle=GDK_JOIN_MITER);
    //: Set line style - GUI thead only
    // Values for linestyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKLINESTYLE">here</a>
    // Values for capstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKCAPSTYLE">here</a>
    // Values for joinstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKJOINSTYLE">here</a>

    bool GUISetLineStyle(IntT& iWidth, GdkLineStyle& linestyle, GdkCapStyle& capstyle, GdkJoinStyle& joinstyle);
    //: Set line style - GUI thead only
    // Values for linestyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKLINESTYLE">here</a>
    // Values for capstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKCAPSTYLE">here</a>
    // Values for joinstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKJOINSTYLE">here</a>
    
    bool GUIDrawRectangle(const IntT &col1,const IntT &row1,const IntT &col2,const IntT &row2,const IntT &colourId); 
    //: Draw a filled rectangle.
    
    bool GUIDrawFrame(const IntT &col1,const IntT &row1,const IntT &col2,const IntT &row2,const IntT &colourId); 
    //: Draw an empty rectangle

    bool GUIDrawFrame(Index2dC topLeft, Index2dC bottomRight, IntT colourId); 
    //: Draw an empty rectangle

    bool GUIClear();
    //: Clear canvas to given colour.
    
    bool Clear();
    //: Clear canvas to given colour.
    
    bool IsReady() const { 
      if(widget == 0) return false;
      if(direct) return true;
      if (pixmap == NULL) return false;
      return initialExposureDone;
    }
    //: Is Canvas ready for drawing.
    
    DListC<TriggerC> &ToDo()
    { return toDo; }
    //: Get todo list.
    
    bool GUIAutoRefresh(bool &val);
    //: Turn auto refresh after draw routines on/off.
    // Call with GUI thread only!
    // Note: Turning autorefresh on, automatically refreshes the display.
    
    void AutoRefresh(bool val);
    //: Turn auto refresh after draw routines on/off.
    // Note: Turning autorefresh on, automatically refreshes the display.
    
    bool AutoRefresh() const
    { return autoRefresh; }
    //: Is auto refresh on ?
    
    bool GUIRefresh();
    //: Refresh display.
    // GUI Thread only.
    
    bool Refresh();
    //: Refresh display.
    
    void SetInitialExposureDone(bool done) {
      initialExposureDone = done;
    }
    //: Say the canvas has been exposed

    bool InitialExposureDone() const {
      return initialExposureDone;
    }
    //: Say the canvas has been exposed

  protected:
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create with a widget supplied from elsewhere.

    virtual void WidgetDestroy();
    //: Called when the underlying widget it destroyed.
    // The default version of this method simpily 0's the widget ptr.
    
    void AddToQueue(TriggerC &trigger);
    //: Add a trigger to the to-do list
    
    int sx,sy;  // NB these violate RAVL coord convention: sx is col, sy is row
    bool direct;
    bool initialExposureDone;
    
    GdkPixmap *pixmap;       // Map.
    bool configDone;
    
    void SetupColours();
    //: Setup colour table.
    
    GdkGC *drawGC;
    SArray1dC<GdkColor> colourTab; // Colour table.
    
    DListC<TriggerC> toDo; // List of things to do as soon as we're initalised.

    bool autoRefresh;
    GdkColor m_gdkBlack;    
    friend class CanvasC;
  };
  
  //! userlevel=Normal
  //: Widget in which graphics can be rendered.
  
  class CanvasC 
    : public WidgetC 
  {
  public:
    CanvasC()
    {}
    //: Default constructor.
    // NB. This creates an invalid handle.
    
    CanvasC(int width,int height,bool direct = false);
    //: Constructor.
    // If direct rendering is enabled (true) it is the user's responsibility to provide an
    // expose method which updates the window on the display.
    
    CanvasC(const ImageRectangleC& rect, bool direct = false);
    //: Constructor.
    // If direct rendering is enabled (true) it is the user's responsibility to provide an
    // expose method which updates the window on the display.<br>
    // <b>N.B.:</b> Only the image size is used from <code>rect</code>.  The offset from the origin is ignored.

    CanvasC(CanvasBodyC &body)
      : WidgetC(body)
    {}
    //: Body constructor.
    
  protected:
    
    CanvasBodyC &Body() 
    { return static_cast<CanvasBodyC &>(WidgetC::Body()); }
    
    const CanvasBodyC &Body() const
    { return static_cast<const CanvasBodyC &>(WidgetC::Body()); }
    
  public:
    GdkDrawable *DrawArea() 
    { return Body().DrawArea(); }
    //: Get drawing area.
    
    bool GUIResize(int &width,int &height)
    { return Body().GUIResize(width,height); }
    //: Resize canvas.
    // Call with the GUI Thread only
    
    bool Resize(int width,int height)
    { return Body().Resize(width,height); }
    //: Resize canvas.
    
    GdkColor &GetColour(int n)
    { return Body().GetColour(n); }
    //: Access colour.
    // GUI thread only.
    
    GdkGC *DrawGC() 
    { return Body().DrawGC(); }
    //: Drawing context.
    // GUI thread only.
    
    void DrawImage(ImageC<ByteT> &img,Index2dC offset=Index2dC(0,0))
    { Body().DrawImage(img,offset); }
    //: Draw an image on the canvas.
    
    void DrawImage(const ImageC<ByteRGBValueC> &img,Index2dC offset=Index2dC(0,0))
    { Body().DrawImage(img,offset); }
    //: Draw an rgb image on the canvas.

    bool DrawRGBImage(const ImageC<ByteRGBValueC> &img)
    { Body().DrawImage(img,Index2dC(0,0)); return true; }
    //: Draw an rgb image on the canvas.
    
    void DrawLine(IntT col1,IntT row1,IntT col2,IntT row2,IntT colourId = 0)
    { Body().DrawLine(col1,row1,col2,row2,colourId); }
    //: Draw a line.
    
    void DrawArc(ImageRectangleC rect, IntT start, IntT angle, IntT colourId = 0, bool fill = false)
    { Body().DrawArc(rect,start,angle,colourId,fill); }
    //: Draw an arc.
    // The rectangle defines an ellipse in the image
    // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
    // angle is the length of the arc, in 1/64ths of a degree
    // If fill is true, the arc is a pie, otherwise it's a line arc

    void DrawText(IntT col,IntT row,StringC text,IntT colourId = 0)
    { Body().DrawText(col,row,text,colourId); }
    //: Draw a line.
    
    void DrawRectangle(IntT col1,IntT row1,IntT col2,IntT row2,IntT colourId = 0)
    { Body().DrawRectangle(col1,row1,col2,row2,colourId); }
    //: Draw a filled rectangle
    
    void DrawFrame(IntT col1,IntT row1,IntT col2,IntT row2,IntT colourId = 0)
    { Body().DrawFrame(col1,row1,col2,row2,colourId); }
    //: Draw an empty rectangle
    
    bool GUIDrawLine(const IntT &col1,const IntT &row1,const IntT &col2,const IntT &row2,const IntT &colourId)
    { return Body().GUIDrawLine(col1,row1,col2,row2,colourId); }
    //: Draw a line.

    bool GUIDrawLine(const Index2dC &p1,const Index2dC &p2,const IntT &colourId)
    { return Body().GUIDrawLine(  p1.Row().V(),  p1.Col().V(), p2.Row().V(),p2.Col().V(),colourId); }
    //: Draw a line.
    // Call with GUI thread only!
    
    bool GUIDrawArc(const ImageRectangleC& rect, const IntT& start, const IntT& angle, const IntT& colourId, const bool& fill)
    { return Body().GUIDrawArc(rect,start,angle,colourId,fill); }
    //: Draw an arc.
    // The rectangle defines an ellipse in the image
    // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
    // angle is the length of the arc, in 1/64ths of a degree
    // If fill is true, the arc is a pie, otherwise it's a line arc

    void SetLineStyle(IntT iWidth, GdkLineStyle linestyle=GDK_LINE_SOLID, GdkCapStyle capstyle=GDK_CAP_NOT_LAST, GdkJoinStyle joinstyle=GDK_JOIN_MITER)
    { Body().SetLineStyle(iWidth,linestyle,capstyle,joinstyle); }
    //: Set line style - GUI thead only
    // Values for linestyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKLINESTYLE">here</a>
    // Values for capstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKCAPSTYLE">here</a>
    // Values for joinstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKJOINSTYLE">here</a>

    bool GUISetLineStyle(IntT& iWidth, GdkLineStyle& linestyle, GdkCapStyle& capstyle, GdkJoinStyle& joinstyle) 
    { return Body().GUISetLineStyle(iWidth,linestyle,capstyle,joinstyle); }
    //: Set line style - GUI thead only
    // Values for linestyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKLINESTYLE">here</a>
    // Values for capstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKCAPSTYLE">here</a>
    // Values for joinstyle are <a href="http://developer.gnome.org/doc/API/gdk/gdk-graphics-contexts.html#GDKJOINSTYLE">here</a>

    bool GUIDrawRectangle(const IntT &col1,const IntT &row1,const IntT &col2,const IntT &row2,const IntT &colourId)
    { return Body().GUIDrawRectangle(col1,row1,col2,row2,colourId); }
    //: Draw a filled rectangle.
    
    bool GUIDrawFrame(const IntT &col1,const IntT &row1,const IntT &col2,const IntT &row2,const IntT &colourId)
    { return Body().GUIDrawFrame(col1,row1,col2,row2,colourId); }
    //: Draw an empty rectangle.
    
    bool GUIDrawImage(ImageC<ByteT> &img,const Index2dC &offset)
    { return Body().GUIDrawImage(img,offset); }
    //: Draw an image on the canvas.
    // Call with GUI thread only!
    
    bool GUIDrawRGBImage(ImageC<ByteRGBValueC> &img,const Index2dC &offset)
    { return Body().GUIDrawRGBImage(img,offset); }
    //: Draw an rgb image on the canvas.
    // Call with GUI thread only!
    
    bool GUIDrawText(const IntT &col,const IntT &row,const StringC &text,const IntT &colourId)
    { return Body().GUIDrawText(col,row,text,colourId); }
    //: Draw some text.
    // Call with GUI thread only!
    
    bool GUIAutoRefresh(bool &val)
    { return Body().GUIAutoRefresh(val); }
    //: Turn auto refresh after draw routines on/off.
    // Call with GUI thread only!
    // Note: Turning autorefresh on, automaticly refreshes the display.
    
    bool GUIClear()
    { return Body().GUIClear(); }
    //: Clear canvas to given colour.
    
    bool Clear()
    { return Body().Clear(); }
    //: Clear canvas to given colour.
    
    bool IsReady() const 
    { return Body().IsReady(); }
    //: Is Canvas ready for drawing ?

    void AutoRefresh(bool val)
    { Body().AutoRefresh(); }
    //: Turn auto refresh after draw routines on/off.
    
    bool AutoRefresh() const
    { return Body().AutoRefresh(); }
    //: Is auto refresh on ?

    bool GUIRefresh()
    { return Body().GUIRefresh(); }
    //: Refresh display.
    // Call with GUI thread only!
    
    bool Refresh()
    { return Body().Refresh(); }
    //: Refresh display.
    
    friend class CanvasBodyC;
  };
}

#endif
