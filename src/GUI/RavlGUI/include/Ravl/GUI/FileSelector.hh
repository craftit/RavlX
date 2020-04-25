// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_FILESELECTOR_HEADER
#define RAVLGUI_FILESELECTOR_HEADER 1
//////////////////////////////////////////////////
//! file="Ravl/GUI/GTK/FileSelector.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/1999"
//! example=exFileSelector.cc
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal3.hh"

namespace RavlGUIN {

  class FileSelectorC;
  
  //! userlevel=Develop
  //: File Selector body
  
  class FileSelectorBodyC
    : public WidgetBodyC
  {
  public:
    FileSelectorBodyC(const StringC &nname,const StringC &filename,bool sendEmptyStringOnCancel = false);
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    bool HideOnSelect() const { return hideOnSelect; }
    //: Should file selector be hiden after selection ?
    
    Signal1C<StringC> &Selected()
    { return selected; }
    //: Access signal containing selected file name.
    
    bool GUISetTitle(const StringC &name);
    //: Set the title of the window.
    // GUI thread only.
    
    bool SetTitle(const StringC &name);
    //: Set the title of the window.
    
    bool GUISetFilename(const StringC &name);
    //: Set file to open
    // GUI thread only.
    
    bool SetFilename(const StringC &name);
    //: Set file to open
    
    void HandleCancel();
    //: Handle selection cancel.
    
    bool SetSendEmptyStringOnCancel(bool val)
    { return sendEmptyStringOnCancel = val; }
    //: Enable/Disable sending of empty string on cancel.
    
  protected:
    void Destroy();
    //: Dissconnect all signals.
    
    StringC name;
    StringC defaultFN; // Default filename
    bool hideOnSelect; // Hide the fileselector after selection ?
    bool sendEmptyStringOnCancel;
    
    Signal1C<StringC> selected;
    
    friend class FileSelectorC;
  };
  
  //! userlevel=Normal
  //: File Selector
  // <p> Pops up a browser in separate window that browses file systems to select a file.</p>
  // <p> Typical usage:</p><ul>
  // <li> Create a file selector object and connect it to the callback that deals with the selected filename.</li>
  // <li> Create a button for it and connect the button to the FileSelectorC::GUIShow method of the file selector object.</li></ul>
  
  class FileSelectorC 
    : public WidgetC
  {
  public:
    FileSelectorC(const StringC &nname = "File Selector",const StringC &filename = ".",bool sendEmptyStringOnCancel = false)
      : WidgetC(*new FileSelectorBodyC(nname,filename,sendEmptyStringOnCancel))
    {}
    //: Constructor
    
    FileSelectorC(FileSelectorBodyC &bod)
      : WidgetC(bod)
      {}
    //: Body constructor.

    FileSelectorC(bool createBody)
    {
      if(createBody)
        *this = FileSelectorC();
    }
    //: Constructor.
    // Will create an invalid handle if passed false.
    
  protected:
    FileSelectorBodyC &Body()
      { return static_cast<FileSelectorBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const FileSelectorBodyC &Body() const
      { return static_cast<const FileSelectorBodyC &>(WidgetC::Body()); }
    //: Access body.  
  public:
    
    Signal1C<StringC> &Selected()
    { return Body().Selected(); }
    //: Access signal containing selected file name.
    
    void SetHideOnSelect(bool val)
    { Body().hideOnSelect = val; }
    //: Set hide on select flag.
    // Thread safe, goes into effect immediately.
    
    bool GUISetTitle(const StringC &name)
    { return Body().GUISetTitle(name); }
    //: Set the title of the window.
    // GUI thread only.
    
    bool SetTitle(const StringC &name)
    { return Body().SetTitle(name); }
    //: Set the title of the window.
    
    bool GUISetFilename(const StringC &name)
    { return Body().GUISetFilename(name); }
    //: Set file to open
    // GUI thread only.
    
    bool SetFilename(const StringC &name)
    { return Body().SetFilename(name); }
    //: Set file to open
    
    bool SetSendEmptyStringOnCancel(bool val)
    { return  Body().SetSendEmptyStringOnCancel(val); }
    //: Enable/Disable sending of empty string on cancel.
    // Default is disabled
    
  };
  
  //////////////////////////////////////////////////////////////
  
  //: Call a function.

  inline
  FileSelectorC FileSelector(const StringC &fsname,
			     bool (*func)(StringC &filename,FileSelectorC &),
			     const StringC &filename = StringC())
  { 
    FileSelectorC ret(fsname,filename);
    Connect(ret.Selected(),func,filename,ret);
    return ret;    
  }  

  //: Call a function.

  inline
  FileSelectorC FileSelector(const StringC &fsname,
			     bool (*func)(StringC &filename),
			     const StringC &filename = StringC())
  {
    FileSelectorC ret(fsname,filename);
    Connect(ret.Selected(),func,filename);
    return ret;    
  }
  
  //: Call a method.
  
  template<class DataT>
  FileSelectorC FileSelector(const StringC &fsname,
			     const DataT &obj,
			     bool (DataT::*func)(StringC &filename),const StringC &fn = StringC())
  { 
    FileSelectorC ret(fsname,fn);
    Connect(ret.Selected(),obj,func);
    return ret;    
  }  

  
  //: Call a function.
  
  template<class DataT>
  FileSelectorC FileSelector(const StringC &fsname,
			     const StringC &filename,
			     bool (*func)(StringC &filename,FileSelectorC &,DataT &ref),
			     const DataT &dat)
  { 
    FileSelectorC ret(fsname,filename);
    StringC fn(filename); // Convert to a real filename.
    Connect(ret.Selected(),func,fn,ret,dat);
    return ret;    
  }  
  
  //: Call a method.
  // Hold only a reference to the destination class.
  
  template<class DataT>
  FileSelectorC FileSelectorR(const StringC &fsname,
			      DataT &obj,
			      bool (DataT::*func)(StringC &filename),const StringC &fn = StringC())
  { 
    FileSelectorC ret(fsname,fn);
    ConnectRef(ret.Selected(),obj,func);
    return ret;    
  }  

  //: Call a method with a parameter
  // Hold only a reference to the destination class.
  
  template<class DataT, class ParamT>
  FileSelectorC FileSelectorR(const StringC &fsname,
			      DataT &obj,
			      bool (DataT::*func)(StringC &filename,ParamT &param),const StringC &fn = StringC(),
			      const ParamT &param = ParamT())
  { 
    FileSelectorC ret(fsname,fn);
    ConnectRef(ret.Selected(),obj,func,"",param);
    return ret;    
  }  
  


}


#endif
