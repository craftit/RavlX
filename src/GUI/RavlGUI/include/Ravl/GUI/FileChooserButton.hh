// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/FileChooserButton.hh"
//! lib=RavlGUI
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.GTK.Control"
//! example = "exFileChooser.cc"

#ifndef RAVLGUI_FILECHOOSERBUTTON_HEADER
#define RAVLGUI_FILECHOOSERBUTTON_HEADER 1

#include "Ravl/GUI/FileChooser.hh"

namespace RavlGUIN
{

  //! userlevel=Develop
  //: File chooser button.
  
  class FileChooserButtonBodyC
  : public FileChooserBodyC
  {
  public:
    FileChooserButtonBodyC(const FileChooserActionT action,
                           const StringC &title,
                           const StringC &defaultFilename,
                           const bool confirmOverwrite);
    //: Ctor.
    //!param: action - The dialog mode: open file, save file, open folder or save to folder.
    //!param: title - The dialog title.
    //!param: defaultFilename - When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    //!param: confirmOverwrite - Enable or disable the default overwrite confirmation dialog.

    FileChooserButtonBodyC(const XMLFactoryContextC &factory);
    //: Ctor.

    virtual bool Create();

    virtual bool Create(GtkWidget *newWidget);

    virtual bool GUISetTitle(const StringC &title);
    //: Set the dialog title.

  protected:
    bool CommonCreate(GtkWidget *newWidget = NULL);
  };
  
  //! userlevel=Normal
  //: File chooser button.
  
  class FileChooserButtonC
  : public FileChooserC
  {
  public:
    FileChooserButtonC()
    {}
    //: Ctor.

    FileChooserButtonC(bool)
    : FileChooserC(*new FileChooserButtonBodyC(FileChooserOpen, "File Chooser", "", true))
    {}
    //: Ctor.

    FileChooserButtonC(const FileChooserActionT action,
                       const StringC &title = "File Chooser",
                       const StringC &defaultFilename = "",
                       const bool confirmOverwrite = true)
    : FileChooserC(*new FileChooserButtonBodyC(action, title, defaultFilename, confirmOverwrite))
    {}
    //: Ctor.
    //!param: action - The dialog mode: open file, save file, open folder or save to folder.
    //!param: title - The dialog title.
    //!param: defaultFilename - When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    //!param: confirmOverwrite - Enable or disable the default overwrite confirmation dialog.
    
    FileChooserButtonC(const XMLFactoryContextC &factory)
    : FileChooserC(*new FileChooserButtonBodyC(factory))
    {}
    //: Ctor.

    FileChooserButtonC(FileChooserButtonBodyC &body)
    : FileChooserC(body)
    {}
    //: Body ctor.

  protected:
    FileChooserButtonBodyC &Body()
    { return static_cast<FileChooserButtonBodyC&>(WidgetC::Body()); }
    //: Body access.
    
    const FileChooserButtonBodyC &Body() const
    { return static_cast<const FileChooserButtonBodyC&>(WidgetC::Body()); }
    //: Body access.
  };
  
}

#endif
