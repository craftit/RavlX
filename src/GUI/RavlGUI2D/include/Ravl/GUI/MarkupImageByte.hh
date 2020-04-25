// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MARKUPIMAGEBYTE_HEADER
#define RAVLGUI_MARKUPIMAGEBYTE_HEADER 1
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/GUI/MarkupInfo.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Vector2d.hh"

namespace RavlGUIN {
  using namespace RavlImageN;
  
  //! userlevel = Develop
  //: Result image
  
  class MarkupImageByteBodyC 
    : public MarkupInfoBodyC
  {
  public:
    MarkupImageByteBodyC(Int64T id,IntT zOrder,const ImageC<ByteT> &img,const Vector2dC &offset);
    //: Constructor.
    
    virtual bool CanClip() const;
    //: Can render method clip the region it updates ?
    
    virtual bool Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected);
    //: Method for rendering frame.
    
    virtual bool MouseEvent(GUIMarkupCanvasBodyC &mv,const Point2dC &at,MouseEventTypeT eventType,const MouseEventC &me,IntT &state,bool &refresh);
    //: Handle mouse event.
    // Returns true if even has been handled.
    
    const ImageC<ByteT> &Image() const
    { return image; }
    //: Access image.
    
  protected:
    ImageC<ByteT> image;
    Vector2dC offset;
  };
  
  //! userlevel=Normal
  //: Handle for MarkupImageByteBodyC
  //!cwiz:author
  
  class MarkupImageByteC
    : public MarkupInfoC
  {
  public:
    MarkupImageByteC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    MarkupImageByteC(Int64T id,IntT zOrder,const ImageC<ByteT> & img,const Vector2dC & offset = Vector2dC(0,0)) 
      : MarkupInfoC(*new MarkupImageByteBodyC(id,zOrder,img,offset))
    {}
    //: Constructor. 
    //!cwiz:author
    
    MarkupImageByteC(const MarkupInfoC &info)
      : MarkupInfoC(dynamic_cast<const MarkupImageByteBodyC *>(BodyPtr(info)))
    {}
    //: Upcast constructor
    
    bool Render(GUIMarkupCanvasBodyC & mv,const RealRange2dC &area,bool selected) 
    { return Body().Render(mv,area,selected); }
    //: Method for rendering frame. 
    //!cwiz:author
    
    bool MouseEvent(GUIMarkupCanvasBodyC & mv,const Point2dC & at,MouseEventTypeT eventType,const MouseEventC &me,IntT &state,bool &refresh) 
    { return Body().MouseEvent(mv,at,eventType,me,state,refresh); }
    //: Handle mouse event. 
    // Returns true if even has been handled.
    //!cwiz:author
    
    const ImageC<ByteT> &Image() const
    { return Body().Image(); }
    //: Access image.
    
  protected:
    MarkupImageByteC(MarkupImageByteBodyC &bod)
     : MarkupInfoC(bod)
    {}
    //: Body constructor. 
    
    MarkupImageByteBodyC& Body()
    { return static_cast<MarkupImageByteBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
    const MarkupImageByteBodyC& Body() const
    { return static_cast<const MarkupImageByteBodyC &>(MarkupInfoC::Body()); }
    //: Body Access. 
    
  };
  
  
}


#endif
