// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/FileChooserButton.cc"
//! lib=RavlGUI
//! author = "Warren Moore"

#include "Ravl/GUI/FileChooserButton.hh"
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

  static void CBFileChooserButtonResponse(GtkWidget *widget, FileChooserButtonBodyC *fileChooserButtonBodyPtr)
  {
    ONDEBUG(std::cerr << "CBFileChooserButtonResponse " << endl);
    RavlAssert(fileChooserButtonBodyPtr);

    StringC filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));

    fileChooserButtonBodyPtr->CBUpdateFilename();

    fileChooserButtonBodyPtr->SigSelected()(filename);
  }

  FileChooserButtonBodyC::FileChooserButtonBodyC(const FileChooserActionT action,
                                                 const StringC &title,
                                                 const StringC &defaultFilename,
                                                 const bool confirmOverwrite)
  : FileChooserBodyC(action, title, defaultFilename, confirmOverwrite)
  {}

  FileChooserButtonBodyC::FileChooserButtonBodyC(const XMLFactoryContextC &factory)
  : FileChooserBodyC(factory)
  {}

  bool FileChooserButtonBodyC::Create()
  {
    return CommonCreate();
  }

  bool FileChooserButtonBodyC::Create(GtkWidget *newWidget)
  {
    return CommonCreate(newWidget);
  }

  bool FileChooserButtonBodyC::CommonCreate(GtkWidget *newWidget)
  {
    ONDEBUG(std::cerr << "FileChooserButtonBodyC::CommonCreate" << endl);

    if (newWidget)
    {
      if (!FileChooserBodyC::Create(newWidget))
        return false;
    }
    else
    {
      GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

      switch (m_action)
      {
        case FileChooserSave: action = GTK_FILE_CHOOSER_ACTION_SAVE; break;
        case FileChooserSelectFolder: action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER; break;
        case FileChooserCreateFolder: action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER; break;
        case FileChooserOpen:
        default:
          break;
      }

      widget = gtk_file_chooser_button_new(m_title, action);

      gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(widget), true);

      GUISetConfirmOverwrite(m_confirmOverwrite);

      if (!FileChooserBodyC::Create())
        return false;
    }

    gtk_signal_connect(GTK_OBJECT(GTK_FILE_CHOOSER_BUTTON(widget)),
                       "file-set",
                       (GtkSignalFunc)CBFileChooserButtonResponse,
                       this);

    return true;
  }

  bool FileChooserButtonBodyC::GUISetTitle(const StringC &title)
  {
    m_title = title;

    if (widget)
    {
      RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");

      gtk_file_chooser_button_set_title(GTK_FILE_CHOOSER_BUTTON(widget), m_title);
    }

    return true;
  }

  static XMLFactoryRegisterHandleConvertC<FileChooserButtonC, WidgetC> g_registerXMLFactoryFileChooserButton("RavlGUIN::FileChooserButtonC");

  void LinkFileChooserButton() {}

}
