// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, RavlPereception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RAWZOOMCANVAS_HEADER
#define RAVL_RAWZOOMCANVAS_HEADER 1
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Control"
//! example=exRawZoomCanvas.cc
//! file="Ravl/GUI/2D/RawZoomCanvas.hh"

#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/Manager.hh"

#include "Ravl/Vector2d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/RealRange2d.hh"

#include "Ravl/Image/WarpScale2d.hh"

namespace RavlGUIN {
  class RawZoomCanvasC;
  
  //! userlevel=Developer
  //: Raw Zoomable Canvas.
  
  class RawZoomCanvasBodyC
    : public RawCanvasBodyC 
  {
  public:
    RawZoomCanvasBodyC(IntT rows,IntT cols);
    //: Constructor.
    
    virtual ~RawZoomCanvasBodyC() { }
    //: Destructor

    const Vector2dC &Offset() const
    { return offset; }
    //: Access current offset.
    
    const Vector2dC &Scale() const
    { return scale; }
    //: Access current scale.
    
    void GUISetOffset(const Vector2dC &off);
    //: Set current offset in GUI thread.
    
    void GUISetScale(const Vector2dC &scale);
    //: Set current scale in GUI thread.
    
    void SetOffset(const Vector2dC &off);
    //: Set current offset.
    
    void SetScale(const Vector2dC &scale);
    //: Set current scale.
    
    Point2dC World2GUI(const Point2dC &at) const
    { return  at * scale + offset; }
    //: Convert world coordinates to GUI coordinates.

    Index2dC World2GUIi(const Point2dC &at) const
    { return  Index2dC(World2GUI(at)); }
    //: Convert world coordinates to GUI coordinates.
    
    RealRange2dC World2GUI(const IndexRange2dC &at) const
    { return  RealRange2dC(World2GUI(at.Origin()),World2GUI(at.End())); }
    //: Convert world coordinates to GUI coordinates.
    
    RealRange2dC World2GUI(const RealRange2dC &at) const
    { return  RealRange2dC(World2GUI(at.Origin()),World2GUI(at.End())); }
    //: Convert world coordinates to GUI coordinates.
    
    IndexRange2dC World2GUIi(const IndexRange2dC &at) const
    { return  IndexRange2dC(World2GUIi(at.Origin()),World2GUIi(at.End())); }
    //: Convert world coordinates to GUI coordinates.
    
    Point2dC GUI2World(const Point2dC &at) const
    { return  (at - offset) / scale; }
    //: Convert GUI coordinates to world coordinates
    
    RealRange2dC GUI2World(const IndexRange2dC &at) const
    { return  RealRange2dC(GUI2World(at.Origin()),GUI2World(at.End())); }
    //: Convert world coordinates to GUI coordinates.
    
    void GUIDrawLine(GdkGC *gc,Point2dC p1,Point2dC p2);
    //: Draw a line.
    
    void GUIDrawRectangle(GdkGC *gc,const RealRange2dC &rect,bool fill = false);
    //: Draw a rectangle.
    
    void GUIDrawRectangle(GdkGC *gc,Point2dC topLeft,Point2dC bottomRight,bool fill = false);
    //: Draw a rectangle.
    
    void GUIDrawText(GdkGC *gc,GdkFont *font,Point2dC at,const StringC &txt);
    //: Draw text.
    // 'at' is the bottom left of the text to be drawn.
    
    void GUITextSize(GdkFont *font,const StringC &txt,Point2dC &size);
    //: Find the size of 'text' rendered in 'font'.
    
    void GUIDrawCircle(GdkGC *gc,Point2dC cent,RealT size,bool fill);
    //: Draw a circle.
    
    template<typename PixelT>
    void GUIDrawImage(const ImageC<PixelT> &image,const Point2dC &offset,bool ignoreImageOrigin = false);
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.

    bool TranslateExposeEvent(const GdkEvent* event,RealRange2dC &rect,IntT &toFollow);
    //: Translate an expose event.
    // 'rect' is the area to be updated.
    // 'toFollow' is a count of how many expose events queued to follow this one.
    
    bool TranslateConfigureEvent(const GdkEvent* event,RealRange2dC &widgeSize);
    //: Translate a configure event.
    // 'rect' is the new size for the widget.
    
    // Override Index based versions.
    
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
    
    template<typename PixelT>
    void GUIDrawImage(const ImageC<PixelT> &image,const Index2dC &offset = Index2dC(0,0),bool ignoreImageOrigin = false)
    { GUIDrawImage(image,Point2dC(offset),ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.

    bool TranslateExposeEvent(const GdkEvent* event,IndexRange2dC &rect,IntT &toFollow);
    //: Translate an expose event.
    // 'rect' is the area to be updated.
    // 'toFollow' is a count of how many expose events queued to follow this one.
    
    bool TranslateConfigureEvent(const GdkEvent* event,IndexRange2dC &widgeSize);
    //: Translate a configure event.
    // 'rect' is the new size for the widget.
    
    RealRange2dC DisplayArea() const
    { return GUI2World(widgetSize); }
    //: Get the size of the display area
    
  protected:  
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *widget);
    //: Create the widget.
    
    virtual bool EventConfigure(GdkEvent* &event);
    //: Handle configure event.
    
    Vector2dC offset;
    Vector2dC scale;
    
    IndexRange2dC widgetSize;
  };
  
  template<typename PixelT>
  void RawZoomCanvasBodyC::GUIDrawImage(const ImageC<PixelT> &image,const Point2dC &doffset,bool ignoreImageOrigin) {
    //cerr << "RawZoomCanvasBodyC::GUIDrawImage(), Called. Offset=" << offset << "\n";
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    IndexRange2dC imgRect = World2GUIi(IndexRange2dC(image.Frame().RowRange().Min(),
                                                     image.Frame().RowRange().Max() + 1,
                                                     image.Frame().ColRange().Min(),
                                                     image.Frame().ColRange().Max() + 1) + doffset);

    IndexRange2dC drawRect = IndexRange2dC(imgRect.RowRange().Min(),
                                           imgRect.RowRange().Max() - 1,
                                           imgRect.ColRange().Min(),
                                           imgRect.ColRange().Max() - 1);

    //ONDEBUG(std::cerr << "DrawRect=" << drawRect << " widgetSize=" << widgetSize << "\n");
    drawRect.ClipBy(widgetSize);
    if(drawRect.Area() <= 0)
      return ;
    //ONDEBUG(std::cerr << "FinalDrawRect=" << drawRect << "\n");

    if(Abs(scale[0] - 1) < 0.0001 && Abs(scale[1] - 1) < 0.0001) {
      RawCanvasBodyC::GUIDrawImage(image,Index2dC(doffset + offset),false);
    } else {
      ImageC<PixelT> drawImg;
      Vector2dC inc(1/scale[0],1/scale[1]);
      if(inc[0] > 1.01 && inc[1] > 1.01) {
        WarpSubsample(image, inc, drawImg);
        //offset image
        Index2dC fullOffset(Round(offset.Row()+doffset.Row()), Round(offset.Col()+doffset.Col()));
        drawImg.ShiftArray(fullOffset);
      } else {
        drawImg = ImageC<PixelT>(drawRect);
        Array2dIterC<PixelT> it(drawImg);
        Point2dC pat,start = GUI2World(it.Index()) + doffset;
        pat = start;
        for(;it;) {
          pat[1] = start[1];
          do {
            Index2dC at(Floor(pat[0] <= image.TRow().V() ? image.TRow().V() : pat[0]),
                        Floor(pat[1] <= image.LCol().V() ? image.LCol().V() : pat[1]));

            if(image.Frame().Contains(at))
              *it = image[at];
            else
              SetZero(*it);
            pat[1] += inc[1];
          } while(it.Next());
          pat[0] += inc[0];
        }
      }
      RawCanvasBodyC::GUIDrawImage(drawImg,Index2dC(0,0),false);
    }
  }
  
  template<>
  void RawZoomCanvasBodyC::GUIDrawImage(const ImageC<ByteIAValueC> &image,const Point2dC &doffset,bool ignoreImageOrigin);

    //! userlevel=Normal
  //: Raw Zoomable Canvas.
  
  class RawZoomCanvasC
    : public RawCanvasC 
  {
  public:
    RawZoomCanvasC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    RawZoomCanvasC(IntT rows,IntT cols)
      : RawCanvasC(*new RawZoomCanvasBodyC(rows,cols))
    {}
    //: Default constructor.
    
  protected:
    RawZoomCanvasC(RawZoomCanvasBodyC &bod)
      : RawCanvasC(bod)
    {}
    //: Body constructor.
    
    RawZoomCanvasBodyC &Body()
    { return static_cast<RawZoomCanvasBodyC &>(WidgetC::Body()); }
    //: Access body.

    const RawZoomCanvasBodyC &Body() const
    { return static_cast<const RawZoomCanvasBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    const Vector2dC &Offset() const
    { return Body().Offset(); }
    //: Access current offset.
    
    const Vector2dC &Scale() const
    { return Body().Scale(); }
    //: Access current scale.
    
    void GUISetOffset(const Vector2dC &off)
    { Body().GUISetOffset(off); }
    //: Access current offset.
    
    void GUISetScale(const Vector2dC &scale)
    { Body().GUISetScale(scale); }
    //: Access current scale.
    
    void SetOffset(const Vector2dC &off)
    { Body().SetOffset(off); }
    //: Access current offset.
    
    void SetScale(const Vector2dC &scale)
    { Body().SetScale(scale); }
    //: Access current scale.
    
    void GUIDrawLine(GdkGC *gc,Point2dC p1,Point2dC p2)
    { Body().GUIDrawLine(gc,p1,p2); }
    //: Draw a line.
    
    void GUIDrawRectangle(GdkGC *gc,const RealRange2dC &rect,bool fill = false)
    { Body().GUIDrawRectangle(gc,rect,fill); }
    //: Draw a rectangle.
    
    void GUIDrawRectangle(GdkGC *gc,Point2dC topLeft,Point2dC bottomRight,bool fill = false)
    { Body().GUIDrawRectangle(gc,topLeft,bottomRight,fill); }
    //: Draw a rectangle.
    
    void GUIDrawText(GdkGC *gc,GdkFont *font,Point2dC at,const StringC &txt)
    { Body().GUIDrawText(gc,font,at,txt); }
    //: Draw text.
    // 'at' is the bottom left of the text to be drawn.
    
    void GUITextSize(GdkFont *font,const StringC &txt,Point2dC &size)
    { Body().GUITextSize(font,txt,size); }
    //: Find the size of 'text' rendered in 'font'.
    
    void GUIDrawCircle(GdkGC *gc,Point2dC cent,RealT size,bool fill = false)
    { Body().GUIDrawCircle(gc,cent,size,fill); }
    //: Draw a circle.
    
    template<typename PixelT>
    void GUIDrawImage(const ImageC<PixelT> &image,const Point2dC &offset = Point2dC(0,0),bool ignoreImageOrigin = false)
    { Body().GUIDrawImage(image,offset,ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    bool TranslateExposeEvent(const GdkEvent* event,RealRange2dC &rect,IntT &toFollow)
    { return Body().TranslateExposeEvent(event,rect,toFollow); }
    //: Translate an expose event.
    // 'rect' is the area to be updated.
    // 'toFollow' is a count of how many expose events queued to follow this one.
    
    bool TranslateConfigureEvent(const GdkEvent* event,RealRange2dC &widgeSize)
    { return Body().TranslateConfigureEvent(event,widgeSize); }
    //: Translate a configure event.
    // 'rect' is the new size for the widget.

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
    
    /*void GUIDrawImage(const ImageC<ByteRGBValueC> &image,const Index2dC &offset,bool ignoreImageOrigin = false)
    { Body().GUIDrawImage(image,offset,ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    void GUIDrawImage(const ImageC<ByteRGBAValueC> &image,const Index2dC &offset,bool ignoreImageOrigin = false)
    { Body().GUIDrawImage(image,offset,ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    void GUIDrawImage(const ImageC<ByteT> &image,const Index2dC &offset,bool ignoreImageOrigin = false)
    { Body().GUIDrawImage(image,offset,ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.
    
    void GUIDrawImage(const ImageC<ByteIAValueC> &image,const Point2dC &offset,bool ignoreImageOrigin = false)
    { Body().GUIDrawImage(image,offset,ignoreImageOrigin); }
    //: Draw an image into the canvas with its origin offset by 'offset'.
    // Note: You have to include the RavlGUI2d library to use this function.*/

    bool TranslateExposeEvent(const GdkEvent* event,IndexRange2dC &rect,IntT &toFollow)
    { return Body().TranslateExposeEvent(event,rect,toFollow); }
    //: Translate an expose event.
    // 'rect' is the area to be updated.
    // 'toFollow' is a count of how many expose events queued to follow this one.
    
    bool TranslateConfigureEvent(const GdkEvent* event,IndexRange2dC &widgeSize)
    { return Body().TranslateConfigureEvent(event,widgeSize); }
    //: Translate a configure event.
    // 'rect' is the new size for the widget.
    
    Point2dC World2GUI(const Point2dC &at) const
    { return Body().World2GUI(at); }
    //: Convert world coordinates to GUI coordinates.

    Index2dC World2GUIi(const Point2dC &at) const
    { return Body().World2GUIi(at); }
    //: Convert world coordinates to GUI coordinates.
    
    RealRange2dC World2GUI(const IndexRange2dC &at) const
    { return Body().World2GUI(at); }
    //: Convert world coordinates to GUI coordinates.
    
    RealRange2dC World2GUI(const RealRange2dC &at) const
    { return  Body().World2GUI(at); }
    //: Convert world coordinates to GUI coordinates.
    
    IndexRange2dC World2GUIi(const IndexRange2dC &at) const
    { return Body().World2GUIi(at); }
    //: Convert world coordinates to GUI coordinates.
    
    Point2dC GUI2World(const Point2dC &at) const
    { return Body().GUI2World(at); }
    //: Convert GUI coordinates to world coordinates
    
    RealRange2dC GUI2World(const IndexRange2dC &at) const
    { return Body().GUI2World(at); }
    //: Convert world coordinates to GUI coordinates.
    
    RealRange2dC DisplayArea() const
    { return Body().DisplayArea(); }
    //: Get the size of the display area
    
    friend class RawZoomCanvasBodyC;
  };

}



#endif

