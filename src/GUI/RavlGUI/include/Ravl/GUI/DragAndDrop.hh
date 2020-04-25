// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DRAGANDDROP_HEADER
#define RAVLGUI_DRAGANDDROP_HEADER 1
//! author="Charles Galambos"
//! date="15/06/2002"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/DragAndDrop.hh"

#include "Ravl/GUI/Widget.hh"

namespace RavlGUIN {
  
  //! userlevel=Normal
  //: Position and time of an event.
  
  class PositionTimeC {
  public:
    PositionTimeC()
    {}
    //: Default constructor.
    
    PositionTimeC(const Index2dC &nat,UIntT ntime)
      : at(nat),
	time(ntime)
    {}
    //: Constructor.

    PositionTimeC(int x,int y,UIntT ntime)
      : at(x,y),
	time(ntime)
    {}
    //: Constructor.
    
    Index2dC &At()
    { return at; }
    //: Access location.
    
    UIntT Time() const
    { return time; }
    //: Access time of click
    
    Index2dC at; // Position of click.
    UIntT time;  // Time of click.
  };
  
  //! userlevel=Normal
  //: Drag and drop info.
  
  class DNDDataInfoC {
  public:
    DNDDataInfoC()
      : context(0),
	data(0),
	info(0),
	time(0)
    {}
    //: Default constructor.
    
    DNDDataInfoC(GdkDragContext *dc,GtkSelectionData *data,UIntT ninfo,UIntT ntime,int x = 0 ,int y = 0)
      : context(dc),
	data(data),
	info(ninfo),
	time(ntime),
	at(y,x)
    {}
    //: Default constructor.
    
    bool Finish(bool success,bool del);
    //: Finish drag and drop.
    // This must be called on the GUI thread.
    
    bool IsString() const;
    //: Is recieved data a string ?
    
    StringC String();
    //: Get data as string.
    // It is the user's responsibility to ensure this is appropriate.
    
    GtkWidget *GTKSourceWidget();
    // Find the GTK source widget.
    
    GtkSelectionData *Data()
    { return data; }
    //: Access data.
    
    UIntT Info() const
    { return info; }
    //: Info field.
    
    Index2dC &At()
    { return at; }
    //: Return location of event.
    
    UIntT Time() const
    { return time; }
    //: Return time of event.
    
    bool PutString(int dtype,const StringC &str);
    //: Put a string.
    // Used in response to 'data_get'. <br>
    // Note: dtype is currently ignored under GTK+-2
    
    DragActionT SuggestedAction();
    //: Get suggested action.
    // See GdkDragAction for definition of bit mask.
    
    DragActionT Actions();
    //: Possible actions that can be taken
    // See GdkDragAction for definition of bit mask.
    
  protected:
    // Info....
    GdkDragContext *context;
    GtkSelectionData *data;
    UIntT info;
    UIntT time;  // Time of click.
    Index2dC at;
  };

  
}


#endif
