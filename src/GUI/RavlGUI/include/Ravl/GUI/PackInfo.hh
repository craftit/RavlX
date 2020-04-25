// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUIPACKINFO_HEADER
#define RAVLGUIPACKINFO_HEADER 1
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/PackInfo.hh"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! author="Charles Galambos"
//! date="23/09/99"

#include "Ravl/GUI/OneChild.hh"

namespace RavlGUIN {

  //: Packing info widget.
  //! userlevel=Develop
  
  class PackInfoBodyC
    : public OneChildBodyC
  {
  public:
    PackInfoBodyC();
    //: Default constructor.
    
    PackInfoBodyC(const WidgetC &widge,bool nexpand = true,bool nfill = true,IntT npadding = 0);
    //: Constructor.

    virtual ~PackInfoBodyC();
    //: Destructor.
    
    bool &Expand()
      { return expand; }
    //: Access expand flag.
    
    bool &Fill()
      { return fill; }
    //: Access fill flag.
    
    IntT &Padding()
      { return padding; }
    //: Padding.
  
  protected:
    virtual bool Create();
    //: Create the widget.
    
    virtual void Destroy();
    //: Undo all references.
    
    bool expand;
    bool fill;
    IntT padding;
  };
  
  //! userlevel=Normal
  //: Packing info widget.

  // <p>This is a dummy widget which holds packing infomation for its child.  It
  // controls the layout, and gives extra information that tells the GUI how
  // the widge should be treated when you stretch the window or resize.</p>
  
  class PackInfoC
    : public OneChildC
  {
  public:
    PackInfoC()
      {}
    //: Default constructor.
    // Creates an invalid handle.
  
    PackInfoC(const WidgetC &widge,bool expand,bool fill = true,IntT padding = 0)
      : OneChildC(*new PackInfoBodyC(widge,expand,fill,padding))
      {}
    //: Constructor.
    // If <code>expand</code> and <code>fill</code> are both true, the behaviour is the default behaviour, i.e. as if <code>PackInfoC</code> was not used.

    
    PackInfoC(WidgetC &other);
    //: Construct packing info from other widget.
    
  protected:
    PackInfoC(PackInfoBodyC &bod)
      : OneChildC(bod)
      {}
    //: Body constructor.
    
    PackInfoBodyC &Body() 
      { return static_cast<PackInfoBodyC &>(WidgetC::Body()); }
    //: Access body.
    
    const PackInfoBodyC &Body() const
      { return static_cast<const PackInfoBodyC &>(WidgetC::Body()); }
    //: Access body.
    
  public:
    bool &Expand()
      { return Body().Expand(); }
    //: Access expand flag.
    
    bool &Fill()
      { return Body().Fill(); }
    //: Access fill flag.
    
    IntT &Padding()
      { return Body().Padding(); }
    //: Padding.
    
  };
}


#endif

