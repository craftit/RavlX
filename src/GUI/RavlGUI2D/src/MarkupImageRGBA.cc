// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D

#include "Ravl/GUI/MarkupImageRGBA.hh"
#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include "Ravl/Image/RealRGBAValue.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlGUIN {
  
  //: Constructor.
  
  MarkupImageRGBABodyC::MarkupImageRGBABodyC(Int64T id,IntT zOrder,const ImageC<ByteRGBAValueC> &img,const Vector2dC &_offset)
    :  MarkupInfoBodyC(id,zOrder),
       image(img),
       offset(_offset)
  {
    coverage = RealRange2dC(image.Frame());
    extent = coverage;
  }
  
  //: Can render method clip the region it updates ?
  
  bool MarkupImageRGBABodyC::CanClip() const { 
    return true; 
  }
  
  //: Method for rendering frame.
  
  bool MarkupImageRGBABodyC::Render(GUIMarkupCanvasBodyC &mv,const RealRange2dC &area,bool selected) {
    IndexRange2dC rng(area.IndexRange());
    rng = rng.ClipBy(image.Frame());
    if((rng.Range1().Min() > rng.Range1().Max()) || (rng.Range2().Min() > rng.Range2().Max())) {
      SysLog(SYSLOG_ERR) << "MarkupImageRGBABodyC::Render, Illegal render request: Image=" << image.Frame() << " Area=" << area << " ";
      return true;
    }
    if(rng.Area() > 0)
      mv.GUIDrawImage(ImageC<ByteRGBAValueC>(image,rng),Point2dC(offset));
    return true;
  }
  
  //: Handle mouse event.
  // Returns true if even has been handled.
  
  bool MarkupImageRGBABodyC::MouseEvent(GUIMarkupCanvasBodyC &mv,const Point2dC &at,MouseEventTypeT eventType,const MouseEventC &me,IntT &state,bool &refresh) {
    return false;
  }
  
}
