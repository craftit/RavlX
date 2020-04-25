// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

#include "Ravl/GUI/MarkupNote.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  
  //: Constructor.
  
  MarkupNoteBodyC::MarkupNoteBodyC(Int64T id,IntT zOrder,const StringC &_text,const MarkupInfoC &_obj,const Vector2dC &_offset) 
    : MarkupInfoBodyC(id,zOrder),
      text(_text),
      obj(_obj),
      offset(_offset)
  {}
  
  //: Range of pixels covered by this one (Garanteed) 
  
  RealRange2dC MarkupNoteBodyC::Coverage(GUIMarkupCanvasBodyC &mv) const {
    return RealRange2dC(0,0);
#if 0
    if(mv.Widget() == 0)
      return RealRange2dC(0,0);
    IndexRange2dC rect;
    Point2dC txtPosition;
    ComputeRectangle(mv,rect,txtPosition);
    return rect;
#endif
  }
  
  //: Extent of object.
  
  RealRange2dC MarkupNoteBodyC::Extent(GUIMarkupCanvasBodyC &mv) const {
    if(mv.Widget() == 0)
      return RealRange2dC(10,10);
    
    IndexRange2dC rect;
    Point2dC txtPosition;
    ComputeRectangle(mv,rect,txtPosition);
    
    return RealRange2dC(rect.Expand(1));
  }
  
  //: Compute text rectangle and position
  
  bool MarkupNoteBodyC::ComputeRectangle(GUIMarkupCanvasBodyC &mv,IndexRange2dC &rect,Point2dC &txtPosition) const{
    Point2dC off = Point2dC(4,4) / mv.Scale();
    if(off[0] < 1) off[0] = 1;
    if(off[1] < 1) off[1] = 1;
    
    GdkFont *font = mv.GUIDrawFont();
    Point2dC textSize = Point2dC(mv.GUITextSize(font,text));
    Point2dC centroid = obj.Centroid(mv) + offset;
    
    Point2dC origin = centroid -  Point2dC(textSize[0]/2,textSize[1]/2);
    Index2dC iorigin(origin - off);
    Index2dC iend(origin + textSize + off);
    rect = IndexRange2dC(iorigin,iend);
    
    txtPosition  = centroid  + Point2dC(textSize[0]/2,-textSize[1]/2);
    return true;
  }
  
  //: Method for rendering frame.
  
  bool MarkupNoteBodyC::Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected) {
    Point2dC txtcent;
    IndexRange2dC rect;
    
    GdkFont *font = mv.GUIDrawFont();
    
    ComputeRectangle(mv,rect,txtcent);
    
#if 1
    ImageC<ByteRGBValueC> background = mv.FrameMarkup().Image();
    rect.ClipBy(background.Frame());
    if(rect.Area() > 0) {
      ImageC<ByteRGBValueC> subImg = ImageC<ByteRGBValueC>(background,rect).Copy();
      subImg /= ByteRGBValueC(2,2,2);
      mv.GUIDrawImage(subImg,Point2dC(0,0));
    }
#endif
    
    // Set up the gc
    if (!SetDrawStyle(mv))
      return true;
    GdkGC *dc = mv.GcDrawContext();

    mv.GUIDrawText(dc,font,txtcent,text);
    
    return true; 
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupNoteBodyC::MouseEvent(GUIMarkupCanvasBodyC &mv,const Point2dC &at,MouseEventTypeT eventType,const MouseEventC &me,IntT &state,bool &refresh) {
    return false; 
  }
  
}
