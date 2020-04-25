// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////
//! rcsid="$Id$
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/MouseEvent.cc"

#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/Stream.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  //: Constructor.
  
  MouseEventC::MouseEventC(RealT nx,RealT ny,IntT nstate,IntT _changed,UIntT _time) 
    : x(nx),
      y(ny),
      state(nstate),
      change(_changed),
      time(_time),
      m_doublePress(false)
  {}
  
  //: Constructor.
  
  MouseEventC::MouseEventC(GdkEventButton &ev)
    : x(ev.x),
      y(ev.y),
      state(ev.state),
      change(0),
      time(ev.time),
      m_doublePress(ev.type == GDK_2BUTTON_PRESS)
  {
    change = 1 << (ev.button - 1);
    ONDEBUG(std::cerr << "MouseEventC::MouseEventC(), Button: " << ev.button << "\n");
  }
  
  //: Constructor.
  
  MouseEventC::MouseEventC(GdkEventMotion &event)
    : x(event.x),
      y(event.y),
      state(event.state),
      change(0),
      time(event.time)
  {
    if (event.is_hint) {
      IntT ix,iy;
      gdk_window_get_pointer (event.window, &ix, &iy,(GdkModifierType *) &state);
      x = ix;
      y = iy;
    } else {
      x = event.x;
      y = event.y;
      state = event.state;
    }
  }
  
  //: Test if a button is pessed.
  
  bool MouseEventC::IsPressed(IntT buttonNo) const {
    if(buttonNo < 0 || buttonNo >= MaxButtons())
      return false;
    return (state & (1 << (8 + buttonNo))) != 0;
  }
  
  //: Has button changed state ?
  
  bool MouseEventC::HasChanged(IntT buttonNo) const {
    if(buttonNo < 0 || buttonNo >= MaxButtons())
      return false;
    return (change & (1 << buttonNo)) != 0; 
  }
  
  
  //: Is shift down ?
  
  bool MouseEventC::IsShift() const { 
    return (state & GDK_SHIFT_MASK) != 0; 
  }
  
  //: Is control down ?
  
  bool MouseEventC::IsCntrl() const { 
    return (state & GDK_CONTROL_MASK) != 0; 
  }
  
  //: Is the caps lock key down ?
  
  bool MouseEventC::IsLock() const {
    return (state & GDK_LOCK_MASK) != 0;
  }
  
  //: Is the alt key pressed?
  
  bool MouseEventC::IsAlt() const {
    return (state & GDK_MOD1_MASK) != 0;
  }
  
  //: Forth modifier key.
  
  bool MouseEventC::IsMod5() const {
    return (state & GDK_MOD2_MASK) != 0;
  }

  //: Sixth modifier key.
  
  bool MouseEventC::IsMod6() const {
    return (state & GDK_MOD3_MASK) != 0;
  }

  //: Seventh modifier key.
  
  bool MouseEventC::IsMod7() const {
    return (state & GDK_MOD4_MASK) != 0;
  }
  
  //: Eight modifier key.
  
  bool MouseEventC::IsMod8() const {
    return (state & GDK_MOD5_MASK) != 0;
  }


  std::ostream &operator<<(std::ostream &s,const MouseEventC &me) {
    //s << " x:" << me.X() << " y:" << me.Y();
    return s;
  }
}
