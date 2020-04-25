// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/FileChooser.cc"
//! lib=RavlGUI
//! author = "Warren Moore"

#include "Ravl/GUI/FileChooser.hh"
#include "Ravl/OS/Filename.hh"
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

  FileChooserBodyC::FileChooserBodyC(const FileChooserActionT action,
                                     const StringC &title,
                                     const StringC &defaultFilename,
                                     const bool confirmOverwrite)
  : m_title(title),
    m_action(action),
    m_defaultFilename(defaultFilename),
    m_filename(defaultFilename),
    m_confirmOverwrite(confirmOverwrite),
    m_sigSelected(StringC(""))
  {}
  
  FileChooserBodyC::FileChooserBodyC(const XMLFactoryContextC &factory)
  : WidgetBodyC(factory),
    m_title("File Chooser"),
    m_action(FileChooserOpen),
    m_defaultFilename(""),
    m_filename(""),
    m_confirmOverwrite(true),
    m_sigSelected(StringC(""))
  {
    rThrowBadConfigContextOnFailS(factory, UseComponent("Selection", m_configParameterFilename), "Failed to load filename selector.");
  }

  bool FileChooserBodyC::Create()
  {
    return CommonCreate();
  }

  bool FileChooserBodyC::Create(GtkWidget *newWidget)
  {
    return CommonCreate(newWidget);
  }

  bool FileChooserBodyC::CommonCreate(GtkWidget *newWidget)
  {
    ONDEBUG(std::cerr << "FileChooserBodyC::CommonCreate" << endl);

    if (newWidget)
    {
      widget = newWidget;

      GtkFileChooserAction action = gtk_file_chooser_get_action(GTK_FILE_CHOOSER(widget));
      switch (action)
      {
        case GTK_FILE_CHOOSER_ACTION_OPEN: m_action = FileChooserOpen; break;
        case GTK_FILE_CHOOSER_ACTION_SAVE: m_action = FileChooserSave; break;
        case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER: m_action = FileChooserSelectFolder; break;
        case GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER: m_action = FileChooserCreateFolder; break;
        default:
          RavlAssertMsg(false, "Unknown file chooser action.");
          break;
      }

      m_confirmOverwrite = gtk_file_chooser_get_do_overwrite_confirmation(GTK_FILE_CHOOSER(widget));
    }

    if (m_defaultFilename != "")
      DoSetFilename();

    if (m_filterList.Size() > 0)
      for (DLIterC<Tuple2C<StringC, DListC<StringC> > > filterIter(m_filterList); filterIter; filterIter++)
        DoAddFilter(filterIter->Data1(), filterIter->Data2());

    if (m_filterSetPatterns.Size() > 0)
      DoSetFilter();
    
    ConnectSignals();

    return true;
  }

  bool FileChooserBodyC::GUISetFilename(const StringC &defaultFilename)
  {
    m_defaultFilename = defaultFilename;

    if (widget)
    {
      return DoSetFilename();
    }

    return true;
  }

  bool FileChooserBodyC::SetFilename(const StringC &defaultFilename)
  {
    Manager.Queue(Trigger(FileChooserC(*this), &FileChooserC::GUISetFilename, defaultFilename));
    return true;
  }

  bool FileChooserBodyC::GUISetConfirmOverwrite(const bool confirmOverwrite)
  {
    m_confirmOverwrite = confirmOverwrite;
    
    if (widget)
    {
      RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");

      gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(widget), confirmOverwrite);
    }

    return true;
  }

  bool FileChooserBodyC::SetConfirmOverwrite(const bool confirmOverwrite)
  {
    Manager.Queue(Trigger(FileChooserC(*this), &FileChooserC::GUISetConfirmOverwrite, confirmOverwrite));
    return true;
  }

  bool FileChooserBodyC::GUISetFilter(const StringC &name, const DListC<StringC> patterns)
  {
    m_filterSetName = name;
    m_filterSetPatterns = patterns;
    
    if (widget)
    {
      return DoSetFilter();
    }

    return true;
  }

  bool FileChooserBodyC::SetFilter(const StringC &name, const DListC<StringC> patterns)
  {
    Manager.Queue(Trigger(FileChooserC(*this), &FileChooserC::GUISetFilter, name, patterns));
    return true;
  }

  bool FileChooserBodyC::GUIAddFilter(const StringC &name, const DListC<StringC> patterns)
  {
    if (patterns.Size() == 0)
      return true;

    for (DLIterC<Tuple2C<StringC, DListC<StringC> > > filterListIter(m_filterList); filterListIter; filterListIter++)
      if (name == filterListIter->Data1())
        return true;

    m_filterList.InsLast(Tuple2C<StringC, DListC<StringC> >(name, patterns));

    if (widget)
    {
      GtkFileFilter *filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(widget));
      if (filter)
      {
        const char *filterName = gtk_file_filter_get_name(filter);
        if (filterName && name == filterName)
        {
          gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(widget), filter);
          return true;
        }
      }
      
      return DoAddFilter(name, patterns);
    }

    return true;
  }

  bool FileChooserBodyC::AddFilter(const StringC &name, const DListC<StringC> patterns)
  {
    Manager.Queue(Trigger(FileChooserC(*this), &FileChooserC::GUIAddFilter, name, patterns));
    return true;
  }

  bool FileChooserBodyC::CBUpdateFilename()
  {
    RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(widget);

    m_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
    ONDEBUG(std::cerr << "FileChooserBodyC::CBUpdateFilename filename(" << m_filename << ")" << endl);

    if (m_configParameterFilename.IsValid())
      m_configParameterFilename->SetValue(m_filename);

    return true;
  }

  bool FileChooserBodyC::DoSetFilename()
  {
    RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(widget);

    FilenameC filenameObject(m_defaultFilename);
    if (filenameObject.Exists())
    {
      gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widget), filenameObject);
    }
    else
    {
      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(widget), filenameObject.PathComponent());
      if (m_action == FileChooserSave || m_action == FileChooserCreateFolder)
        gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(widget), filenameObject.NameComponent());
    }

    CBUpdateFilename();
    
    return true;
  }

  bool FileChooserBodyC::DoSetFilter()
  {
    RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(widget);

    if (m_filterList.Size() > 0)
    {
      GSList *listedFilters = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(widget));
      if (listedFilters)
      {
        GSList *listedFilterIter = listedFilters;
        while (listedFilterIter)
        {
          GtkFileFilter *filter = GTK_FILE_FILTER(listedFilterIter->data);
          const char *filterName = gtk_file_filter_get_name(filter);
          ONDEBUG(std::cerr << "FileChooserBodyC::GUISetFilter name(" << m_filterSetName << ") checking name(" << filterName << ")" << endl);
          if (filterName && m_filterSetName == filterName)
          {
            ONDEBUG(std::cerr << "FileChooserBodyC::GUISetFilter name(" << m_filterSetName << ") found name(" << filterName << ")" << endl);
            gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(widget), filter);
            break;
          }

          listedFilterIter = listedFilterIter->next;
        }

        g_slist_free(listedFilters);
      }
    }
    else
    {
      GtkFileFilter *filter = gtk_file_filter_new();
      gtk_file_filter_set_name(filter, m_filterSetName);
      for (DLIterC<StringC> patternIter(m_filterSetPatterns); patternIter; patternIter++)
      {
        ONDEBUG(std::cerr << "FileChooserBodyC::GUISetFilter name(" << m_filterSetName << ") adding pattern(" << *patternIter << ")" << endl);
        gtk_file_filter_add_pattern(filter, *patternIter);
      }

      gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(widget), filter);
    }

    return true;
  }

  bool FileChooserBodyC::DoAddFilter(const StringC &name, const DListC<StringC> patterns)
  {
    RavlAssertMsg(Manager.IsGUIThread(), "Incorrect thread. This method may only be called on the GUI thread.");
    RavlAssert(widget);

    bool filterFound = false;
    GSList *listedFilters = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(widget));
    if (listedFilters)
    {
      GSList *listedFilterIter = listedFilters;
      while (listedFilterIter)
      {
        GtkFileFilter *filter = GTK_FILE_FILTER(listedFilterIter->data);
        const char *filterName = gtk_file_filter_get_name(filter);
        ONDEBUG(std::cerr << "FileChooserBodyC::DoAddFilter name(" << m_filterSetName << ") checking name(" << filterName << ")" << endl);
        if (filterName && name == filterName)
        {
          ONDEBUG(std::cerr << "FileChooserBodyC::DoAddFilter name(" << m_filterSetName << ") found name(" << filterName << ")" << endl);
          filterFound = true;
          break;
        }

        listedFilterIter = listedFilterIter->next;
      }

      g_slist_free(listedFilters);
    }

    if (!filterFound)
    {
      GtkFileFilter *filter = gtk_file_filter_new();
      gtk_file_filter_set_name(filter, name);
      for (DLIterC<StringC> patternIter(patterns); patternIter; patternIter++)
      {
        ONDEBUG(std::cerr << "FileChooserBodyC::GUIAddFilter name(" << name << ") adding pattern(" << *patternIter << ")" << endl);
        gtk_file_filter_add_pattern(filter, *patternIter);
      }

      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(widget), filter);
    }

    return true;
  }

  void FileChooserBodyC::Destroy()
  {
    m_sigSelected.DisconnectAll();
    WidgetBodyC::Destroy();
  }

}
