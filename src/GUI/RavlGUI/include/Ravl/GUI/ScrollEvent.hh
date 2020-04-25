// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/ScrollEvent.hh"
//! lib=RavlGUI
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.GTK.Internal"

#ifndef RAVLGUI_SCROLLEVENT_HEADER
#define RAVLGUI_SCROLLEVENT_HEADER 1

#include "Ravl/GUI/MouseEvent.hh"

// Namespace fix for Visual C++
#if RAVL_COMPILER_VISUALCPP6
#ifndef RavlGUIN
#define RavlGUIN RavlN
#endif
#endif

typedef struct _GdkEventScroll GdkEventScroll;

namespace RavlGUIN
{
  
  enum ScrollDirectionT
  {
    ScrollDirectionUnknown,
    ScrollDirectionUp,
    ScrollDirectionDown,
    ScrollDirectionLeft,
    ScrollDirectionRight
  };
  
  //! userlevel=Normal
  //: Scroll event information.
  
  class ScrollEventC
  : public MouseEventC
  {
  public:
    ScrollEventC(RealT col = 0, RealT row = 0, IntT state = 0, UIntT time = 0);
    //: Ctor.
    
    ScrollEventC(GdkEventScroll &event);
    //: Ctor.

    ScrollDirectionT Direction()
    { return m_direction; }
    //: Access the scroll direction.
    
  protected:
    ScrollDirectionT m_direction;
  };
  
  std::ostream &operator<<(std::ostream &stream, const ScrollEventC &event);
  //: Print description of scroll event to stream.
  
}

#endif
