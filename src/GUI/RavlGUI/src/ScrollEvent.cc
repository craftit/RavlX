// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/ScrollEvent.cc"
//! lib=RavlGUI
//! author = "Warren Moore"

#include "Ravl/GUI/ScrollEvent.hh"
#include "Ravl/Stream.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN
{

  ScrollEventC::ScrollEventC(RealT col, RealT row, IntT state, UIntT time)
  : MouseEventC(col, row, state, 0, time),
    m_direction(ScrollDirectionUnknown)
  {}

  ScrollEventC::ScrollEventC(GdkEventScroll &event)
  : MouseEventC(event.x, event.y, event.state, 0, event.time),
    m_direction(ScrollDirectionUnknown)
  {
    switch (event.direction)
    {
      case GDK_SCROLL_UP: m_direction = ScrollDirectionUp; break;
      case GDK_SCROLL_DOWN: m_direction = ScrollDirectionDown; break;
      case GDK_SCROLL_LEFT: m_direction = ScrollDirectionLeft; break;
      case GDK_SCROLL_RIGHT: m_direction = ScrollDirectionRight; break;
      default: break;
    }
  }

  std::ostream &operator<<(std::ostream &stream, const ScrollEventC &event)
  {
    return stream;
  }

}
