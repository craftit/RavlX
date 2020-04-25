// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUIVLABEL_HEADER
#define RAVLGUIVLABEL_HEADER 1
////////////////////////////////////////////
//! file="Ravl/GUI/GTK/VLabel.hh"
//! lib=RavlGUI
//! author="Charles Galambos"
//! date="23/03/99"
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"

namespace RavlGUIN {

  class VLabelBodyC;
  
  //! userlevel=Develop
  //: Vertical Label body
  // Label with writting that is veritical instead of horizontal.
  
  class VLabelBodyC 
    : public WidgetBodyC
  {
  public:
    VLabelBodyC(const StringC &ntext);
    //: Constructor.
    
    virtual StringC Name() const;
    //: Get name of widget.
    // Defaults to WidgetName(), but may be differnt
    // for Lables etc.
    
    virtual bool Create();
    //: Create the widget.
    
    StringC &Label() { return text; }
    //: Access string.
    
    const StringC &Label() const { return text; }
    //: Access string.
    
    bool Label(const StringC &text);
    //: Update the text in the label.
    
    int TextWidth() const
      { return sx; }
    //: Get width of text.
    
    int TextHeight() const
      { return sy; }
    //: Get height of text.
    
    void SizeUpText();
    //: Sort out the text size.
    // GUI thead only.
  protected:
    
    bool GUISetText(StringC &txt);
    //: Set text.
    // GUI thread only.
    
    int sx; // Width. Size of text box.
    int sy; // Height. Size of text box.
    
    StringC text;
    
    friend class VLabelC;
  };
  
  
  //! userlevel=Normal
  //: Vertical Label.
  // Label with writting that is veritical instead of horizontal.
  
  class VLabelC 
    : public WidgetC
  {
  public:
    VLabelC(const StringC &text)
      : WidgetC(*new VLabelBodyC(text))
      {}
    //: Constructor
    
    VLabelC()
      {}
    //: Default constructor.
    // Creayes an invalid handle.
    
  protected:
    VLabelC(VLabelBodyC &bod)
      : WidgetC(bod)
      {}
    //: Body constructor.
    
    VLabelBodyC &Body()
      { return static_cast<VLabelBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const VLabelBodyC &Body() const
      { return static_cast<const VLabelBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    bool GUISetText(StringC &txt)
      { return Body().GUISetText(txt); }
    //: Set text.
    // GUI thread only.
    
  public:
    StringC &Label() 
      { return Body().Label(); }
    //: Access string.
    
    const StringC &Label() const 
      { return Body().Label(); }
    //: Access string.
    
    bool Label(const StringC &text)
      { return Body().Label(text); }
    //: Update the text in the label.
    
    friend class VLabelBodyC;
  };
  
  
  inline VLabelC VLabel(const char *label,const char *tooltip = 0) { 
    VLabelC ret = VLabelC(label);
    if(tooltip != 0)
      ret.SetToolTip(tooltip);
    return ret;
  }
  //: Create a text label.
  // With an optional tooltip. (Doesn't work at the moment...)
  
}

#endif
