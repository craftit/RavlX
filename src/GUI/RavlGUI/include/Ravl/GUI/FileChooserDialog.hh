// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/FileChooserDialog.hh"
//! lib=RavlGUI
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.GTK.Control"
//! example = "exFileChooser.cc"

#ifndef RAVLGUI_FILECHOOSERDIALOG_HEADER
#define RAVLGUI_FILECHOOSERDIALOG_HEADER 1

#include "Ravl/GUI/FileChooser.hh"

namespace RavlGUIN
{

  //! userlevel=Develop
  //: File chooser dialog.
  
  class FileChooserDialogBodyC
  : public FileChooserBodyC
  {
  public:
    FileChooserDialogBodyC(const FileChooserActionT action,
                           const StringC &title,
                           const StringC &defaultFilename,
                           const bool confirmOverwrite,
                           const bool hideOnResponse,
                           const bool sendEmptyStringOnCancel);
    //: Ctor.
    //!param: action - The dialog mode: open file, save file, open folder or save to folder.
    //!param: title - The dialog title.
    //!param: defaultFilename - When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    //!param: confirmOverwrite - Enable or disable the default overwrite confirmation dialog.
    //!param: hideOnResponse - If enabled, the dialog will disappear when one of the dialog buttons is clicked.
    //!param: sendEmptyStringOnCancel - If enabled, a signal containing an empty string is emitted if cancel is clicked or the dialog is closed.

    FileChooserDialogBodyC(const XMLFactoryContextC &factory);
    //: Ctor.

    virtual bool Create();

    virtual bool Create(GtkWidget *newWidget);

    virtual bool GUISetTitle(const StringC &title);
    //: Set the dialog title.

    void SetHideOnResponse(const bool hideOnResponse)
    { m_hideOnResponse = hideOnResponse; }
    //: If enabled, the dialog will disappear when one of the dialog buttons is clicked.

    bool HideOnResponse() const
    { return m_hideOnResponse; }
    //: Will the dialog will disappear when one of the dialog buttons is clicked?

    void SetSendEmptyStringOnCancel(const bool sendEmptyStringOnCancel)
    { m_sendEmptyStringOnCancel = sendEmptyStringOnCancel; }
    //: If enabled, a signal containing an empty string is emitted if cancel is clicked or the dialog is closed.

    bool SendEmptyStringOnCancel() const
    { return m_sendEmptyStringOnCancel; }
    //: Will a signal containing an empty string be emitted if cancel is clicked or the dialog is closed?

  protected:
    bool CommonCreate(GtkWidget *newWidget = NULL);
    
    bool m_hideOnResponse;
    bool m_sendEmptyStringOnCancel;
  };
  
  //! userlevel=Normal
  //: File chooser dialog.
  
  class FileChooserDialogC
  : public FileChooserC
  {
  public:
    FileChooserDialogC()
    {}
    //: Ctor.

    FileChooserDialogC(bool)
    : FileChooserC(*new FileChooserDialogBodyC(FileChooserOpen, "File Chooser", "", true, true, true))
    {}
    //: Ctor.

    FileChooserDialogC(const FileChooserActionT action,
                       const StringC &title = "File Chooser",
                       const StringC &defaultFilename = "",
                       const bool confirmOverwrite = true,
                       const bool hideOnResponse = true,
                       const bool sendEmptyStringOnCancel = true)
    : FileChooserC(*new FileChooserDialogBodyC(action, title, defaultFilename, confirmOverwrite, hideOnResponse, sendEmptyStringOnCancel))
    {}
    //: Ctor.
    //!param: action - The dialog mode: open file, save file, open folder or save to folder.
    //!param: title - The dialog title.
    //!param: defaultFilename - When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    //!param: confirmOverwrite - Enable or disable the default overwrite confirmation dialog.
    //!param: hideOnResponse - If enabled, the dialog will disappear when one of the dialog buttons is clicked.
    //!param: sendEmptyStringOnCancel - If enabled, a signal containing an empty string is emitted if cancel is clicked or the dialog is closed.
    
    FileChooserDialogC(const XMLFactoryContextC &factory)
    : FileChooserC(*new FileChooserDialogBodyC(factory))
    {}
    //: Ctor.

    FileChooserDialogC(FileChooserDialogBodyC &body)
    : FileChooserC(body)
    {}
    //: Body ctor.

    void SetHideOnResponse(const bool hideOnResponse)
    { Body().SetHideOnResponse(hideOnResponse); }
    //: If enabled, the dialog will disappear when one of the dialog buttons is clicked.

    bool HideOnResponse() const
    { return Body().HideOnResponse(); }
    //: Will the dialog will disappear when one of the dialog buttons is clicked?

    void SetSendEmptyStringOnCancel(const bool sendEmptyStringOnCancel)
    { Body().SetSendEmptyStringOnCancel(sendEmptyStringOnCancel); }
    //: If enabled, a signal containing an empty string is emitted if cancel is clicked or the dialog is closed.

    bool SendEmptyStringOnCancel() const
    { return Body().SendEmptyStringOnCancel(); }
    //: Will a signal containing an empty string be emitted if cancel is clicked or the dialog is closed?

  protected:
    FileChooserDialogBodyC &Body()
    { return static_cast<FileChooserDialogBodyC&>(WidgetC::Body()); }
    //: Body access.
    
    const FileChooserDialogBodyC &Body() const
    { return static_cast<const FileChooserDialogBodyC&>(WidgetC::Body()); }
    //: Body access.
  };
  
}

#endif
