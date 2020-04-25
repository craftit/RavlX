// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_MOUSEEVENT_HEADER
#define RAVLGUI_MOUSEEVENT_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/MouseEvent.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Internal"
//! date="08/04/1999"

#include "Ravl/Types.hh"
#include "Ravl/Index2d.hh"

// Namespace fix for Visual C++
#if RAVL_COMPILER_VISUALCPP6
#ifndef RavlGUIN
#define RavlGUIN RavlN
#endif
#endif

typedef struct _GdkEventMotion	    GdkEventMotion;
typedef struct _GdkEventButton	    GdkEventButton;

namespace RavlGUIN {
  
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Mouse event information.
  
  class MouseEventC {
  public:
    MouseEventC(RealT col = 0,RealT row = 0,IntT state = 0,IntT changed = 0,UIntT time = 0);
    //: Constructor.
    
    MouseEventC(GdkEventButton &ev);
    //: Constructor.
    
    MouseEventC(GdkEventMotion &ev);
    //: Constructor.
    
    RealT Row() const { return y; }
    //: Get row number of position.
    
    RealT Col() const { return x; }
    //: Get column number of position.
    
    Index2dC At() const { return Index2dC(y,x); }
    //: Get position.
    // Position of mouse click in RAVL co-ordinates (row, col).
    
    bool IsPressed(IntT buttonNo = 0) const;
    //: Test if a button is pessed.

    bool IsDoublePressed() const
    { return m_doublePress; }

    bool HasChanged(IntT buttonNo = 0) const;
    //: Has button changed state ?
    
    IntT MaxButtons() const { return 5; }
    //: Maximum buttons available.
    
    bool IsShift() const;
    //: Is shift down ?
    
    bool IsCntrl() const;
    //: Is control down ?
    
    bool IsLock() const;
    //: Is the caps lock key down ?
    
    bool IsAlt() const;
    //: Is the alt key pressed?
    
    bool IsMod5() const;
    //: Forth modifier key.
    
    bool IsMod6() const;
    //: Sixth modifier key.

    bool IsMod7() const;
    //: Seventh modifier key.
    // Can be 'Windows Key'
    
    bool IsMod8() const;
    //: Eight modifier key.
    // Can be 'Alt Gr'
    
    IntT RawState() const
    { return state; }
    //: Access raw state value.
    
    IntT RawChanged() const
    { return change; }
    //: Raw button changed bits.
    
    UIntT Time() const
    { return time; }
    //: Time of event (from gdk)
  protected:
    RealT x,y;   // Current position in GTK coords.
    IntT state;  // Current state.
    IntT change; // Change flags.
    UIntT time;  // Time of event.
    bool m_doublePress;
  };
  
  std::ostream &operator<<(std::ostream &,const MouseEventC &me);
  //: Print description of mouse event to stream.
  
  /////// CListEventC ////////////////////////////////////////////////
  
  class CListEventC {
  public:
    CListEventC(IntT nrowId,IntT nrow,IntT ncol,GdkEventButton *mevent)
      : rowId(nrowId),
      row(nrow),
      col(ncol),
      event(*mevent)
      {}
    //: Constructor.
    
    CListEventC(IntT nrowId,IntT nrow,IntT ncol)
      : rowId(nrowId),
      row(nrow),
      col(ncol)
      {}
    //: Constructor.
    
    CListEventC()
      {}
    //: Default constructor.
    
    IntT &RowID()
      { return rowId; }
    //: Access row id.
    
    IntT RowID() const
      { return rowId; }
    //: Access row id.

    MouseEventC &MouseEvent()
      { return event; }
    //: Access mouse event.

    const MouseEventC &MouseEvent() const
      { return event; }
    //: Access mouse event.
    
    IntT rowId;
    IntT row;
    IntT col;
    MouseEventC event;
  };
  
}

#endif
