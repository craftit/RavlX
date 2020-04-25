// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/RawCanvas.cc"

#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  //: Create the widget.
  
  bool RawCanvasBodyC::Create() {
    if(widget != 0)
      return true; // Done already.
    widget =  gtk_drawing_area_new ();
    gtk_drawing_area_size(GTK_DRAWING_AREA(widget),sx,sy);
    ConnectSignals();
    return true;
  }

  //: Create the widget.
  
  bool RawCanvasBodyC::Create(GtkWidget *nwidget) {
    widget = nwidget;
    ConnectSignals();
    return true;
  }


  //: Access draw area.
  
  GdkDrawable *RawCanvasBodyC::DrawArea()
  { return widget->window; }
  
  //: Access drawing context.
  
  GdkGC *RawCanvasBodyC::GUIDrawGC() {
    RavlAssert(Widget() != 0);
    return Widget()->style->base_gc[0];
    //widget->style->font
  }
  
  //: Get black drawing context.
  
  GdkGC *RawCanvasBodyC::GUIDrawGCBlack() {
    RavlAssert(Widget() != 0);
    return Widget()->style->black_gc;
  }
  
  //: Get white drawing context.
  
  GdkGC *RawCanvasBodyC::GUIDrawGCWhite() {
    RavlAssert(Widget() != 0);
    return Widget()->style->white_gc;
  }
  
  //: Get grey drawing context.
  
  GdkGC *RawCanvasBodyC::GUIDrawGCGrey() {
    RavlAssert(Widget() != 0);
    return Widget()->style->mid_gc[0];
  }
  
  //: Font to use.
  
  GdkFont *RawCanvasBodyC::GUIDrawFont() {
    RavlAssert(Widget() != 0);
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    GdkFont *cfont = gtk_style_get_font(widget->style);
    return cfont;
  }
  
  //: Get get draw area.
  
  GdkWindow *RawCanvasBodyC::GUIDrawWindow() {
    RavlAssert(Widget() != 0);
    return Widget()->window;    
  }

  //: Draw an arc.
  // The rectangle defines an ellipse in the image
  // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
  // angle is the length of the arc, in 1/64ths of a degree
  // If fill is true, the arc is a pie, otherwise it's a line arc
  
  void RawCanvasBodyC::GUIDrawArc(GdkGC *gc,const IndexRange2dC &rect, IntT start, IntT angle, bool fill) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_draw_arc (GUIDrawWindow(),
                  gc,
                  fill,
                  rect.Origin().Col().V(),
                  rect.Origin().Row().V(),
                  rect.Size().Col().V(),
                  rect.Size().Row().V(),
                  start,
                  angle);
    
  }
  
  //: Draw a line.
  
  void RawCanvasBodyC::GUIDrawLine(GdkGC *gc,Index2dC p1,Index2dC p2) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_draw_line(GUIDrawWindow(),gc,p1[1].V(),p1[0].V(),p2[1].V(),p2[0].V());
  }
  
  //: Draw a rectangle.
  
  void RawCanvasBodyC::GUIDrawRectangle(GdkGC *gc,const IndexRange2dC &rect,bool fill) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_draw_rectangle(GUIDrawWindow(),
		       gc,fill,
		       rect.LCol().V(),rect.TRow().V(),
		       rect.Cols(),rect.Rows());
  }
  
  //: Draw a rectangle.
  
  void RawCanvasBodyC::GUIDrawRectangle(GdkGC *gc,Index2dC topLeft,Index2dC bottomRight,bool fill) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    Index2dC size = (bottomRight - topLeft) + Index2dC(1,1);
    gdk_draw_rectangle(GUIDrawWindow(),
		       gc,fill,
		       topLeft[1].V(),topLeft[0].V(),
		       size[1].V(),size[0].V()
		       ); 
  }
  
  //: Draw text.
  
  void RawCanvasBodyC::GUIDrawText(GdkGC *gc,GdkFont *font,Index2dC at,const StringC &txt) {    
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_draw_text(GUIDrawWindow(),
		  font,gc,
		  at[1].V(),at[0].V(),
		  txt.chars(),txt.Size());
  }
  
  //: Find the size of 'text' rendered in 'font'.
  
  Index2dC RawCanvasBodyC::GUITextSize(GdkFont *font,const StringC &txt) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    IntT width = gdk_text_width(font,txt.chars(),txt.Size());
    return Index2dC(10,width); // Where's the 10 come from ?
  }
  
  //: Draw a circle.
  
  void RawCanvasBodyC::GUIDrawCircle(GdkGC *gc,Index2dC cent,UIntT size,bool fill) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_draw_arc(GUIDrawWindow(),gc,(int) fill ,
		 cent[1].V() - size,cent[0].V() - size,
		 size*2,size*2,
		 0,64* 360);
    
  }
  
  //: Translate an expose event.
  // 'rect' is the area to be updated.
  // 'toFollow' is a count of how many expose events queued to follow this one.
  
  bool RawCanvasBodyC::TranslateExposeEvent(const GdkEvent* event,IndexRange2dC &rect,IntT &toFollow) {
    RavlAssert(event->type == GDK_EXPOSE);
    GdkEventExpose &expose = *((GdkEventExpose *)event);    
    rect = IndexRange2dC(expose.area.y,expose.area.height + expose.area.y,
			 expose.area.x,expose.area.width + expose.area.x);
    toFollow = expose.count;
    return true;
  }
  
  //: Translate a configure event.
  // 'rect' is the new size for the widget.
  
  bool RawCanvasBodyC::TranslateConfigureEvent(const GdkEvent* event,IndexRange2dC &widgeSize) {
    RavlAssert(event->type == GDK_CONFIGURE);
    GdkEventConfigure *configEvent = (GdkEventConfigure *) event;
    widgeSize = IndexRange2dC(0,configEvent->height,
			      0,configEvent->width);
    return true;
  }


}
