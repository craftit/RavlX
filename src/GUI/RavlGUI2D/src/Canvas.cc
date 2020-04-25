// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI2D
//! file="Ravl/GUI/2D/Canvas.cc"

#include "Ravl/Math.hh"
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/CallMethodRefs.hh"
#include "Ravl/Stream.hh"

#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define WARNING_QUEUE_SIZE 30

namespace RavlGUIN {

  static int gtkCanvasDestroyGC (GtkWidget *widget,GdkGC * data) { 
    ONDEBUG(std::cerr << "Got destroy for GC : " << ((void *) data) << "\n");
    gdk_gc_unref(data);
    return 1;
  }
  
  void CanvasBodyC::DoSomeSetup() {
    drawGC = gdk_gc_new(Widget()->window);
    gdk_gc_copy(drawGC,widget->style->white_gc);
    //gdk_gc_ref(drawGC);
    gtk_signal_connect (GTK_OBJECT (Widget()), "destroy",
                        (GtkSignalFunc) gtkCanvasDestroyGC,drawGC);
    
    //widget->style->fg_gc[0] = drawGC; // In the hope it'll take care of deleting it...
  }
  
  /* Create a new backing pixmap of the appropriate size */
  static gint win_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data ) {
    CanvasBodyC &body = *(CanvasBodyC *) data;
    ONDEBUG(std::cerr <<"Configuring pixmap. \n");
    if(body.ConfigDone())
      return true;
    GdkPixmap *orgPixmap = body.Pixmap();
    gint xs,ys;
    if(orgPixmap != 0) {
      // Do we really need to resize, or is it smaller ?
      gdk_window_get_size(orgPixmap,&xs,&ys);
      if(widget->allocation.width < xs ||
         widget->allocation.height < ys) 
        return true; // Its smaller, don't bother.
    }
    body.Pixmap() = gdk_pixmap_new(widget->window,
                                   widget->allocation.width,
                                   widget->allocation.height,
                                   -1);
#if 0
    GdkVisual *vis = gdk_window_get_visual (widget->window);
    std::cerr << "Vis:" << ((void *) vis) << std::endl;
    std::cerr << "Type: " << ((int) vis->type) << " Depth:" << ((int) vis->depth) << std::endl;
    std::cerr << "ColMapSize: " << vis->colormap_size << std::endl;
    std::cerr << "BitPerRGB:" << vis->bits_per_rgb << std::endl;
#endif
    if(body.DrawGC() == 0)
      body.DoSomeSetup();
    // Clear the new pixmap.
    gdk_draw_rectangle (body.Pixmap(),
                        widget->style->black_gc,
                        true,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);
    if (orgPixmap != 0 ) { // Copy contents of old one ?
      gdk_draw_pixmap(body.Pixmap(),
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      orgPixmap,
                      0,0,
                      0,0,
                      xs, ys);
      // Delete old pixmap.
      gdk_pixmap_unref(orgPixmap);
    } 
    
    ONDEBUG(std::cerr <<"Configuring pixmap done. \n");
    return true;
  }
  
  /* Redraw the screen from the backing pixmap */
  static gint
  win_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data) {
    CanvasBodyC &body = *(CanvasBodyC *) data;
    ONDEBUG(std::cerr <<"Expose event. \n");
    if(body.Pixmap() != 0) {
      gdk_draw_pixmap(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      body.Pixmap(),
                      event->area.x, event->area.y,
                      event->area.x, event->area.y,
                      event->area.width, event->area.height);
    } else
    std::cerr << "WARNING: Canvas empty. \n";
    ONDEBUG(std::cerr <<"Expose event done. \n");
  /* Take care of pending actions that require the pixmap. */
    if (!body.InitialExposureDone()) {
      body.SetInitialExposureDone(true);
      while(!body.ToDo().IsEmpty()) {
        ONDEBUG(std::cerr << "Catching up with stuff. \n");
        body.ToDo().Last().Invoke();
        body.ToDo().DelLast();
      }
    }
    
    return false;
  }
  
  //: Constructor.
  CanvasBodyC::CanvasBodyC(int nsx,int nsy,bool ndirect)
    : sx(nsx),sy(nsy),
      direct(ndirect),
      initialExposureDone(false),
      pixmap(0),
      configDone(false),
      drawGC(0),
      autoRefresh(true)
  {}

  //: Constructor.
  CanvasBodyC::CanvasBodyC(const ImageRectangleC& rect,bool ndirect)
    : sx(rect.Cols()),sy(rect.Rows()),
      direct(ndirect),
      initialExposureDone(false),
      pixmap(0),
      configDone(false),
      drawGC(0),
      autoRefresh(true)
  {}
  
  //: Destructor.
  
  CanvasBodyC::~CanvasBodyC()
  {}
  
  //: Create the widget.
  bool CanvasBodyC::Create() {
    if(widget != 0) return true; // Done already.
    return Create(gtk_drawing_area_new ());
  }
  
  //: Create with a widget supplied from elsewhere.
  
  bool CanvasBodyC::Create(GtkWidget *nwidget) {
    widget = nwidget;
    ONDEBUG(std::cerr <<"CanvasBodyC::Create() start. \n");
    gtk_drawing_area_size (GTK_DRAWING_AREA (widget), sx, sy);  
    if(!direct) {
      gtk_signal_connect (GTK_OBJECT (widget), "expose_event",
                          (GtkSignalFunc) win_expose_event,(gpointer) this);
      
      gtk_signal_connect (GTK_OBJECT(widget),"configure_event",
                          (GtkSignalFunc) win_configure_event,(gpointer) this);
    }
    SetupColours();
    ConnectSignals();
    gtk_widget_add_events(widget,GDK_EXPOSURE_MASK);
    ONDEBUG(std::cerr <<"CanvasBodyC::Create() done. \n");
    return true;    
  }
  
  void CanvasBodyC::WidgetDestroy() {
    if(pixmap != 0) {
      gdk_pixmap_unref(pixmap);
      pixmap = 0;
    }
    WidgetBodyC::WidgetDestroy();
  }
  
  //: Get draw area.
  
  GdkDrawable *CanvasBodyC::DrawArea() {
    if(direct)
      return widget->window;
    return pixmap; 
  }
  

  //: Draw an image on the canvas.
  
  void CanvasBodyC::DrawImage(ImageC<ByteT> &img,Index2dC offset) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawImage,const_cast<ImageC<ByteT> &>(img),offset));
  }
  
  //: Draw an rgb image on the canvas.
  
  void CanvasBodyC::DrawImage(const ImageC<ByteRGBValueC> &img,Index2dC offset) {
    ONDEBUG(std::cerr <<"CanvasBodyC::DrawImage()" << std::endl;)
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawRGBImage,const_cast<ImageC<ByteRGBValueC> &>(img),offset));
  }

  //: Draw an rgb image on the canvas.
  
  void CanvasBodyC::DrawLine(IntT x1,IntT y1,IntT x2,IntT y2,IntT c) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawLine,x1,y1,x2,y2,c));
  }

  void CanvasBodyC::DrawArc(ImageRectangleC rect, IntT start, IntT angle, IntT colId, bool fill) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawArc,rect,start,angle,colId,fill));
  }

  //: Draw some text
  
  void CanvasBodyC::DrawText(IntT x1,IntT y1,StringC text,IntT colId) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawText,x1,y1,text,colId));
  }
  
  //: Draw a rectangle.
  
  void CanvasBodyC::DrawRectangle(IntT x1,IntT y1,IntT x2,IntT y2,IntT c) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawRectangle,x1,y1,x2,y2,c));
  }

  void CanvasBodyC::DrawFrame(IntT x1,IntT y1,IntT x2,IntT y2,IntT c) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIDrawFrame,x1,y1,x2,y2,c));
  }

  void CanvasBodyC::SetLineStyle(IntT iWidth, GdkLineStyle linestyle, GdkCapStyle capstyle, GdkJoinStyle joinstyle) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUISetLineStyle,iWidth,linestyle,capstyle,joinstyle));
  }
  
  bool CanvasBodyC::GUISetLineStyle(IntT& iWidth, GdkLineStyle& linestyle, GdkCapStyle& capstyle, GdkJoinStyle& joinstyle) {
    if (DrawGC() != 0) {
      gdk_gc_set_line_attributes(DrawGC(),
                                 iWidth,
                                 linestyle,
                                 capstyle,
                                 joinstyle);
    }
    return true;
  }
  
  //: Turn auto refresh after draw routines on/off.
  
  bool CanvasBodyC::GUIAutoRefresh(bool &val) { 
    ONDEBUG(std::cerr << "CanvasBodyC::GUIAutoRefresh(bool). Val=" << val << "\n");
    if(!autoRefresh && val)
      GUIRefresh();
    autoRefresh = val;
    return true;
  }
  
  //: Turn auto refresh after draw routines on/off.
  
  void CanvasBodyC::AutoRefresh(bool val) {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIAutoRefresh,val));
  }
  
  //: Draw an image on the canvas.
  // Call with GUI thread only!
  
  bool CanvasBodyC::GUIDrawImage(ImageC<ByteT> &img,const Index2dC &ioffset) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawImage(), WARNING: Asked to render data before canvas is initialise. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawImage,img,ioffset);
      AddToQueue(trigger);
      return true;
    }
    if(img.IsEmpty()) {
      std::cerr << "CanvasBodyC::GUIDrawImage(), WARNING: Ask to render empty image. \n";
      return true;
    }
#if 0
    if(!img.IsContinuous()) {
      std::cerr << "CanvasBodyC::GUIDrawImage(), WARNING: Image not continuous in memory, making copy. \n";
      ImageC<ByteT> tmp(img.Copy()); // Make image continuous.
      return GUIDrawImage(tmp,ioffset);
    }
#endif
    
    ONDEBUG(std::cerr << "CanvasBodyC::GUIDrawImage(), Rendering image. \n");
    Index2dC off = ioffset + img.Rectangle().Origin();    
    int atx = off.Col().V(); // Convert between RAVL and GTK co-ordinates...
    int aty = off.Row().V();
    
    // Check that that rows are sequential in memory.
    // Check that rows at least look sequential in memory.
    
    if(!img.IsBlock()) 
      img = img.Copy(); // The don't seem to be, so make a copy.
    
    gdk_draw_gray_image(DrawArea(),
                        widget->style->black_gc,
                        atx,aty,
                        img.Cols(),img.Rows(),
                        GDK_RGB_DITHER_NORMAL,
                        img.Row(img.TRow()),
                        img.Stride());
    
#if 1    
    if(autoRefresh) {
      GdkRectangle update_rect;
      update_rect.x = atx;
      update_rect.y = aty;
      update_rect.width = Min((SizeT)Widget()->allocation.width,img.Cols());
      update_rect.height = Min((SizeT)Widget()->allocation.height,img.Rows());
      gtk_widget_draw (Widget(), &update_rect);  
    }
#endif
    return true;
  }
  
  //: Draw an rgb image on the canvas.
  // Call with GUI thread only!
  
  bool CanvasBodyC::GUIDrawRGBImage(ImageC<ByteRGBValueC> &img,const Index2dC &ioffset) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawRGBImage(), WARNING: Asked to render data before canvas is initialise. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawRGBImage,img,ioffset);
      AddToQueue(trigger);
      return true;
    }
    if(img.IsEmpty()) {
      std::cerr << "CanvasBodyC::GUIDrawRGBImage(), WARNING: Ask to render empty image. \n";
      return true;
    }
    ONDEBUG(std::cerr << "CanvasBodyC::GUIDrawRGBImage(), Render image. \n");
    Index2dC off = ioffset + img.Rectangle().Origin();    
    int atx = off.Col().V(); // Convert between RAVL and GTK co-ordinates...
    int aty = off.Row().V(); 
    
    GtkWidget *widget = Widget();
    
    // Check that rows at least look sequential in memory.
    if(!img.IsBlock())
      img = img.Copy(); // The don't seem to be, so make a copy.
    
    gdk_draw_rgb_image(DrawArea(),
                       widget->style->black_gc,
                       atx,aty,
                       img.Cols(),img.Rows(),
                       GDK_RGB_DITHER_NORMAL,
                       (unsigned char *) img.Row(img.TRow()),
                       img.Stride() * sizeof(ByteRGBValueC));

#if 1
    if(autoRefresh) {
      GdkRectangle update_rect;
      update_rect.x = atx;
      update_rect.y = aty;
      update_rect.width = Min((SizeT)Widget()->allocation.width,img.Cols());
      update_rect.height = Min((SizeT)Widget()->allocation.height,img.Rows());
      gtk_widget_draw (Widget(), &update_rect);  
    }
#endif
    return true;
  }
  
  
  //: Draw a line.
  
  bool CanvasBodyC::GUIDrawLine(const IntT &x1,const IntT &y1,const IntT &x2,const IntT &y2,const IntT &c) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawLine(), WARNING: Asked to render data before canvas is initialise. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawLine,x1,y1,x2,y2,c);
      AddToQueue(trigger);
      return true;
    }
    GdkGC *gc;
    if(c == 0)
      gc = widget->style->white_gc;
    else{
      gc = DrawGC();
      gdk_gc_set_foreground(gc,&GetColour(c));
    }
    gdk_draw_line (DrawArea(),
                   gc,
                   x1, y1,
                   x2, y2);

    ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawLine(), AutoRefresh=" << autoRefresh << "\n");
    if(autoRefresh)
      GUIRefresh();
    return true;
  } 

  bool CanvasBodyC::GUIDrawLine(Index2dC start, Index2dC end, IntT c) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    IntT x1 = start.Col().V();
    IntT y1 = start.Row().V();
    IntT x2 = end.Col().V();
    IntT y2 = end.Row().V();
    return GUIDrawLine(x1, y1, x2, y2, c);
  } 


  //: Draw an arc
  
  bool CanvasBodyC::GUIDrawArc(const ImageRectangleC& rect, const IntT& start, const IntT& angle, const IntT& colId, const bool& fill) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawArc(), WARNING: Asked to render data before canvas is initialised. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawArc,rect,start,angle,colId,fill);
      AddToQueue(trigger);
      return true;
    }
    GdkGC *gc;
    if(colId == 0)
      gc = widget->style->white_gc;
    else{
      gc = DrawGC();
      gdk_gc_set_foreground(gc,&GetColour(colId));
    }
    gdk_draw_arc (DrawArea(),
                  gc,
                  fill,
                  rect.Origin().Col().V(),
                  rect.Origin().Row().V(),
                  rect.Size().Col().V(),
                  rect.Size().Row().V(),
                  start,
                  angle);

    ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawArc(), AutoRefresh=" << autoRefresh << "\n");
    if(autoRefresh)
      GUIRefresh();
    return true;    
  }

  //: Draw some text.
  
  bool CanvasBodyC::GUIDrawText(const IntT &x1,const IntT &y1,const StringC &text,const IntT &c) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawText(), WARNING: Asked to render data before canvas is initialise. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawText,x1,y1,text,c);
      AddToQueue(trigger);
      return true;
    }
    GdkGC *gc;
    if(c == 0)
      gc = widget->style->white_gc;
    else{
      gc = DrawGC();
      gdk_gc_set_foreground(gc,&GetColour(c));
    }
    GdkFont *cfont = gtk_style_get_font(widget->style);
    gdk_draw_text(DrawArea(),
                  cfont,
                  gc,
                  x1,
                  y1,
                  text.chars(),
                  text.length());
    
    if(autoRefresh)
      GUIRefresh();
    return true;
  }

  //: Draw a rectangle.
  
  bool CanvasBodyC::GUIDrawRectangle(const IntT &x1,const IntT &y1,const IntT &x2,const IntT &y2,const IntT &c) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawRectangle(), WARNING: Asked to render data before canvas is initialise. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawRectangle,x1,y1,x2,y2,c);
      AddToQueue(trigger);
      return true;
    }
    GdkGC *gc;
    if(c == 0)
      gc = widget->style->white_gc;
    else{
      gc = DrawGC();
      gdk_gc_set_foreground(gc,&GetColour(c));
    }
    
    gdk_draw_rectangle (DrawArea(),
                        gc,
                        true,
                        x1, y1,
                        x2, y2);
    if(autoRefresh)
      GUIRefresh();
    return true;
    
  }

  bool CanvasBodyC::GUIDrawFrame(const IntT &x1,const IntT &y1,const IntT &x2,const IntT &y2,const IntT &c) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(!IsReady()) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIDrawFrame(), WARNING: Asked to render data before canvas is initialise. \n");
      TriggerC trigger = TriggerR(*this,&CanvasBodyC::GUIDrawFrame,x1,y1,x2,y2,c);
      AddToQueue(trigger);
      return true;
    }
    GdkGC *gc;
    if(c == 0)
      gc = widget->style->white_gc;
    else{
      gc = DrawGC();
      gdk_gc_set_foreground(gc,&GetColour(c));
    }
    
    gdk_draw_rectangle (DrawArea(),
                        gc,
                        false,
                        x1, y1,
                        x2, y2);
    if(autoRefresh)
      GUIRefresh();
    return true;
    
  }

  bool CanvasBodyC::GUIDrawFrame(Index2dC topLeft, Index2dC bottomRight, IntT c) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    IntT x1 = topLeft.Col().V();
    IntT y1 = topLeft.Row().V();
    IntT width = (bottomRight.Col() - topLeft.Col()).V();
    IntT height = (bottomRight.Row() - topLeft.Row()).V();
    return GUIDrawFrame(x1, y1, width, height, c);                                                     
  }

  
  //: Refresh display.
  
  bool CanvasBodyC::GUIRefresh() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(widget == 0) {
      ONDEBUG(std::cerr <<"CanvasBodyC::GUIRefresh(), WARNING: Asked to refresh before canvas is initialise. \n");
      toDo.InsFirst(TriggerR(*this,&CanvasBodyC::GUIRefresh));
      return true;
    }
    GdkRectangle update_rect;
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = Widget()->allocation.width;
    update_rect.height = Widget()->allocation.height;
    gtk_widget_draw (Widget(), &update_rect);    
    return true;
  }

  //: Refresh display.
  
  bool CanvasBodyC::Refresh() {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIRefresh));
    return true;
  }
  
  //: Resize canvas.
  // GUI Thread only

  bool CanvasBodyC::GUIResize(int &nsx,int &nsy) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(nsx == sx && nsy == sy)
      return true; // No change!
    sx = nsx;
    sy = nsy;
    if(widget == 0)
      return true;
    //: Make new pixmap....
    if(pixmap != 0)
      gdk_pixmap_unref(pixmap);
    pixmap = gdk_pixmap_new(widget->window,
                            sx,sy,
                            -1);
    //: Make it blank.
    gdk_draw_rectangle (pixmap,
                        widget->style->black_gc,
                        true,
                        0, 0,
                        sx,sy);
    //: Set size....
    configDone = true; 
    gtk_drawing_area_size (GTK_DRAWING_AREA (widget), sx, sy);  
    return true;
  }
  
  //: Resize canvas.
  
  bool CanvasBodyC::Resize(int nsx,int nsy) {
    RavlAssert(sx >= 0 && sy >= 0);
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIResize,nsx,nsy));
    return true;
  }
  
  //: Access colour.
  // GUI thread only.
  
  GdkColor &CanvasBodyC::GetColour(int n) {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    //: special treatment for black - not in the default colormap
    if(n==65) {
      GdkColormap *colorMap = gdk_window_get_colormap(widget->window);
      if(!gdk_color_black(colorMap,&m_gdkBlack)) {
        std::cerr << "Failed to set color to black" << std::endl;
      }
      return m_gdkBlack;
    }

    //: for all other colors in the table
    static GdkColor nullColour;
    GdkColor &ret = colourTab[((UIntT)(n-1)) % colourTab.Size()];
    if(ret.pixel == 0) { // Need to allocate ?
      if(widget == 0) {
        std::cerr <<"CanvasBodyC::GetColour(), WARNING: Called before canvas is initalised. \n";
        return nullColour;
      }
      GdkColormap *colorMap = gdk_window_get_colormap(widget->window);
      if(!gdk_colormap_alloc_color (colorMap,
                                    &ret,
                                    false,
                                    true)) {
        std::cerr << "ViewGeomCanvasBodyC::AllocColours(), ERROR: Failed to allocate colour. \n";
      }
      if(ret.pixel == 0)
        std::cerr << "Ooops WARNING: Pixel value of 0, and we hoped this wouldn't happen! \n";
    }
    return ret;
  }
  
  //: Setup colour table.
  
  void CanvasBodyC::SetupColours() {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    if(colourTab.IsValid())
      return ; // Done already.
    
    //GdkColormap *colorMap = gdk_window_get_colormap(widget->window);
  
    colourTab = SArray1dC<GdkColor>(64);
    static const int clookup[4] = { 0,65535,32767,49151 };
    for(int i = 0;i < (int) colourTab.Size();i++) {
      GdkColor &col = colourTab[i];
      int num = i + 1;
      col.red   = clookup[(num  & 1)       + ((num &  8) >> 2)];
      col.green = clookup[((num & 2) >> 1) + ((num & 16) >> 3)];
      col.blue  = clookup[((num & 4) >> 2) + ((num & 32) >> 4)];
      col.pixel = 0;
    }
  }

  //: Clear canvas to given colour.
  
  bool CanvasBodyC::GUIClear() {
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gdk_draw_rectangle (pixmap,
                        widget->style->black_gc,
                        true,
                        0, 0,
                        sx,sy);
    
    if(autoRefresh) {
      GdkRectangle update_rect;
      update_rect.x = 0;
      update_rect.y = 0;
      update_rect.width = Widget()->allocation.width;
      update_rect.height = Widget()->allocation.height;
      gtk_widget_draw (Widget(), &update_rect);  
    }
    return true;
  }
  
  //: Clear canvas to given colour.
  
  bool CanvasBodyC::Clear() {
    Manager.Queue(Trigger(CanvasC(*this),&CanvasC::GUIClear));
    return true;
  }


  void CanvasBodyC::AddToQueue(TriggerC &trigger) {
    toDo.InsFirst(trigger);
    if (toDo.Size() >= WARNING_QUEUE_SIZE) {
      std::cerr << "WARNING!!! CanvasBodyC::AddToQueue() - draw to-do list size greater than " << WARNING_QUEUE_SIZE << "!"
           << endl << "Possible memory problems ahead..." << std::endl;
    }
  }

  /////////////////////////////////////////////////////////
  
  //: Constructor.
  
  CanvasC::CanvasC(int xs,int ys,bool direct)
    : WidgetC(*new CanvasBodyC(xs,ys,direct))
  {}

  CanvasC::CanvasC(const ImageRectangleC& rect,bool direct)
    : WidgetC(*new CanvasBodyC(rect,direct))
  {}

}  

