// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/FileChooserDialog.cc"
//! lib=RavlGUI
//! author = "Warren Moore"

#include "Ravl/GUI/FileChooserDialog.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/XMLFactoryRegister.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN
{

  static void CBFileChooserDialogResponse(GtkWidget *widget, gint response, FileChooserDialogBodyC *fileChooserDialogBodyPtr)
  {
    ONDEBUG(std::cerr << "CBFileChooserDialogResponse response(" << response << ")" << endl);
    RavlAssert(fileChooserDialogBodyPtr);

    if (fileChooserDialogBodyPtr->HideOnResponse())
      fileChooserDialogBodyPtr->GUIHide();

    bool fileSelected = false;
    StringC filename;
    switch (response)
    {
      case GTK_RESPONSE_OK:
      case GTK_RESPONSE_ACCEPT:
      {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
        fileSelected = true;
        break;
      }
      case GTK_RESPONSE_CANCEL:
      case GTK_RESPONSE_DELETE_EVENT:
      default:
        break;
    }

    if (fileSelected)
      fileChooserDialogBodyPtr->CBUpdateFilename();
    
    if (fileSelected || fileChooserDialogBodyPtr->SendEmptyStringOnCancel())
      fileChooserDialogBodyPtr->SigSelected()(filename);
  }

  FileChooserDialogBodyC::FileChooserDialogBodyC(const FileChooserActionT action,
                                                 const StringC &title,
                                                 const StringC &defaultFilename,
                                                 const bool confirmOverwrite,
                                                 const bool hideOnResponse,
                                                 const bool sendEmptyStringOnCancel)
  : FileChooserBodyC(action, title, defaultFilename, confirmOverwrite),
    m_hideOnResponse(hideOnResponse),
    m_sendEmptyStringOnCancel(sendEmptyStringOnCancel)
  {}

  FileChooserDialogBodyC::FileChooserDialogBodyC(const XMLFactoryContextC &factory)
  : FileChooserBodyC(factory),
    m_hideOnResponse(factory.AttributeBool("hideOnResponse", true)),
    m_sendEmptyStringOnCancel(factory.AttributeBool("sendEmptyStringOnCancel", false))
  {}

  bool FileChooserDialogBodyC::Create()
  {
    return CommonCreate();
  }

  bool FileChooserDialogBodyC::Create(GtkWidget *newWidget)
  {
    return CommonCreate(newWidget);
  }

  bool FileChooserDialogBodyC::CommonCreate(GtkWidget *newWidget)
  {
    ONDEBUG(std::cerr << "FileChooserDialogBodyC::CommonCreate widget(" << (newWidget ? "Y" : "N") << ")" << endl);

    if (newWidget)
    {
      if (!FileChooserBodyC::Create(newWidget))
        return false;
    }
    else
    {
      GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
      StringC actionButton = GTK_STOCK_OPEN;

      switch (m_action)
      {
        case FileChooserSave:
          action = GTK_FILE_CHOOSER_ACTION_SAVE;
          actionButton = GTK_STOCK_SAVE;
          break;
        case FileChooserSelectFolder:
          action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
          break;
        case FileChooserCreateFolder:
          action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
          actionButton = GTK_STOCK_SAVE;
          break;
        case FileChooserOpen:
        default:
          break;
      }

      widget = gtk_file_chooser_dialog_new(m_title,
                                           NULL,
                                           action,
                                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                           actionButton.chars(), GTK_RESPONSE_ACCEPT,
                                           NULL);

      gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(widget), true);

      GUISetConfirmOverwrite(m_confirmOverwrite);

      if (!FileChooserBodyC::Create())
        return false;
    }

    gtk_signal_connect(GTK_OBJECT(GTK_DIALOG(widget)),
                       "response",
                       (GtkSignalFunc)CBFileChooserDialogResponse,
                       this);

    return true;
  }

  bool FileChooserDialogBodyC::GUISetTitle(const StringC &title)
  {
    m_title = title;

    if (widget)
    {
      RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");

      gtk_window_set_title(GTK_WINDOW(widget), m_title);
    }

    return true;
  }

  static XMLFactoryRegisterHandleConvertC<FileChooserDialogC, WidgetC> g_registerXMLFactoryFileChooserDialog("RavlGUIN::FileChooserDialogC");

  void LinkFileChooserDialog() {}

}
