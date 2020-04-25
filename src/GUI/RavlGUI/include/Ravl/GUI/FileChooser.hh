// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/FileChooser.hh"
//! lib=RavlGUI
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.GTK.Control"
//! example = "exFileChooser.cc"

#ifndef RAVLGUI_FILECHOOSER_HEADER
#define RAVLGUI_FILECHOOSER_HEADER 1

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal3.hh"
#include <gtk/gtk.h>
#include "Ravl/XMLFactory.hh"
#include "Ravl/ConfigParameter.hh"

namespace RavlGUIN
{

  enum FileChooserActionT
  {
    FileChooserOpen, // Select a file to open.
    FileChooserSave, // Select a file to save.
    FileChooserSelectFolder, // Select a folder.
    FileChooserCreateFolder // Select or create a folder.
  };

  //! userlevel=Develop
  //: File chooser.
  
  class FileChooserBodyC
  : public WidgetBodyC
  {
  public:
    FileChooserBodyC(const FileChooserActionT action,
                     const StringC &title,
                     const StringC &defaultFilename,
                     const bool confirmOverwrite);
    //: Ctor.
    //!param: action - The dialog mode: open file, save file, open folder or save to folder.
    //!param: title - The dialog title.
    //!param: defaultFilename - When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    //!param: confirmOverwrite - Enable or disable the default overwrite confirmation dialog.

    FileChooserBodyC(const XMLFactoryContextC &factory);
    //: Ctor.

    virtual bool Create();

    virtual bool Create(GtkWidget *newWidget);

    virtual bool GUISetTitle(const StringC &title) = 0;
    //: Set the dialog title.

    bool SetTitle(const StringC &title);
    //: Set the dialog title.

    bool GUISetFilename(const StringC &defaultFilename);
    //: When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    // NOTE: This will reset any added or set file filters (as of GTK 2.18.6).

    bool SetFilename(const StringC &defaultFilename);
    //: When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    // NOTE: This will reset any added or set file filters (as of GTK 2.18.6).

    StringC Filename() const
    { return m_filename; }
    //: Get the selected filename.

    bool GUISetConfirmOverwrite(const bool confirmOverwrite);
    //: Enable or disable the default overwrite confirmation dialog.

    bool SetConfirmOverwrite(const bool confirmOverwrite);
    //: Enable or disable the default overwrite confirmation dialog.

    bool GUISetFilter(const StringC &name, const DListC<StringC> patterns);
    //: Set the currently selected filter.
    // NOTE: This filter does not need to be separately added with AddFilter().
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    bool SetFilter(const StringC &name, const DListC<StringC> patterns);
    //: Set the currently selected filter.
    // NOTE: This filter does not need to be separately added with AddFilter().
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    bool GUIAddFilter(const StringC &name, const DListC<StringC> patterns);
    //: Add a user selectable filter.
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    bool AddFilter(const StringC &name, const DListC<StringC> patterns);
    //: Add a user selectable filter.
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    Signal1C<StringC> &SigSelected()
    { return m_sigSelected; }
    //: Signal emitted when a file or folder is selected. If SendEmptyStringOnCancel is enabled, an empty string is emitted if cancel is clicked or the dialog is closed.

    ConfigParameterStringC &ConfigParameterFilename()
    { return *m_configParameterFilename; }
    //: Access the selection.

    bool CBUpdateFilename();
    //: Called back to inform the wrapper the widget filename selection has been changed.

  protected:
    bool CommonCreate(GtkWidget *newWidget = NULL);
    
    bool DoSetFilename();

    bool DoSetFilter();

    bool DoAddFilter(const StringC &name, const DListC<StringC> patterns);

    virtual void Destroy();

    StringC m_title;
    FileChooserActionT m_action;
    StringC m_defaultFilename;
    StringC m_filename;
    bool m_confirmOverwrite;
    StringC m_filterSetName;
    DListC<StringC> m_filterSetPatterns;
    DListC<Tuple2C<StringC, DListC<StringC> > > m_filterList;

    Signal1C<StringC> m_sigSelected;

    ConfigParameterStringC::RefT m_configParameterFilename;
  };
  
  //! userlevel=Normal
  //: File chooser.
  
  class FileChooserC
  : public WidgetC
  {
  public:
    FileChooserC()
    {}
    //: Ctor.

    FileChooserC(FileChooserBodyC &body)
    : WidgetC(body)
    {}
    //: Body ctor.

    bool GUISetTitle(const StringC &title)
    { return Body().GUISetTitle(title); }
    //: Set the dialog title.

    bool SetTitle(const StringC &title)
    { return Body().SetTitle(title); }
    //: Set the dialog title.

    StringC Filename() const
    { return Body().Filename(); }
    //: Get the selected filename.

    bool GUISetFilename(const StringC &defaultFilename)
    { return Body().GUISetFilename(defaultFilename); }
    //: When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    // NOTE: This will reset any added or set file filters (as of GTK 2.18.6).

    bool SetFilename(const StringC &defaultFilename)
    { return Body().SetFilename(defaultFilename); }
    //: When opening, select the specified file or folder. When saving, suggest the specified file or folder.
    // NOTE: This will reset any added or set file filters (as of GTK 2.18.6).

    bool GUISetConfirmOverwrite(const bool confirmOverwrite)
    { return Body().GUISetConfirmOverwrite(confirmOverwrite); }
    //: Enable or disable the default overwrite confirmation dialog.

    bool SetConfirmOverwrite(const bool confirmOverwrite)
    { return Body().SetConfirmOverwrite(confirmOverwrite); }
    //: Enable or disable the default overwrite confirmation dialog.

    bool GUISetFilter(const StringC &name, const DListC<StringC> patterns)
    { return Body().GUISetFilter(name, patterns); }
    //: Set the currently selected filter.
    // NOTE: This filter does not need to be separately added with AddFilter().
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    bool SetFilter(const StringC &name, const DListC<StringC> patterns)
    { return Body().SetFilter(name, patterns); }
    //: Set the currently selected filter.
    // NOTE: This filter does not need to be separately added with AddFilter().
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    bool GUIAddFilter(const StringC &name, const DListC<StringC> patterns)
    { return Body().GUIAddFilter(name, patterns); }
    //: Add a user selectable filter.
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    bool AddFilter(const StringC &name, const DListC<StringC> patterns)
    { return Body().AddFilter(name, patterns); }
    //: Add a user selectable filter.
    //!param: name - The name of the filter e.g. "Image files".
    //!param: patterns - A list of shell globs e.g. a list containing the strings "*.jpg", "*.png" and "*.bmp".

    Signal1C<StringC> &SigSelected()
    { return Body().SigSelected(); }
    //: Signal emitted when a file or folder is selected. If SendEmptyStringOnCancel is enabled, an empty string is emitted if cancel is clicked or the dialog is closed.

    ConfigParameterStringC &ConfigParameterFilename()
    { return Body().ConfigParameterFilename(); }
    //: Access the selection.

  protected:
    FileChooserBodyC &Body()
    { return static_cast<FileChooserBodyC&>(WidgetC::Body()); }
    //: Body access.
    
    const FileChooserBodyC &Body() const
    { return static_cast<const FileChooserBodyC&>(WidgetC::Body()); }
    //: Body access.
  };
  
}

#endif
