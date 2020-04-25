// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/MessageBox.cc"

#include "Ravl/GUI/MessageBox.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Manager.hh"


namespace RavlGUIN {
  
  //: Constructor.  
  
  MessageBoxBodyC::MessageBoxBodyC(StringC message, bool bYesNo, const char *title, const WindowC* parent)
    : WindowBodyC(200,100,title,GTK_WINDOW_DIALOG,0,false),
      m_strMessage(message),
      m_bYesNo(bYesNo),
      m_sigDone(false),
      m_gotSelfRef(true)
  {
    // Make sure this isn't deleted before the buttons have been clicked
    IncRefCounter();
    
    // Set transience if we need to
    if (parent) {
      if (Manager.IsGUIThread()) {
        GUIMakeTransient(const_cast<WindowC&>(*parent));
      } else {
        MakeTransient(const_cast<WindowC&>(*parent));
      }
    }
  }
  
  //: Create the widget.  
  
  bool MessageBoxBodyC::Create() {
    // Add widgets
    if (m_bYesNo) {
      Add(VBox(Label(m_strMessage) +
	       HBox(ButtonR("Yes",*this,&MessageBoxBodyC::OnClick,true) +
		    ButtonR("No",*this,&MessageBoxBodyC::OnClick,false))
	       ,30,false,20));
    }
    else {
      Add(VBox(Label(m_strMessage) +
	       HBox(ButtonR("OK",*this,&MessageBoxBodyC::OnClick,true)),30,false,20));
    }
    // Set positioning
    GtkWindowPosition pos(GTK_WIN_POS_CENTER);
    GUISetPositioning(pos);

    // Create window
    return WindowBodyC::Create();
  }
  
  bool MessageBoxBodyC::OnClick( bool& bResult) {
    // First, hide the window
    GUIHide();
    
    // Send the "done" signal
    m_sigDone(bResult);
    
    // Delete the box if we are holding the only reference
    if(m_gotSelfRef) { // Protect against acidental double clicks.
      m_gotSelfRef = false;
      if(DecRefCounter()) 
        delete this;
    }
    // Done
    return bResult;
  }

}

