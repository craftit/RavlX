// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/TextView.cc"
//! lib=RavlGUI
//! author = "Warren Moore"

#include "Ravl/GUI/TextView.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN
{
  
  TextViewBodyC::TextViewBodyC(const StringC &text) :
    m_text(text)
  {}
  
  bool TextViewBodyC::CommonCreate(GtkWidget *_widget)
  {
    ONDEBUG(std::cerr << "TextViewBodyC::Create()\n");
    
    if (widget != 0)
      return true;
    
    if (_widget == NULL)
    {
      widget = gtk_text_view_new();
    }
    else
    {
      widget = _widget;
    }

    GtkTextBuffer *textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
    if (textBuffer)
    {
      gtk_text_buffer_set_text(textBuffer, m_text.chars(), -1);
    }
    
    gtk_text_view_set_editable(GTK_TEXT_VIEW(widget), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widget), FALSE);
    
    ConnectSignals();
    
    return true;
  }

}
