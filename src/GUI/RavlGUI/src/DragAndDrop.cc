// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/DragAndDrop.cc"

#include "Ravl/GUI/DragAndDrop.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  //: Finish drag and drop.
  // This must be called on the GUI thread.
  
  bool DNDDataInfoC::Finish(bool success,bool del) {
    RavlAssert(context != 0);
    ONDEBUG(std::cerr << "DNDDataInfoC::Finish, \n");
    gtk_drag_finish(context,success,del,time);
#if DODEBUG
    std::cerr << "  DND Protocol=" << context->protocol << "\n";
    std::cerr << "  DND is_source =" << context->is_source << "\n";
    std::cerr << "  DND source_window =" << context->source_window << "\n";
    std::cerr << "  DND dest_window =" << context->dest_window << "\n";
    std::cerr << "  DND targets =" << context->targets << "\n";
#endif    
    return true;
  }
  
  // Find the GTK source widget.
  
  GtkWidget *DNDDataInfoC::GTKSourceWidget() {
    RavlAssert(context != 0);
    ONDEBUG(std::cerr << "DNDDataInfoC::GTKSourceWidget, Get source widget. \n");
    return gtk_drag_get_source_widget (context);
  }
  
  //: Is recieved data a string ?
  
  bool DNDDataInfoC::IsString() const {
    if(data == 0)
      return false;
    return (data->format == 8); // This is really a guess, but it seems to be faily standard.
  }
  
  //: Get data as string.
  // It is the user's responsibility to ensure this is appropriate.
  
  StringC DNDDataInfoC::String() {
    if(data == 0)
      return StringC();
    if(data->format != 8)
      std::cerr << "DNDDataInfoC::String(), WARNING: Selection may not be a string. \n";    
    return StringC((char *) data->data,data->length);
  }

  //: Put a string.
  
  bool DNDDataInfoC::PutString(int dtype,const StringC &str) {
    ONDEBUG(std::cerr << "DNDDataInfoC::PutString, DND target type =" << data->target << "\n");    
    gtk_selection_data_set_text(data,const_cast<gchar *>((const gchar *)str.chars()),str.Size());
    ONDEBUG(std::cerr << "DNDDataInfoC::PutString, Done.\n");
    return true;
  }


  //: Get suggested action.
  // See GdkDragAction for definition of bit mask.
  
  DragActionT DNDDataInfoC::SuggestedAction() {
    if(context == 0)
      return 0;
    return context->suggested_action;
  }
  
  //: Possible actions that can be taken
  // See GdkDragAction for definition of bit mask.
  
  DragActionT DNDDataInfoC::Actions() {
    if(context == 0)
      return 0;
    return context->actions;
  }
  
}
