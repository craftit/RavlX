// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUIPROGRESSBAR_HEADER
#define RAVLGUIPROGRESSBAR_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! file="Ravl/GUI/GTK/ProgressBar.hh"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN{
  
  class ProgressBarC;
  
  enum ProgBarOrientationT {
    GUIPROGBAR_LEFT_TO_RIGHT,
    GUIPROGBAR_RIGHT_TO_LEFT,
    GUIPROGBAR_BOTTOM_TO_TOP,
    GUIPROGBAR_TOP_TO_BOTTOM 
  };
  
  enum ProgBarTypeT {
    GUIPROGBAR_CONTINUOUS,
    GUIPROGBAR_CONTINUOUS_WITH_TEXT,
    GUIPROGBAR_DISCRETE,
    GUIPROGBAR_ACTIVITY
  };
  
  //! userlevel=Develop
  //: Vertical or horizontal separator.
  
  class ProgressBarBodyC
    : public WidgetBodyC
  {
  public:
    
    ProgressBarBodyC(ProgBarTypeT ntype=GUIPROGBAR_CONTINUOUS ,ProgBarOrientationT norient= GUIPROGBAR_LEFT_TO_RIGHT,int nblocks = 10)
      : type(ntype),
	orient(norient),
	blocks(nblocks)
    {}
    //: Constructor.
    
    virtual bool Create();
    //: Create the widget.
    
    virtual bool Create(GtkWidget *_widget);
    //: Create the widget.
    
    bool GUIUpdate(const float &percentage);
    //: Update percentage done on bar.
    
    void Update(float percentage);
    //: Update percentage done on bar.
    
    bool GUISetType(ProgBarTypeT ntype);
    //: Set type of progress bar.
    
    bool SetType(ProgBarTypeT ntype);
    //: Set type of progress bar.
    
  protected:
    ProgBarTypeT type;
    ProgBarOrientationT orient;
    int blocks; // Number of blocks, used in discrete and activity mode.
    friend class ProgressBarC;
  };
  
  //! userlevel=Normal
  //: Vertical or horizontal separator.
  
  class ProgressBarC
    : public WidgetC
  {
  public:
    ProgressBarC()
    {}
    //: Default constructor.
    
    ProgressBarC(ProgBarTypeT ntype,
		 ProgBarOrientationT orient = GUIPROGBAR_LEFT_TO_RIGHT,
		 int nblocks = 10)
      : WidgetC(*new ProgressBarBodyC(ntype,orient,nblocks))
    {}
    //: Construct a separator.
  
  protected:
    ProgressBarC(ProgressBarBodyC &bod)
      : WidgetC(bod)
    {}
    //: Body constructor.
    
    ProgressBarBodyC &Body()
    { return static_cast<ProgressBarBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const ProgressBarBodyC &Body() const
    { return static_cast<const ProgressBarBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    void Update(float percentage)
    { Body().Update(percentage); }
    //: Update percentage done on bar.
    
    bool GUISetType(ProgBarTypeT ntype)
    { return Body().GUISetType(ntype); }
    //: Set type of progress bar.
    
    bool GUIUpdate(const float &percentage)
    { return Body().GUIUpdate(percentage); }
    //: Update percentage done on bar.
    
    bool SetType(ProgBarTypeT ntype)
    { return Body().SetType(ntype); }
    //: Set type of progress bar.
    
    friend class ProgressBarBodyC;
  };
  
  inline
  ProgressBarC ProgressBar(ProgBarTypeT ntype=GUIPROGBAR_CONTINUOUS,
			   ProgBarOrientationT orient = GUIPROGBAR_LEFT_TO_RIGHT,
			   int nblocks = 10
			   ) { 
    return ProgressBarC(ntype,orient,nblocks);
  }
  //: Create a separator.
  
}

#endif
