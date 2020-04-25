// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/ButtonBox.cc"

#include "Ravl/GUI/ButtonBox.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/SArray1dIter.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  
  //: Constructor.  
  ButtonBoxBodyC::ButtonBoxBodyC(StringC message, SArray1dC<StringC> & buttons, const char *title, const WindowC* parent)
    : WindowBodyC(200,100,title,GTK_WINDOW_DIALOG,0,false),
      m_strMessage(message),
      m_buttons(buttons),
      sigDone((UIntT) 0)
  {
    // Make sure this isn't deleted before the buttons have been clicked
    IncRefCounter();
    // Set transience if we need to
    if (parent) 
      MakeTransient(const_cast<WindowC&>(*parent));
  }
  

  //: Create the widget.  
  bool ButtonBoxBodyC::Create() {
    // Add widgets
    LBoxC lbox(false);
    for(SArray1dIterC<StringC>it(m_buttons);it;it++) {
      lbox.GUIAdd(ButtonR(*it,*this,&ButtonBoxBodyC::OnClick,(UIntT)it.Index().V()));
    }
    Add(VBox(Label(m_strMessage) + lbox,30,false,20));

    // Set positioning
    GtkWindowPosition pos(GTK_WIN_POS_CENTER);
    GUISetPositioning(pos);

    // Create window
    return WindowBodyC::Create();
  }
  
  bool ButtonBoxBodyC::OnClick(UIntT & buttonNo) {
    // First, hide the window
    GUIHide();
    // Send the "done" signal
    sigDone(buttonNo);
    // Delete the box if we are holding the only reference
    if(DecRefCounter())
      delete this;
    // Done
    return true;
  }

}

