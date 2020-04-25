// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_COLOURSELECTOR_HEADER
#define RAVLGUI_COLOURSELECTOR_HEADER 1
//////////////////////////////////////////////////
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="12/07/2004"
//! example=exColourSelector.cc
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! file="Ravl/GUI/GTK/ColourSelector.hh"

#include "Ravl/config.h"
#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Threads/Signal3.hh"
#include "Ravl/Image/UInt16RGBValue.hh"

namespace RavlGUIN {
  class FileSelectorC;
  
  //! userlevel=Develop
  //: ColourSelector body
  
  class ColourSelectorBodyC
    : public WidgetBodyC
  {
  public:
    ColourSelectorBodyC(const StringC &nname,const UInt16RGBValueC &colour);
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    bool HideOnSelect() const { return hideOnSelect; }
    //: Should file selector be hiden after selection ?
    
    Signal1C<UInt16RGBValueC> &Selected()
    { return selected; }
    //: Access selected signal.
    
    bool GUISetTitle(const StringC &name);
    //: Set the title of the window.
    // GUI thread only.
    
    bool SetTitle(const StringC &name);
    //: Set the title of the window.
    
    void HandleCancel();
    //: Handle selection cancel.
    
    void HandleSelect();
    //: Handle selection 
    
  protected:
    void Destroy();
    //: Disconnect all signals.
    
    StringC name;
    UInt16RGBValueC colour;
    bool hideOnSelect; // Hide the fileselector after selection ?
    
    Signal1C<UInt16RGBValueC> selected;
    
    friend class ColourSelectorC;
  };
  
  //! userlevel=Normal
  //: ColourSelector
  
  class ColourSelectorC 
    : public WidgetC
  {
  public:
    ColourSelectorC(const StringC &nname = "Colour Selector",const UInt16RGBValueC &colour = UInt16RGBValueC(0,0,0))
      : WidgetC(*new ColourSelectorBodyC(nname,colour))
    {}
    //: Constructor
    
    ColourSelectorC(ColourSelectorBodyC &bod)
      : WidgetC(bod)
      {}
    //: Body constructor.
    
  protected:
    ColourSelectorBodyC &Body()
      { return static_cast<ColourSelectorBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ColourSelectorBodyC &Body() const
      { return static_cast<const ColourSelectorBodyC &>(WidgetC::Body()); }
    //: Access body.  
  public:
    
    Signal1C<UInt16RGBValueC> &Selected()
    { return Body().Selected(); }
    //: Access selected signal.
    
    void SetHideOnSelect(bool val)
    { Body().hideOnSelect = val; }
    //: Set hide on select flag.
    // Thread safe, goes into effect immediatly.
    
    bool GUISetTitle(const StringC &name)
    { return Body().GUISetTitle(name); }
    //: Set the titel of the window.
    // GUI thread only.
    
    bool SetTitle(const StringC &name)
    { return Body().SetTitle(name); }
    //: Set the titel of the window.
    
    
  };
  
  //////////////////////////////////////////////////////////////
  
  //: Call a function.
  
  inline
  ColourSelectorC ColourSelector(const StringC &fsname,
				 bool (*func)(UInt16RGBValueC &colour,ColourSelectorC &),
				 const UInt16RGBValueC &initialColour = UInt16RGBValueC(255,0,0))
  { 
    ColourSelectorC ret(fsname,initialColour);
    Connect(ret.Selected(),func,initialColour,ret);
    return ret;    
  }  

  //: Call a function.

  inline
  ColourSelectorC ColourSelector(const StringC &fsname,
				 bool (*func)(UInt16RGBValueC &colour),
				 const UInt16RGBValueC &initialColour = UInt16RGBValueC(255,0,0))
  {
    ColourSelectorC ret(fsname,initialColour);
    Connect(ret.Selected(),func,initialColour);
    return ret;    
  }
  
  //: Call a method.
  
  template<class DataT>
  ColourSelectorC ColourSelector(const StringC &fsname,
			     const DataT &obj,
			     bool (DataT::*func)(UInt16RGBValueC &colour),const UInt16RGBValueC &initialColour = UInt16RGBValueC(255,0,0))
  { 
    ColourSelectorC ret(fsname,initialColour);
    Connect(ret.Selected(),obj,func);
    return ret;    
  }  

  
  //: Call a function.
  
  template<class DataT>
  ColourSelectorC ColourSelector(const StringC &fsname,
				 const UInt16RGBValueC &colour,
				 bool (*func)(UInt16RGBValueC &colour,ColourSelectorC &,DataT &ref),
				 const DataT &dat)
  { 
    ColourSelectorC ret(fsname,colour);
    Connect(ret.Selected(),func,colour,ret,dat);
    return ret;    
  }  
  
  //: Call a method.
  // Hold only a reference to the destination class.
  
  template<class DataT>
  ColourSelectorC ColourSelectorR(const StringC &fsname,
				  DataT &obj,
				  bool (DataT::*func)(UInt16RGBValueC &colour),const UInt16RGBValueC &initialColour = UInt16RGBValueC(255,0,0))
  { 
    ColourSelectorC ret(fsname,initialColour);
    ConnectRef(ret.Selected(),obj,func);
    return ret;    
  }  

  //: Call a method with a parameter
  // Hold only a reference to the destination class.
  
  template<class DataT, class ParamT>
  ColourSelectorC ColourSelectorR(const StringC &fsname,
				  DataT &obj,
				  bool (DataT::*func)(UInt16RGBValueC &colour,ParamT &param),const UInt16RGBValueC &initialColour = UInt16RGBValueC(255,0,0),
				  const ParamT &param = ParamT())
  { 
    ColourSelectorC ret(fsname,initialColour);
    ConnectRef(ret.Selected(),obj,func,UInt16RGBValueC(255,0,0),param);
    return ret;    
  }  

}


#endif
