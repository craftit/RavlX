// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_RAWCANVAS_HEADER 
#define RAVLGUI_RAWCANVAS_HEADER  1
//////////////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/RawCanvas.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/09/1999"
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/Pixbuf.hh"

namespace RavlImageN {
  template<class PixelT> class ImageC;
  class ByteRGBValueC;
  class ByteRGBAValueC;
  class ByteIAValueC;
}

namespace RavlGUIN {
  using namespace RavlImageN;
  class PixbufC;
  
  //! userlevel=Develop
  //: RawCanvas body.
  
  class RawCanvasBodyC
    : public WidgetBodyC 
  {
  public:
    RawCanvasBodyC(int width,int height)
      : sx(width),sy(height)
    {}
    //: Default constructor.
    
    GdkDrawable *DrawArea();
    //: Access draw area.
    
    GdkGC *GUIDrawGC();
    //: Get drawing context.
    
    GdkFont *GUIDrawFont();
    //: Font to use.
    
    GdkGC *GUIDrawGCBlack();
    //: Get black drawing context.
    
    GdkGC *GUIDrawGCWhite();
    //: Get white drawing context.
    
    GdkGC *GUIDrawGCGrey();
    //: Get grey drawing context.
    
    GdkWindow *GUIDrawWindow();
    //: Get get draw area.
    
    void GUIDrawArc(GdkGC *gc,const IndexRange2dC &rect, IntT start, IntT angle, bool fill = false); 
    //: Draw an arc.
    // The rectangle defines an ellipse in the image
    // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
    // angle is the length of the arc, in 1/64ths of a degree
    // If fill is true, the arc is a pie, otherwise it's a line arc
    
    void GUIDrawLine(GdkGC *gc,Index2dC p1,Index2dC p2);
    //: Draw a line.
    
    void GUIDrawRectangle(GdkGC *gc,const IndexRange2dC &rect,bool fill = false);
    //: Draw a rectangle.
    
    void GUIDrawRectangle(GdkGC *gc,Index2dC topLeft,Index2dC bottomRight,bool fill = false);
    //: Draw a rectangle.
    
    void GUIDrawText(GdkGC *gc,GdkFont *font,Index2dC at,const StringC &txt);
    //: Draw text.
    // 'at' is the bottom left of the text to be drawn.
    
    Index2dC GUITextSize(GdkFont *font,const StringC &txt);
    //: Find the size of 'text' rendered in 'font'.
    
    void GUIDrawCircle(GdkGC *gc,Index2dC cent,UIntT size,bool fill);
    //: Draw a circle.
    
    void GUIDrawImage(const PixbufC &pixbuf,const Index2dC &offset = Index2dC(0,0));
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    template <typename PixelT>
    void GUIDrawImage(const ImageC<PixelT> &image, const Index2dC &offset = Index2dC(0,0),bool ignoreImageOrigin = false)
    { Index2dC at = offset; if(!ignoreImageOrigin) at += image.Frame().Origin(); GUIDrawImage(PixbufC(image), at); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.

    bool TranslateExposeEvent(const GdkEvent* event,IndexRange2dC &rect,IntT &toFollow);
    //: Translate an expose event.
    // 'rect' is the area to be updated.
    // 'toFollow' is a count of how many expose events queued to follow this one.
    
    bool TranslateConfigureEvent(const GdkEvent* event,IndexRange2dC &widgeSize);
    //: Translate a configure event.
    // 'rect' is the new size for the widget.
    
    IntT SizeX() 
    { return sx; }
    //: Get the width.
    
    IntT SizeY() 
    { return sy; }
    //: Get the height.

  protected:  
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *nwidget);
    //: Create the widget.
    
    virtual void Destroy()
    { WidgetBodyC::Destroy(); }
    //: Undo all references.
    // Placeholder only.
    
  private:
    int sx,sy;
  };
  

  template <>
  void RawCanvasBodyC::GUIDrawImage(const ImageC<ByteRGBValueC> &image,const Index2dC &offset,bool ignoreImageOrigin);
  //: Draw an image into the canvas with its origin offset by 'offset'.
  // Note: You have to include the RavlGUI2d library to use this function.

  template <>
  void RawCanvasBodyC::GUIDrawImage(const ImageC<ByteT> &image,const Index2dC &offset,bool ignoreImageOrigin);
  //: Draw an image into the canvas with its origin offset by 'offset'.
  // Note: You have to include the RavlGUI2d library to use this function.

  //! userlevel=Normal
  //: RawCanvas handle.
  
  class RawCanvasC
    : public WidgetC 
  {
  public:
    RawCanvasC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
    
    RawCanvasC(int width,int height)
      : WidgetC(*new RawCanvasBodyC(width,height))
      {}
    //: Constructor.  

  protected:
    RawCanvasC(RawCanvasBodyC &body)
      : WidgetC(body)
    {}
    //: Body constructor.
    
    RawCanvasBodyC &Body()
    { return static_cast<RawCanvasBodyC &>(WidgetC::Body()); }
    //: Access body.

    const RawCanvasBodyC &Body() const
    { return static_cast<const RawCanvasBodyC &>(WidgetC::Body()); }
    //: Access body.
  public:
    GdkDrawable *DrawArea()
    { return Body().DrawArea(); }
    //: Access draw area.
    
    
    GdkGC *GUIDrawGC()
    { return Body().GUIDrawGC(); }
    //: Get drawing context.
    
    GdkFont *GUIDrawFont()
    { return Body().GUIDrawFont(); }
    //: Font to use.
    
    GdkGC *GUIDrawGCBlack()
    { return Body().GUIDrawGCBlack(); }
    //: Get black drawing context.
    
    GdkGC *GUIDrawGCWhite()
    { return Body().GUIDrawGCWhite(); }
    //: Get white drawing context.
    
    GdkGC *GUIDrawGCGrey()
    { return Body().GUIDrawGCGrey(); }
    //: Get grey drawing context.
    
    GdkWindow *GUIDrawWindow()
    { return Body().GUIDrawWindow(); }
    //: Get get draw area.
    
    void GUIDrawLine(GdkGC *gc,Index2dC p1,Index2dC p2)
    { Body().GUIDrawLine(gc,p1,p2); }
    //: Draw a line.
    
    void GUIDrawRectangle(GdkGC *gc,const IndexRange2dC &rect,bool fill = false)
    { Body().GUIDrawRectangle(gc,rect,fill); }
    //: Draw a rectangle.
    
    void GUIDrawRectangle(GdkGC *gc,Index2dC topLeft,Index2dC bottomRight,bool fill = false)
    { Body().GUIDrawRectangle(gc,topLeft,bottomRight,fill); }
    //: Draw a rectangle.
    
    void GUIDrawText(GdkGC *gc,GdkFont *font,Index2dC at,const StringC &txt)
    { Body().GUIDrawText(gc,font,at,txt); }
    //: Draw text.
    // 'at' is the bottom left of the text to be drawn.
    
    Index2dC GUITextSize(GdkFont *font,const StringC &txt)
    { return Body().GUITextSize(font,txt); }
    //: Find the size of 'text' rendered in 'font'.
    
    void GUIDrawCircle(GdkGC *gc,Index2dC cent,UIntT size,bool fill)
    { Body().GUIDrawCircle(gc,cent,size,fill); }
    //: Draw a circle.
    
    void GUIDrawArc(GdkGC *gc,const IndexRange2dC &rect, IntT start, IntT angle, bool fill = false)
    { return Body().GUIDrawArc(gc,rect,start,angle,fill); }
    //: Draw an arc.
    // The rectangle defines an ellipse in the image
    // start is the angle to start at, relative to 3 o'clock, in 1/64ths of a degree.
    // angle is the length of the arc, in 1/64ths of a degree
    // If fill is true, the arc is a pie, otherwise it's a line arc
    
    void GUIDrawImage(const PixbufC &pixbuf,const Index2dC &offset = Index2dC(0,0))
    { Body().GUIDrawImage(pixbuf,offset); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    template <typename PixelT>
    void GUIDrawImage(const ImageC<PixelT> &image,const Index2dC &offset = Index2dC(0,0),bool ignoreImageOrigin = false)
    { Body().GUIDrawImage(image,offset,ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    bool TranslateExposeEvent(const GdkEvent* event,IndexRange2dC &rect,IntT &toFollow)
    { return Body().TranslateExposeEvent(event,rect,toFollow); }
    //: Translate an expose event.
    // 'rect' is the area to be updated.
    // 'toFollow' is a count of how many expose events queued to follow this one.
    
    bool TranslateConfigureEvent(const GdkEvent* event,IndexRange2dC &widgeSize)
    { return Body().TranslateConfigureEvent(event,widgeSize); }
    //: Translate a configure event.
    // 'rect' is the new size for the widget.
    
    IntT SizeX() 
    { return Body().SizeX(); }
    //: Get the width.
    
    IntT SizeY() 
    { return Body().SizeY(); }
    //: Get the height.

  };


}

#endif
