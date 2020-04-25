// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////
//! docentry="Graphics.GTK"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/TextEntry.cc"

#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/ReadBack.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  TextEntryBodyC::TextEntryBodyC(const StringC &ntext,IntT nMaxLen,bool nsigAllChanges,bool editable, IntT xdim,IntT ydim)
    : text(ntext),
      maxLen(nMaxLen),
      sigAllChanges(nsigAllChanges),
      bPasswdMode(false),
      activate(text),
      xsize(xdim), 
      ysize(ydim),
      isEditable(editable)
  {}
  
  //: XML factory constructor

  TextEntryBodyC::TextEntryBodyC(const XMLFactoryContextC &factory)
  : text(factory.AttributeString("value","")),
    maxLen(factory.AttributeInt("maxLen",-1)),
    sigAllChanges(factory.AttributeBool("signalAllChanges",false)),
    bPasswdMode(factory.AttributeBool("passwordMode",false)),
    activate(text),
    xsize(factory.AttributeInt("xDim",-1)),
    ysize(factory.AttributeInt("yDim",-1)),
    isEditable(factory.AttributeBool("editable",true))
  {
    factory.UseComponent("sigActivate",activate,true,typeid(Signal1C<StringC>));
  }

  //: Got a changed signal.
  
  bool TextEntryBodyC::SigChanged() {
    ONDEBUG(std::cerr << "TextEntryBodyC::SigChanged() \n");
    MutexLockC lock(access);
    text = StringC(gtk_entry_get_text(GTK_ENTRY(Widget())));
    lock.Unlock();
    if(sigAllChanges)
      activate(text);
    return true;
  }
  
  //: Got a activate signal.
  
  bool TextEntryBodyC::SigActivate()  {  
    ONDEBUG(std::cerr << "TextEntryBodyC::SigActivate() \n"); 
    MutexLockC lock(access);
    text = StringC(gtk_entry_get_text(GTK_ENTRY(Widget())));
    lock.Unlock();
    activate(text);
    return true;
  }
  
  //: Create the widget.
  
  bool TextEntryBodyC::Create() {
    ONDEBUG(std::cerr << "TextEntryBodyC::Create() MaxLen=" << maxLen << " Text='" << text << "'\n");
    if(maxLen > 0)
      widget = gtk_entry_new_with_max_length(maxLen);
    else
      widget = gtk_entry_new();
    if(!text.IsEmpty())
      gtk_entry_set_text (GTK_ENTRY (widget), text);
    GUISetUSize( xsize, ysize ) ; 
    gtk_editable_set_editable(GTK_EDITABLE(widget), isEditable) ;
    
    ConnectSignals();
    ConnectRef(Signal("changed"),*this,&TextEntryBodyC::SigChanged);
    ConnectRef(Signal("activate"),*this,&TextEntryBodyC::SigActivate);

    // Set password mode if necessary
    if (bPasswdMode) GUIHideText(bPasswdMode);
    // Done
    return true;
  }
  


  bool TextEntryBodyC::Create(GtkWidget *newWidget) {
    widget = newWidget;
    if(!text.IsEmpty())
      gtk_entry_set_text (GTK_ENTRY (widget), text);
      
    ConnectSignals();
    ConnectRef(Signal("changed"),*this,&TextEntryBodyC::SigChanged);
    ConnectRef(Signal("activate"),*this,&TextEntryBodyC::SigActivate);

    // Set password mode if necessary
    if (bPasswdMode) GUIHideText(bPasswdMode);
    // Done
    return true;
  }

  //: Access text

  StringC TextEntryBodyC::GUIText()
  {
    MutexLockC lock(access);
    RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");
    text = StringC(gtk_entry_get_text(GTK_ENTRY(Widget())));
    StringC ret = text;
    lock.Unlock();
    return ret;
  }

  //: Access text

  StringC TextEntryBodyC::Text() const {
    MutexLockC lock(access);
    StringC ret = text; // Ensure the right order.
    lock.Unlock();
    return ret;
  }
  
  //: Set text to edit.
  // This should only be called within the GUI thread.
  
  bool TextEntryBodyC::GUISetText(const StringC &txt) {
    MutexLockC lock(access);
    text=txt;
    lock.Unlock();
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_entry_set_text (GTK_ENTRY (widget), text);
    return true;
  }
  
  //: Update text.
  // This is thread safe.
  
  bool TextEntryBodyC::Text(const StringC &txt) {
    Manager.Queue(Trigger(TextEntryC(*this),&TextEntryC::GUISetText,txt));
    return true;
  }
  
  //: Set text to display.
  
  bool TextEntryBodyC::SetText(const StringC &txt) {
    gtk_entry_set_text (GTK_ENTRY (widget), txt);
    return true;
  }

  bool TextEntryBodyC::GUIHideText(bool hide) {
    bPasswdMode = hide;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_entry_set_visibility (GTK_ENTRY (widget), !hide);
    return true;
  }
  
  bool TextEntryBodyC::HideText(bool hide) {
    Manager.Queue(Trigger(TextEntryC(*this),&TextEntryC::GUIHideText,hide));
    return true;
  }



  bool TextEntryBodyC::Editable() const
  {
    ReadBackLockC rbLock;
    MutexLockC lock(access);
    return gtk_editable_get_editable(GTK_EDITABLE(widget)) != 0;
  }
  
  
  
  bool TextEntryBodyC::GUISetEditable(const bool editable) 
  {
    isEditable = editable ; 
    if (widget == 0)
      return true ; 
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    gtk_editable_set_editable(GTK_EDITABLE(widget), editable) ;
    return true ; 
  }



  bool TextEntryBodyC::SetEditable(const bool editable) 
  {
    Manager.Queue(Trigger(TextEntryC(*this), &TextEntryC::GUISetEditable, editable)) ;
    return true ; 
  }

  static XMLFactoryRegisterHandleConvertC<TextEntryC,WidgetC> g_registerXMLFactoryTextEntry("RavlGUIN::TextEntryC");


}
