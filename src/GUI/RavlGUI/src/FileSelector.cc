// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! docentry="Graphics.GTK"
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/FileSelector.cc"

#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/GUI/Manager.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {

  /* Get the selected filename and print it to the console */
  static void file_ok_sel (GtkWidget *w, FileSelectorBodyC *fs) { 
    if(fs->HideOnSelect())
      fs->GUIHide(); // We're in the GUI thread.
    fs->Selected()(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs->Widget()))); 
  }
  
  static void cancel (GtkWidget *widget, FileSelectorBodyC *fs) { 
    fs->HandleCancel(); // We're in the GUI thread.
  } 
  
  //: Constructor.
  
  FileSelectorBodyC::FileSelectorBodyC(const StringC &nname,const StringC &filename,bool _sendEmptyStringOnCancel)
    : name(nname),
      defaultFN(filename),
      hideOnSelect(true),
      sendEmptyStringOnCancel(_sendEmptyStringOnCancel),
      selected(StringC(""))
  {}
  
  
  bool FileSelectorBodyC::Create() {
    widget = gtk_file_selection_new (name);
  
    // Connect default signal, to handle widget destructon correctly
    ConnectSignals(); 
    
    /* Connect the ok_button to file_ok_sel function */
    gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (widget)->ok_button),
			"clicked", (GtkSignalFunc) file_ok_sel, this );
    
    /* Connect the cancel button to the destroyed function */
    
    gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (widget)->cancel_button),
			"clicked", (GtkSignalFunc) cancel, this );
    
    /* Lets set the filename, as if this were a save dialog, and we are giving
       a default filename */
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(widget), 
				     defaultFN);
    
    ConnectSignals();
    
    return true;
  }

  //: Handle selection cancel.
  void FileSelectorBodyC::HandleCancel() {
    GUIHide();
    if(sendEmptyStringOnCancel) {
      StringC empty;
      selected(empty);
    }
  }
  
  //: Dissconnect all signals.
  
  void FileSelectorBodyC::Destroy() {
    selected.DisconnectAll();
    WidgetBodyC::Destroy();
  }


  //: Set the title of the window.
  // GUI thread only.
  
  bool FileSelectorBodyC::GUISetTitle(const StringC &nname) {
    //cerr << "FileSelectorBodyC::GUISetTitle(), Called. \n";
    name = nname;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_window_set_title(GTK_WINDOW(widget),nname);    
    return true;
  }
  
  //: Set the title of the window.
  
  bool FileSelectorBodyC::SetTitle(const StringC &nname) {
    Manager.Queue(Trigger(FileSelectorC(*this),&FileSelectorC::GUISetTitle,nname));
    return true;
  }
  
  //: Set file to open
  // GUI thread only.
  
  bool FileSelectorBodyC::GUISetFilename(const StringC &nname) {
    defaultFN = nname;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(widget), 
				     defaultFN);
    return true;
  }
  
  //: Set file to open
  
  bool FileSelectorBodyC::SetFilename(const StringC &nname) {
    Manager.Queue(Trigger(FileSelectorC(*this),&FileSelectorC::GUISetFilename,nname));
    return true;
  }
  
}
